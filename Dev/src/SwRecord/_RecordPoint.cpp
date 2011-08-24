/**
 * @file _RecordPoint.cpp
 * @brief Record Point
 * @version 1.0
 * @date Tue Apr 28 15:55:16 CEST 2009
 * @author F.Bighelli
 */


#include <SwApplication.h>
#include <SwMacros.h>
#include "_RecordPoint.h"
#include "ISwServiceRecording.h"
#include <QXmlStreamWriter>
#include "SwAddress_ToolBox.h"
#include "_SwConfigurationSelector.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwRecord;

//-------------------------------------------------------------------------
_RecordPoint::_RecordPoint():SwAssistedComponent() 
{
    _pinIn			= 0;
    _pinOut			= 0;
    _recordManager	= 0;
	_isRecording	= false;

	setExecutableServiceAvaibility(true);
	setPinServiceAvaibility(true);

    ISwServiceRecording * rservice=dynamic_cast<ISwServiceRecording *>(SW_APP->QueryService(CG_SW_SERVICE_RECORDING));

    if (rservice!=0) 
	{
        QStringList liste=rservice->getSwDataCodecs();

        for(int i=0;i<liste.count();i++)
		{
            _dataType.AddKey(i,liste[i]);
        }

        _dataType.FromInt(0);
        _codec=rservice->buildCodec(liste[0]);
    }

}
//-------------------------------------------------------------------------
_RecordPoint::~_RecordPoint() 
{
	unregisterPin(_pinIn);
	unregisterPin(_pinOut);

	//DETRUIRE LES AUTRES ATTRIBUTS DE LA CLASSE
    if ( _codec != 0 ) 
	{
        _codec->destroy();
    }   

    ISwServiceRecording * rservice=dynamic_cast<ISwServiceRecording *>(SW_APP->QueryService(CG_SW_SERVICE_RECORDING));
    if ( rservice != 0 ) 
	{
        rservice->unregisterRecordPoint(this);
    }
}

//-------------------------------------------------------------------------
void _RecordPoint::eventReceiveData(SwPin * src,SwData_Class * data)
{
    if ( src == _pinIn ) 
	{
        //Enregistrement
        data->_addRef();
        _recordQueue.push_back(data);
    } 
	else
	{
        //Sinon ByPass
        _pinIn->SendData(data);
    }
}

//-------------------------------------------------------------------------
SwUUID _RecordPoint::getRecordIdentifier() 
{
    return _identifier;
}

//-------------------------------------------------------------------------
QString _RecordPoint::getRecordName() 
{
    QString goodName=SwAddress_ToolBox::BuildUniversalPath(this);
    goodName.replace(QRegExp("[\\.$]"), "_");
    return goodName;
}

//-------------------------------------------------------------------------
void _RecordPoint::setRecordManager(ISwRecordManager * recordManager) 
{
    _recordManager = recordManager;
}

//-------------------------------------------------------------------------
bool _RecordPoint::buildKey(QXmlStreamReader * reader) 
{
    SwData_Class * data=new SwData_Class();
    data->_addRef();
    data = (SwData_Class *)_codec->decode(reader,data);

    _waitingQueue.push_back(data);

    return true;
}

//-------------------------------------------------------------------------
void _RecordPoint::submitKey()
{
    _sendingQueue.push_back(_waitingQueue.front());
    _waitingQueue.pop_front();
}

//-------------------------------------------------------------------------
void _RecordPoint::cleanKeys() 
{
    while (!_waitingQueue.isEmpty()) 
	{
        _waitingQueue.front()->_release();
        _waitingQueue.pop_front();
    }

    while (!_sendingQueue.isEmpty()) 
	{
        _sendingQueue.front()->_release();
        _sendingQueue.pop_front();
    }
}

//-------------------------------------------------------------------------
SwEnum _RecordPoint::getDataType() const 
{
    return _dataType;
}

//-------------------------------------------------------------------------
void _RecordPoint::setDataType(const SwEnum & val) 
{
    if(_dataType==val) 
	{
        return;
    }

	//Sur changement du type de données, on supprime les connecteurs
	unregisterPin(_pinIn);
	unregisterPin(_pinOut);

    //On affecte la valeur
    _dataType=val;

    //on reconstruit les connecteurs (True = Listener, => pinIn Listener = Bidirectionalité)
	_pinIn=registerPin(QString("in"),_dataType.ToString(),true);
	_pinOut=registerPin(QString("out"),_dataType.ToString(),true);

    //On recupere le codec associé
    if (_codec!=0) 
	{
        _codec->destroy();
    }   

    ISwServiceRecording * rservice=dynamic_cast<ISwServiceRecording *>(SW_APP->QueryService(CG_SW_SERVICE_RECORDING));
    if (rservice!=0) 
	{
        _codec=rservice->buildCodec(_dataType.ToString());
    } 
	else 
	{
		_codec=0;
	}
}

//-------------------------------------------------------------------------
SwUUID _RecordPoint::getIdentifier() const
{
    return _identifier;
}

//-------------------------------------------------------------------------
void _RecordPoint::setIdentifier(const SwUUID & id) 
{
    ISwServiceRecording * rservice=dynamic_cast<ISwServiceRecording *>(SW_APP->QueryService(CG_SW_SERVICE_RECORDING));
    if (rservice!=0) 
	{
        rservice->unregisterRecordPoint(this);
    }

    _identifier=id;

    if (rservice!=0) 
	{
        rservice->registerRecordPoint(this);
    }
}

//-------------------------------------------------------------------------
void _RecordPoint::Initialize(double start_time,ISwExecution_Service * executor) throw (SwException) 
{
    _currentTime=start_time;
   // _recordQueue.clear();
}

//-------------------------------------------------------------------------
void _RecordPoint::Start(double current_time) throw (SwException)
{
    _currentTime=current_time;

	//Si enregistrement en cours
	/*if (_recordManager != 0 && _recordQueue.count()>0) 
	{
		registerPropertiesListener();
	}*/
}           

//-------------------------------------------------------------------------
void _RecordPoint::Execute(double current_time,bool is_first_call) throw (SwException)
{
	 QXmlStreamWriter *writer = NULL;
	 _currentTime = current_time;

	//Si on est en enregist
	if(_recordManager && _recordManager->isRecording() && !_isRecording)
	{
		_isRecording = true;
		_recordQueue.clear();
		registerPropertiesListener();
	}

	if( (_recordManager && !_recordManager->isRecording() && _isRecording ) || !_recordManager )
	{
		_isRecording = false;
	}
	
    //Si enregistrement en cours
    if ( _isRecording && !_recordQueue.isEmpty()) 
	{
        writer = _recordManager->queryRecordKey(this,current_time);

		while (!_recordQueue.isEmpty()) 
		{
			SwData_Class * data=_recordQueue.front();
			_recordQueue.pop_front();

			//Si enregistrement en cours
			if (writer != NULL && _isRecording) 
			{
				_codec->encode(writer,(void *)data);
			}

			//Envoie
			_pinOut->SendData(data);

			//Liberation
			data->_release();
		}

        _recordManager->finalizeRecordKey();
    }


    //Si données de rejeu a emettre
    while (!_sendingQueue.isEmpty()) 
	{
        SwData_Class * data=_sendingQueue.front();
        _sendingQueue.pop_front();

        //Envoie
        _pinOut->SendData(data);

        //Liberation
        data->_release();
    }
}            

//-------------------------------------------------------------------------
void _RecordPoint::Stop(double current_time)
{
    _currentTime=current_time;
}         

//-------------------------------------------------------------------------
void _RecordPoint::AdminSetup()
{

	SwComponent_Class *root;
	root=this;
	while (root->GetParent()!=NULL) root=root->GetParent();

	_SwConfigurationSelector * selector=new _SwConfigurationSelector(0,root,&_exported_entities,true);
	if (selector->exec()==QDialog::Accepted) 
	{
		selector->ValidChange();
	}
}

//-------------------------------------------------------------------------
void _RecordPoint::initializeComponent() throw(SwException)
{
	QVariant tmp;

	//--------------------------------------
	//Definition Pins
	//--------------------------------------
	_pinIn=registerPin(QString("in"),_dataType.ToString(),true);
	_pinOut=registerPin(QString("out"),_dataType.ToString(),true);

	//--------------------------------------
	//Definition Properties
	//--------------------------------------
	createPropertiesForThisObject("",true);

	ISwProperty * p = getISwProperty("identifier");

	QVariant v;
	v.setValue(SwUUID::generateUUID());
	p->SetValue(v);
}

//-------------------------------------------------------------------------
void _RecordPoint::Load( QDomElement & elt,ISwFinalizerManager & finalizer_manager )
{
	QDomElement elt_ent;
	_SwConfigurationExportedEntity *entity;

	for(elt_ent = elt.firstChildElement(); !elt_ent.isNull(); elt_ent = elt_ent.nextSiblingElement())
	{
		entity=NULL;
		if ( elt_ent.nodeName() == QString(CL_CONFIG_XML_NODE_PROPERTY) ) 
		{
			entity=_SwConfigurationExportedEntity::NewEntity(_SwConfigurationExportedEntity::Ent_Property);
		}

		if (entity!=NULL &&
			elt_ent.hasAttribute(CL_CONFIG_XML_NODE_ATT_NAME) &&
			elt_ent.hasAttribute(CL_CONFIG_XML_NODE_ATT_EXP_NAME) &&
			elt_ent.hasAttribute(CL_CONFIG_XML_NODE_ATT_PATH)) 
		{
			entity->_name=elt_ent.attribute(CL_CONFIG_XML_NODE_ATT_NAME);    
			entity->_exported_name=elt_ent.attribute(CL_CONFIG_XML_NODE_ATT_EXP_NAME);    
			entity->_host_path=elt_ent.attribute(CL_CONFIG_XML_NODE_ATT_PATH);   
			_exported_entities.push_back(entity);
		} 
		else 
		{
			delete entity;
		}
	}

	bool result;
	h_index=elt.attribute(CL_CONFIG_XML_NODE_ATT_IDX).toULongLong(&result);

	if ( result == false )
		h_index=SW_APP->GetHistoricCpt();

	finalizer_manager.RegisterFinalization(h_index,this);
}

//-------------------------------------------------------------------------
void _RecordPoint::Save( QDomElement & elt,QDomDocument &doc )
{
	QDomElement elt_ent;

	for( int i=0 ; i < _exported_entities.count() ; i++ ) 
	{
		//Creation du noeud
		if( _exported_entities[i]->_type == _SwConfigurationExportedEntity::Ent_Property )
		{
			elt_ent = doc.createElement(CL_CONFIG_XML_NODE_PROPERTY);
		
			//Affectation des attributs
			elt_ent.setAttribute(CL_CONFIG_XML_NODE_ATT_NAME,_exported_entities[i]->_name);
			elt_ent.setAttribute(CL_CONFIG_XML_NODE_ATT_EXP_NAME,_exported_entities[i]->_exported_name);
			elt_ent.setAttribute(CL_CONFIG_XML_NODE_ATT_PATH,_exported_entities[i]->_host_path);

			//Attachement du neoud au parent
			elt.appendChild(elt_ent);
			elt.setAttribute(CL_CONFIG_XML_NODE_ATT_IDX,h_index);
		}
	}
}

//-------------------------------------------------------------------------
bool _RecordPoint::Finalize( quint64 historic_index )
{
	if ( h_index == historic_index ) 
	{
		//Observe all property
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------
void _RecordPoint::registerPropertiesListener()
{
	SwComponent_Class * _host;
	SwComponent_Class * root;

	root=this;
	while (root->GetParent()!=NULL) 
		root = root->GetParent();

	for( int i=0 ; i<_exported_entities.count() ; i++) 
	{
		//On cherche le composant par rapport au noeuf root du stream
		_host=SwAddress_ToolBox::FindTarget(_exported_entities[i]->_host_path,root);

		//On récupère le service de propriétés du composant 
		ISwProperties * _internal_properties=dynamic_cast<ISwProperties *>(_host->QueryService(CG_SW_SERVICE_PROPERTIES));  

		//On récupere la ISwProperty
		ISwProperty* prop =  _internal_properties->GetProperty(_exported_entities[i]->_name);

		//On s'enregistre en tant que listener de la propriété

		prop->GetOnChangeSignal().iconnect(*this,&_RecordPoint::OnComponentPropertyChange);

#ifdef  _DEBUG
	//qDebugAA
	qDebug() << (_SwConfigurationExportedEntity)(*_exported_entities[i]) ;
#endif
	}

}

//-------------------------------------------------------------------------
void _RecordPoint::OnComponentPropertyChange( ISwProperty * prop )
{
	#ifdef  _DEBUG
	//qDebugAA
	qDebug() << "je suis notifie";
#endif
}
