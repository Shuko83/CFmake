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

using namespace StreamWork::SwCore;
using namespace StreamWork::SwRecord;

/** @brief Constructeur */
_RecordPoint::_RecordPoint():SwComponent_Class() {
    _properties_service=NULL;
    _provider_service=NULL;
    _pins_service=NULL;
	_consumer_service=NULL;
    _pinIn=0;
    _pinOut=0;
    _recordManager=0;

	//RESET Interfaces

	//RESET Pins

	//RESET Properties
	//INITIALIZER LES AUTRES ATTRIBUTS DE LA CLASSE
    

    ISwServiceRecording * rservice=dynamic_cast<ISwServiceRecording *>(SW_APP->QueryService(CG_SW_SERVICE_RECORDING));
    if (rservice!=0) {
        QStringList liste=rservice->getSwDataCodecs();
        for(int i=0;i<liste.count();i++) {
            _dataType.AddKey(i,liste[i]);
        }
        _dataType.FromInt(0);
        _codec=rservice->buildCodec(liste[0]);
    }

}
/** @brief Destructeur */
_RecordPoint::~_RecordPoint() {
	//Destruction properties automatiques
	//Desenregistrement automatiques des interfaces
	//Destruction Pins
    _pins_service->UnregisterPin(_pinIn);
    delete _pinIn;
    _pins_service->UnregisterPin(_pinOut);
    delete _pinOut;

    //Desenregistrement des services
    this->UnregisterService(GetServiceName());
    this->UnregisterService(_pins_service->GetServiceName());
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _pins_service;
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;

	//DETRUIRE LES AUTRES ATTRIBUTS DE LA CLASSE
    if (_codec!=0) {
        _codec->destroy();
    }   
    ISwServiceRecording * rservice=dynamic_cast<ISwServiceRecording *>(SW_APP->QueryService(CG_SW_SERVICE_RECORDING));
    if (rservice!=0) {
        rservice->unregisterRecordPoint(this);
    }
}
/** 
 * @brief Initialisation des ressources
 * @note tous les services du composants doivent être déclarés dans cette methodes
 */
void _RecordPoint::InitializeResources() throw(SwException) {
    QVariant tmp;

    //Creation des services
    _pins_service=new SwPins_Manager_Class(this) ;
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);

    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);
    this->RegisterService(_pins_service);
    this->RegisterService(this);

    //--------------------------------------
    //Definition Interfaces fournis
    //--------------------------------------

    //--------------------------------------
    //Definition Interfaces consommés
    //--------------------------------------

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

    //--------------------------------------
    //Definition Pins
    //--------------------------------------
    _pinIn=new SwPin(_pins_service,QString("int"),_dataType.ToString());
    _pinIn->RegisterListener(this);
    _pins_service->RegisterPin(_pinIn);
    _pinOut=new SwPin(_pins_service,QString("out"),_dataType.ToString());
    _pinOut->RegisterListener(this);
    _pins_service->RegisterPin(_pinOut);
    //--------------------------------------
    //Definition Properties
    //--------------------------------------
    _properties_service->CreatePropertiesForQObject(this,QString(),true);
    ISwProperty * p=_properties_service->GetProperty("identifier");
    QVariant v;
    v.setValue(SwUUID::generateUUID());
    p->SetValue(v);
}

/** @brief Callback sur les changements de propriétés*/
void _RecordPoint::OnPropertyChange(ISwProperty * property) {
}

//----------------------------------------------------
// Interface ISwPin_Listener
//----------------------------------------------------
/** 
 * @brief Sur reception d'une donnée
 * @warning
 *  - Si vous gardez une reference sur la donnée reçues au dela de la portée de la methode suivante
 *utiliser un SwRefPtr sur la donnée
 *  - Si vous souhaitez modifier une donnée recue il faut d'abord en faire une copy
*/
void _RecordPoint::OnReceiveData(SwPin * src,SwData_Class * data) {
    if (src==_pinIn) {
        //Enregistrement
        data->_addRef();
        _recordQueue.push_back(data);
    } else {
        //Sinon ByPass
        _pinIn->SendData(data);
    }
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/** @brief Avant changement de la disponibilité de l'interface */
void _RecordPoint::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}
/** @brief Apres changement de la disponibilité de l'interface */
void _RecordPoint::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}
//--------------------------------------------------------------
//Properties ISwRecordPoint
//--------------------------------------------------------------
/* @brier identifiant */
SwUUID _RecordPoint::getRecordIdentifier() {
    return _identifier;
}
/* @brier name */
QString _RecordPoint::getRecordName() {
    QString goodName=SwAddress_ToolBox::BuildUniversalPath(this);
    goodName.replace(QRegExp("[\\.$]"), "_");
    return goodName;
}
/* @brief assignation du manager d'enregistrement */
void _RecordPoint::setRecordManager(ISwRecordManager * recordManager) {
    _recordManager=recordManager;
}
/* @brief construction d'une clef */
bool _RecordPoint::buildKey(QXmlStreamReader * reader) {
    SwData_Class * data=new SwData_Class();
    data->_addRef();
    data=(SwData_Class *)_codec->decode(reader,data);
    _waitingQueue.push_back(data);
    return true;
}
/* @brief soumission d'une clef pour l'emission*/
void _RecordPoint::submitKey(){
    _sendingQueue.push_back(_waitingQueue.front());
    _waitingQueue.pop_front();
}
/* @brief clean des clefs existantes*/
void _RecordPoint::cleanKeys() {
    while (!_waitingQueue.isEmpty()) {
        _waitingQueue.front()->_release();
        _waitingQueue.pop_front();
    }
    while (!_sendingQueue.isEmpty()) {
        _sendingQueue.front()->_release();
        _sendingQueue.pop_front();
    }
}

//--------------------------------------------------------------
//Properties getter and setter
//--------------------------------------------------------------
/** @brief dataType */
SwEnum _RecordPoint::getDataType() const {
    return _dataType;
}
void _RecordPoint::setDataType(const SwEnum & val) {
    if(_dataType==val) {
        return;
    }
    //Sur changement du type de données, on supprime les connecteurs
    _pins_service->UnregisterPin(_pinIn);
    delete _pinIn;
    _pins_service->UnregisterPin(_pinOut);
    delete _pinOut;
    //On affecte la valeur
    _dataType=val;
    //on reconstruit les connecteurs
    _pinIn=new SwPin(_pins_service,QString("int"),_dataType.ToString());
    _pinIn->RegisterListener(this);
    _pins_service->RegisterPin(_pinIn);
    _pinOut=new SwPin(_pins_service,QString("out"),_dataType.ToString());
    _pinOut->RegisterListener(this);
    _pins_service->RegisterPin(_pinOut);
    //On recupere le codec associé
    if (_codec!=0) {
        _codec->destroy();
    }   
    ISwServiceRecording * rservice=dynamic_cast<ISwServiceRecording *>(SW_APP->QueryService(CG_SW_SERVICE_RECORDING));
    if (rservice!=0) {
        _codec=rservice->buildCodec(_dataType.ToString());
    } else 
        _codec=0;
}
/** @brief identifier */
SwUUID _RecordPoint::getIdentifier() const{
    return _identifier;
}
void _RecordPoint::setIdentifier(const SwUUID & id) {
    ISwServiceRecording * rservice=dynamic_cast<ISwServiceRecording *>(SW_APP->QueryService(CG_SW_SERVICE_RECORDING));
    if (rservice!=0) {
        rservice->unregisterRecordPoint(this);
    }
    _identifier=id;
    if (rservice!=0) {
        rservice->registerRecordPoint(this);
    }
}
//---------------------------------------------------------------------
// Interface ISwExecutable_Service
//---------------------------------------------------------------------
/*! \brief Initialisation */
void _RecordPoint::Initialize(double start_time,ISwExecution_Service * executor) throw (SwException) {
    _currentTime=start_time;
    _recordQueue.clear();
}
/*! \brief Demarrage */
void _RecordPoint::Start(double current_time) throw (SwException){
    _currentTime=current_time;

}           
/*! \brief Execution */
void _RecordPoint::Execute(double current_time,bool is_first_call) throw (SwException){
    _currentTime=current_time;
    QXmlStreamWriter *writer=0;

    //Si enregistrement en cours
    if (_recordManager!=0 && _recordQueue.count()>0) {
        writer=_recordManager->queryRecordKey(this,current_time);
    }

    while (!_recordQueue.isEmpty()) {
        SwData_Class * data=_recordQueue.front();
        _recordQueue.pop_front();
        //Si enregistrement en cours
        if (writer!=0) {
            _codec->encode(writer,(void *)data);
        }
        //Envoie
        _pinOut->SendData(data);
        //Liberation
        data->_release();
    }
    if (writer!=0) {
        _recordManager->finalizeRecordKey();
    }

    //Si données de rejeu a emettre
    while (!_sendingQueue.isEmpty()) {
        SwData_Class * data=_sendingQueue.front();
        _sendingQueue.pop_front();
        //Envoie
        _pinOut->SendData(data);
        //Liberation
        data->_release();
    }
}            
/*! \brief Execution */
void _RecordPoint::Stop(double current_time){
    _currentTime=current_time;

}         
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _RecordPoint::Liberate(){
    //rien a faire
}  
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * _RecordPoint::GetHostComponent(){
    return this;
}            
