/*!
 \file _SwModelHost_Class.cpp
 \brief Implementation d'un hote de model
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
 */
#include <QMessageBox>
#include <SwApplication.h>
#include <SwMacros.h>
#include <SwMacros.h>
#include <QUuid>
#include <QRegExp>
#include <SwAddress_ToolBox.h>
#include <QMessageBox>
#include "_SwModelHost_Class.h"
#include "_SwModel_Class.h"
#include "_SwModelHostExportSelector.h"
#include "_SwModelExportedExecutable.h"
#include "_SwModelExportedInterfaceP.h"
#include "_SwModelExportedInterfaceC.h"

#define CL_XML_NODE_PROPERTY "property"
#define CL_XML_NODE_INTERFACE_C "iconsume"
#define CL_XML_NODE_INTERFACE_P "iprovide"
#define CL_XML_NODE_PIN "pin"
#define CL_XML_NODE_EXE "executable"    
#define CL_XML_NODE_ACTIVABLE "activable"
#define CL_XML_NODE_OWNERCONFIG "ownerConfig"
#define CL_XML_NODE_ATT_NAME "name"
#define CL_XML_NODE_ATT_EXP_NAME "ename"
#define CL_XML_NODE_ATT_PATH "path"
#define CL_XML_NODE_ATT_ITYPE "itype"
#define CL_XML_NODE_ATT_IDX "_idhost"


using namespace StreamWork::SwCore;


//-----------------------------------------------------------------------
_SwModelHost_Class::_SwModelHost_Class() : SwComponent_Class()
{
	_model = NULL;
}

//-----------------------------------------------------------------------
_SwModelHost_Class::~_SwModelHost_Class()
{
	DestroyBinding();

	//Desenregistrement des services
	this->UnregisterService(_pins_service->GetServiceName());
	this->UnregisterService(_consumer_service->GetServiceName());
	this->UnregisterService(_provider_service->GetServiceName());
	this->UnregisterService(_properties_service->GetServiceName());
	this->UnregisterService(GetServiceName());
	//Destruction des services
	delete _pins_service;
	delete _consumer_service;
	delete _provider_service;
	delete _properties_service;

	//Nettoyage des entites
// 	for(int i=0;i<_exported_entities.count();i++) {
// 		delete _exported_entities[i];
// 	}
}

//-----------------------------------------------------------------------
void _SwModelHost_Class::InitializeResources() throw(SwException)
{

	//Creation des services
	_consumer_service = new SwInterfaces_Consumer_Class(this);
	_provider_service = new SwInterfaces_Provider_Class(this);
	_properties_service = new SwProperties_Class(this);
	_pins_service = new SwPins_Manager_Class(this);
	//Enregistrement des services
	this->RegisterService(this);
	this->RegisterService(_properties_service);
	this->RegisterService(_consumer_service);
	this->RegisterService(_provider_service);
	this->RegisterService(_pins_service);

	if ( SW_APP->IsVerbose() ) SW_APP->Logger().Log(LogLvl_Info, QString("InitializeResources of _SwModelHost_Class done\n"));

	BuildEntities();

}
//----------------------------------------------------
// Interface ISwAdminSetup
//----------------------------------------------------

//-----------------------------------------------------------------------
void _SwModelHost_Class::AdminSetup()
{
	SwComponent_Class *root;
	bool adminDone = false;

	root = this;
	while ( root->GetParent() != NULL ) root = root->GetParent();

	_SwModelHostExportSelector * selector = new _SwModelHostExportSelector(0, root, &_exported_entities);
	if ( selector->exec() == QDialog::Accepted )
	{
		DestroyEntities();
		selector->ValidChange();
		adminDone = true;
	}
	delete selector;
	if ( adminDone )
	{
		BuildEntities();
	}
}

//----------------------------------------------------
// Gestion liaison entre model et model_host
//----------------------------------------------------

//-----------------------------------------------------------------------
void _SwModelHost_Class::CreateBinding(SwComponent_Class * model)
{
	if ( _model != NULL )
	{
		DestroyBinding();
	}
	_model = model;
	if ( _model == NULL )
		return;
	for ( int i = 0; i < _exported_entities.count(); i++ )
	{
		_exported_entities[i]->Bind(_model);
	}
	h_index = SW_APP->GetHistoricCpt();
}

//-----------------------------------------------------------------------
void _SwModelHost_Class::DestroyBinding()
{
	if ( _model == NULL )
		return;
	for ( int i = 0; i < _exported_entities.count(); i++ )
	{
		_exported_entities[i]->Unbind();
	}
	_model = NULL;
}

//-----------------------------------------------------------------------
void _SwModelHost_Class::BuildEntities()
{
	//Si un modele est connecté on le deconnecte
	if ( _model != NULL )
	{
		DestroyBinding();
	}
	for ( int i = 0; i < _exported_entities.count(); i++ )
	{
		_exported_entities[i]->Build(this);
		if ( _exported_entities[i]->_type == Ent_InterfaceP )
		{
			QMap<QString, TargetInterface *>::iterator it = _lastProviders.find(_exported_entities[i]->_exported_name);
			if ( it != _lastProviders.end() )
			{
				_SwModelExportedInterfaceP  * p = dynamic_cast<_SwModelExportedInterfaceP *>(_exported_entities[i]);
				p->connectInterfaceTo(it.value()->provider, it.value()->name);
			}
		}
		else if ( _exported_entities[i]->_type == Ent_InterfaceC )
		{
			QMap<QString, QList<TargetInterface *>>::iterator it = _lastConsumers.find(_exported_entities[i]->_exported_name);
			if ( it != _lastConsumers.end() )
			{
				_SwModelExportedInterfaceC  * c = dynamic_cast<_SwModelExportedInterfaceC *>(_exported_entities[i]);
				QList<TargetInterface *> liste = it.value();
				QStringList consumers;
				QStringList names;
				for ( int j = 0; j < liste.count(); j++ )
				{
					TargetInterface * tif = liste[j];
					consumers.push_back(tif->provider);
					names.push_back(tif->name);
				}
				c->connectInterfaceTo(consumers, names);
			}
		}
	}
	//Nettoyage
	QMap<QString, QList<TargetInterface *>>::iterator itc = _lastConsumers.begin();
	while ( itc != _lastConsumers.end() )
	{
		QList<TargetInterface *> liste = itc.value();
		if ( liste.count() > 0 )
		{
			for ( int j = 0; j < liste.count(); j++ )
			{
				TargetInterface * tif = liste[j];
				delete tif;
			}
			liste.clear();
		}
		itc++;
	}
	_lastConsumers.clear();
	QMap<QString, TargetInterface *>::iterator it = _lastProviders.begin();
	while ( it != _lastProviders.end() )
	{
		delete it.value();
		it++;
	}
	_lastProviders.clear();

}

//-----------------------------------------------------------------------
void _SwModelHost_Class::DestroyEntities()
{
	if ( _model != NULL )
	{
		DestroyBinding();
	}

	for ( int i = 0; i < _exported_entities.count(); i++ )
	{
		_exported_entities[i]->Destroy();
		switch ( _exported_entities[i]->_type )
		{
			case Ent_InterfaceC: {
				_SwModelExportedInterfaceC * c = dynamic_cast<_SwModelExportedInterfaceC *>(_exported_entities[i]);
				if ( c != NULL )
				{
					QStringList consumers = c->getConsumerPaths();
					QStringList names = c->getInterfaceNames();
					QList<TargetInterface *> liste;
					for ( int j = 0; j < consumers.count(); j++ )
					{
						TargetInterface * tif = new TargetInterface(consumers[j], names[j]);
						liste.push_back(tif);
					}
					_lastConsumers.insert(_exported_entities[i]->_exported_name, liste);
				}
			}
								 break;
			case Ent_InterfaceP: {
				_SwModelExportedInterfaceP  * p = dynamic_cast<_SwModelExportedInterfaceP *>(_exported_entities[i]);
				if ( p != NULL )
				{
					QString provider = p->getProviderPath();
					if ( !provider.isEmpty() )
					{
						TargetInterface * tif = new TargetInterface(provider, p->getInterfaceName());
						_lastProviders.insert(_exported_entities[i]->_exported_name, tif);
					}
				}
			}
								 break;
			default:
				break;
		}
	}
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------

//-----------------------------------------------------------------------
void _SwModelHost_Class::Liberate()
{

}
//---------------------------------------------------------------------
// Interface ISwPersistence
//---------------------------------------------------------------------

//-----------------------------------------------------------------------
void _SwModelHost_Class::Load(QDomElement & elt, ISwFinalizerManager & finalizer_manager)
{
	QDomElement elt_ent;
	_SwModelExportedEntity *entity;
	DestroyEntities();
	for ( elt_ent = elt.firstChildElement(); !elt_ent.isNull(); elt_ent = elt_ent.nextSiblingElement() )
	{
		entity = NULL;
		if ( elt_ent.nodeName() == QString(CL_XML_NODE_PROPERTY) )
		{
			entity = _SwModelExportedEntity::NewEntity(Ent_Property);
		}
		if ( elt_ent.nodeName() == QString(CL_XML_NODE_INTERFACE_C) )
		{
			entity = _SwModelExportedEntity::NewEntity(Ent_InterfaceC);
		}
		if ( elt_ent.nodeName() == QString(CL_XML_NODE_INTERFACE_P) )
		{
			entity = _SwModelExportedEntity::NewEntity(Ent_InterfaceP);
		}
		if ( elt_ent.nodeName() == QString(CL_XML_NODE_PIN) )
		{
			entity = _SwModelExportedEntity::NewEntity(Ent_Pin);
		}
		if ( elt_ent.nodeName() == QString(CL_XML_NODE_EXE) )
		{
			entity = _SwModelExportedEntity::NewEntity(Ent_Executable);
		}
		if ( elt_ent.nodeName() == QString(CL_XML_NODE_OWNERCONFIG) )
		{
			entity = _SwModelExportedEntity::NewEntity(Ent_OwnerConfigurable);
		}
		if ( elt_ent.nodeName() == QString(CL_XML_NODE_ACTIVABLE) )
		{
			entity = _SwModelExportedEntity::NewEntity(Ent_Activable);
		}
		if ( entity != NULL &&
			elt_ent.hasAttribute(CL_XML_NODE_ATT_NAME) &&
			elt_ent.hasAttribute(CL_XML_NODE_ATT_EXP_NAME) &&
			(elt_ent.hasAttribute(CL_XML_NODE_ATT_PATH) ||
			elt_ent.hasAttribute(CL_XML_NODE_ATT_ITYPE)) )
		{
			entity->_name = elt_ent.attribute(CL_XML_NODE_ATT_NAME);
			entity->_exported_name = elt_ent.attribute(CL_XML_NODE_ATT_EXP_NAME);
			entity->_host_path = elt_ent.attribute(CL_XML_NODE_ATT_PATH);
			entity->_itype = elt_ent.attribute(CL_XML_NODE_ATT_ITYPE);
			_exported_entities.push_back(entity);
		}
		else
		{
			delete entity;
		}
	}
	BuildEntities();
}

//-----------------------------------------------------------------------
void _SwModelHost_Class::Save(QDomElement & elt, QDomDocument &doc)
{
	QDomElement elt_ent;

	for ( int i = 0; i < _exported_entities.count(); i++ )
	{
		//Creation du neoud
		switch ( _exported_entities[i]->_type )
		{
			case Ent_Property:
				elt_ent = doc.createElement(CL_XML_NODE_PROPERTY);
				break;
			case Ent_OwnerConfigurable:
				elt_ent = doc.createElement(CL_XML_NODE_OWNERCONFIG);
				break;
			case Ent_InterfaceC:
				elt_ent = doc.createElement(CL_XML_NODE_INTERFACE_C);
				break;
			case Ent_InterfaceP:
				elt_ent = doc.createElement(CL_XML_NODE_INTERFACE_P);
				break;
			case Ent_Pin:
				elt_ent = doc.createElement(CL_XML_NODE_PIN);
				break;
			case Ent_Executable:
				elt_ent = doc.createElement(CL_XML_NODE_EXE);
				break;
			case Ent_Activable:
				elt_ent = doc.createElement(CL_XML_NODE_ACTIVABLE);
				break;
			default:
				return;
				break;
		}
		//Affectation des attributs
		elt_ent.setAttribute(CL_XML_NODE_ATT_NAME, _exported_entities[i]->_name);
		elt_ent.setAttribute(CL_XML_NODE_ATT_EXP_NAME, _exported_entities[i]->_exported_name);
		elt_ent.setAttribute(CL_XML_NODE_ATT_PATH, _exported_entities[i]->_host_path);
		elt_ent.setAttribute(CL_XML_NODE_ATT_ITYPE, _exported_entities[i]->_itype);
		//Attachement du neoud au parent
		elt.appendChild(elt_ent);
		elt.setAttribute(CL_XML_NODE_ATT_IDX, h_index);
	}
}
//---------------------------------------------------------------------
// Interface ISwModelHostModifier
//---------------------------------------------------------------------

//-----------------------------------------------------------------------
QString _SwModelHost_Class::addProviderInterface(QString name, SwComponent_Class * component, QString itype)
{
	_SwModelExportedEntity *entity;
	entity = _SwModelExportedEntity::NewEntity(Ent_InterfaceP);
	entity->_name = name;
	entity->_exported_name = name;
	entity->_host_path = SwAddress_ToolBox::BuildAbsolutePath(component);
	entity->_itype = itype;
	_exported_entities.push_back(entity);
	return entity->_exported_name;
}

//-----------------------------------------------------------------------
QString _SwModelHost_Class::addConsumerInterface(QString name, SwComponent_Class * component, QString itype)
{
	_SwModelExportedEntity *entity;
	entity = _SwModelExportedEntity::NewEntity(Ent_InterfaceC);
	entity->_name = name;
	entity->_exported_name = name;
	entity->_host_path = SwAddress_ToolBox::BuildAbsolutePath(component);
	entity->_itype = itype;
	_exported_entities.push_back(entity);
	return entity->_exported_name;

}

//-----------------------------------------------------------------------
QString _SwModelHost_Class::addConnector(QString name, SwComponent_Class * component, QString itype)
{
	_SwModelExportedEntity *entity;
	entity = _SwModelExportedEntity::NewEntity(Ent_Pin);
	entity->_name = name;
	entity->_exported_name = name;
	entity->_host_path = SwAddress_ToolBox::BuildAbsolutePath(component);
	entity->_itype = itype;
	_exported_entities.push_back(entity);
	return entity->_exported_name;
}

//-----------------------------------------------------------------------
void _SwModelHost_Class::updateModelHost()
{
	BuildEntities();
}
