/*!
 \file _SwConfiguration.cpp
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
#include "_SwConfiguration.h"
#include "_SwConfigurationSelector.h"
#include "_SwConfigurationExportedExecution.h"
#include "_SwConfigurationExportedEntity.h"
#include "_SwConfigurationExportedPerspective.h"
#include "_SwConfigurationExportedOwnerConfigurable.h"
#include "ISwPerspective.h"
#define CL_CONFIG_XML_NODE_PROPERTY "property"
#define CL_CONFIG_XML_NODE_PERSPECTIVE "iperspectivee"
#define CL_CONFIG_XML_NODE_EXE "execution"
#define CL_CONFIG_XML_NODE_CONFPERS "ownerConfigurable"
#define CL_CONFIG_XML_NODE_ATT_NAME "name"
#define CL_CONFIG_XML_NODE_ATT_EXP_NAME "ename"
#define CL_CONFIG_XML_NODE_ATT_PATH "path"
#define CL_CONFIG_XML_NODE_ATT_IDX "_idhost"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwConfiguration;

/*! \brief Constructeur */
_SwConfiguration::_SwConfiguration(): SwComponent_Class(){
    _model=NULL;
    _properties_service = 0;
    _pins_service = 0;
    _consumer_service = 0;
    _provider_service = 0;
	_activated = false;
   // _recordAvailable = true;
}
/*! \brief Destructeur */
_SwConfiguration::~_SwConfiguration(){
	    
    if (_model==this)
        DestroyBinding();

    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());    
    this->UnregisterService(_consumer_service->GetServiceName());

    //Desenregistrement des services    

    this->UnregisterService(GetServiceName());


    //Destruction des services
    //delete _pins_service;
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwConfiguration::InitializeResources() throw(SwException) {

    //Creation des services
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
   
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);
    this->RegisterService(this);

    _provider_service->RegisterProvidedInterface<ISwConfiguration>("ISwConfiguration",(ISwConfiguration *) this);
    
    //if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of _SwConfiguration done\n"));
        
    CreateBinding(this);
    //_properties_service->CreatePropertyForQObject(this,"IsRecordAvailable");


}
//----------------------------------------------------
// Interface ISwAdminSetup
//----------------------------------------------------
/*! \brief Permet d'acceder aux preferences d'administration*/
void _SwConfiguration::AdminSetup() {
    SwComponent_Class *root;
    bool isOwner=(_model==this);

    if (isOwner)
        DestroyBinding();

    if (_model!=NULL) {
        QMessageBox::warning(0,"Model host alert","You must unbind model before model host edition");
        return;
    }

    root=this;
    while (root->GetParent()!=NULL) root=root->GetParent();

    _SwConfigurationSelector * selector=new _SwConfigurationSelector(0,root,&_exported_entities);
    if (selector->exec()==QDialog::Accepted) {
        selector->ValidChange();
    }
    delete selector;
    if (isOwner)
        CreateBinding(this);

}
//----------------------------------------------------
// Gestion liaison entre model et model_host
//----------------------------------------------------
/*! \brief Mise en place du pont entre le model et le model host*/
void _SwConfiguration::CreateBinding(SwComponent_Class * model) {
    if(_model!=NULL) {
        DestroyBinding();
    }
    _model=model;
    if (_model==NULL)
        return;
    for(int i=0;i<_exported_entities.count();i++) {
        _exported_entities[i]->Bind(this,_model);    
    }
    h_index=SW_APP->GetHistoricCpt();
	signalPropertiesChanged();
    setActivated(false);

}
/*! \brief Suppression du pont entre le model et le model host*/
void _SwConfiguration::DestroyBinding(){
    if (_model==NULL)
        return;
    for(int i=0;i<_exported_entities.count();i++) {
        _exported_entities[i]->Unbind();    
    }
    if (_model!=this) {
        _model=NULL;
        CreateBinding(this);
        signalPropertiesChanged();
        return;
    }
    
    _model=NULL;
    signalPropertiesChanged();
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwConfiguration::Liberate() {

}
//---------------------------------------------------------------------
// Interface ISwPersistence
//---------------------------------------------------------------------
/*! \brief methode permettant de charger des donnees
\param[in] elt neoud parent
\param[in] finalizer_manager manager de finalisation
*/
void _SwConfiguration::Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager) {
    QDomElement elt_ent;
    _SwConfigurationExportedEntity *entity;

    for(elt_ent = elt.firstChildElement(); !elt_ent.isNull(); elt_ent = elt_ent.nextSiblingElement())
    {
        entity=NULL;
        if (elt_ent.nodeName()==QString(CL_CONFIG_XML_NODE_PROPERTY)) 
        {
            entity=_SwConfigurationExportedEntity::NewEntity(_SwConfigurationExportedEntity::Ent_Property);
        }
        if (elt_ent.nodeName()==QString(CL_CONFIG_XML_NODE_PERSPECTIVE)) 
        {
            entity=_SwConfigurationExportedEntity::NewEntity(_SwConfigurationExportedEntity::Ent_Perspective);
        }
        if (elt_ent.nodeName()==QString(CL_CONFIG_XML_NODE_EXE)) 
        {
            entity=_SwConfigurationExportedEntity::NewEntity(_SwConfigurationExportedEntity::Ent_Execution);
        }
        if (elt_ent.nodeName()==QString(CL_CONFIG_XML_NODE_CONFPERS)) 
        {
            entity=_SwConfigurationExportedEntity::NewEntity(_SwConfigurationExportedEntity::Ent_OwnerConfigurable);
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
        } else {
            delete entity;
        }
    }
    bool result;
    h_index=elt.attribute(CL_CONFIG_XML_NODE_ATT_IDX).toULongLong(&result);
    if (result==false)
        h_index=SW_APP->GetHistoricCpt();
    finalizer_manager.RegisterFinalization(h_index,this);
    
}
/*! \brief methode permettant de sauver des donnees
\param[in] elt neoud parent
\param[in] doc document parent
*/
void _SwConfiguration::Save(QDomElement & elt,QDomDocument &doc) {
    QDomElement elt_ent;
    
    for(int i=0;i<_exported_entities.count();i++) {
        //Creation du noeud
        switch (_exported_entities[i]->_type) {
            case _SwConfigurationExportedEntity::Ent_Property:
                elt_ent=doc.createElement(CL_CONFIG_XML_NODE_PROPERTY);
                break;
            case _SwConfigurationExportedEntity::Ent_Perspective:
                elt_ent=doc.createElement(CL_CONFIG_XML_NODE_PERSPECTIVE);
                break;
            case _SwConfigurationExportedEntity::Ent_Execution:
                elt_ent=doc.createElement(CL_CONFIG_XML_NODE_EXE);
                break;
            case _SwConfigurationExportedEntity::Ent_OwnerConfigurable:
                elt_ent=doc.createElement(CL_CONFIG_XML_NODE_CONFPERS);
                break;                
            default:
                return;
                break;
        }
        //Affectation des attributs
        elt_ent.setAttribute(CL_CONFIG_XML_NODE_ATT_NAME,_exported_entities[i]->_name);
        elt_ent.setAttribute(CL_CONFIG_XML_NODE_ATT_EXP_NAME,_exported_entities[i]->_exported_name);
        elt_ent.setAttribute(CL_CONFIG_XML_NODE_ATT_PATH,_exported_entities[i]->_host_path);
        //Attachement du neoud au parent
        elt.appendChild(elt_ent);
        elt.setAttribute(CL_CONFIG_XML_NODE_ATT_IDX,h_index);
    }
}
//---------------------------------------------------------------------
// Interface ISwFinalizer
//---------------------------------------------------------------------
/*! \brief finalize l'operation correspondant a l'index d'historique
\param[in] historic_index index d'historique
\return false si la finalisation n'a pas eu lieu et true si ok*/
bool _SwConfiguration::Finalize(quint64 historic_index)
{
    if (h_index==historic_index) {
        CreateBinding(this);
        return true;
    }
    return false;
}

void _SwConfiguration::setPerspectiveActivation(_SwConfigurationExportedPerspective * exportedModelPerspective, bool value)
{
	if (exportedModelPerspective != 0)
	{
        StreamWork::SwGui::ISwPerspective  * perspective = 0;
        perspective = reinterpret_cast <StreamWork::SwGui::ISwPerspective *> (exportedModelPerspective->_handle);
		if (value && perspective != 0)
			perspective->setPerspectiveVisible(value);/*_external_provider->SetInterfaceAvailable(perspective->_exported_name);*/
		else if (perspective != 0)
			perspective->setPerspectiveVisible(value);/*_external_provider->SetInterfaceUnavailable(perspective->_exported_name);*/
	}
}

void _SwConfiguration::setActivated(bool value)
{

	this->_activated = value;

    // on desactive toutes les perspectives
	for(int i=0;i<_exported_entities.count();i++) 
	{
		// si c 'est une interface Produite c est une perspective
		if (_exported_entities[i]->_type == _SwConfigurationExportedEntity::Ent_Perspective)
		{
			_SwConfigurationExportedPerspective * perspective = 
				dynamic_cast<_SwConfigurationExportedPerspective *> (_exported_entities[i]);
			setPerspectiveActivation(perspective,_activated);	
		}	
	}

	if (this->_activated)
	{
		signalActivated();
	}
	else
	{
		signalDisabled();
	}

}
/*void _SwConfiguration::setRecordAvailable(bool value)
{
    _recordAvailable = value;
    signalRecordAvailableChanged();
} */
void _SwConfiguration::signalPropertiesChanged()
{
	QListIterator<ISwConfigurationListener *> it(_configurationListeners);
	it.toFront();
	while (it.hasNext())
	{
		ISwConfigurationListener * listener = it.next();
		listener->OnPropertiesChanged(this);
	}
}

void _SwConfiguration::signalActivated()
{
	QListIterator<ISwConfigurationListener *> it(_configurationListeners);
	it.toFront();
	while (it.hasNext())
	{
		ISwConfigurationListener * listener = it.next();
		listener->OnActivate(this);
	}
}

/*void _SwConfiguration::signalRecordAvailableChanged()
{
	QListIterator<ISwConfigurationListener *> it(_configurationListeners);
	it.toFront();
	while (it.hasNext())
	{
		ISwConfigurationListener * listener = it.next();
		listener->OnRecordAvailableChanged(this);
	}
} */

/** @brief ajout d un listener */
void _SwConfiguration::addListener(ISwConfigurationListener * listener)
{
	if (listener != 0 && !_configurationListeners.contains(listener))
		_configurationListeners.append(listener);
}

/** @brief suppression d un listener */
void _SwConfiguration::removeListener(ISwConfigurationListener * listener)
{
	if (_configurationListeners.contains(listener))
		_configurationListeners.removeOne(listener);
}

void _SwConfiguration::signalDisabled()
{
	QListIterator<ISwConfigurationListener *> it(_configurationListeners);
	it.toFront();
	while (it.hasNext())
	{
		ISwConfigurationListener * listener = it.next();
		listener->OnDisable(this);
	}
}

bool _SwConfiguration::isActivated()
{
	return _activated;	
}

/*bool _SwConfiguration::isRecordAvailable()
{
	return _recordAvailable;	
} */

ISwProperties * _SwConfiguration::getProperties()
{

	return _properties_service;	
}

QList<StreamWork::SwCore::ISwServiceOwnerConfigurable *> _SwConfiguration::getServiceOwnerConfigurable()
{
    QList<StreamWork::SwCore::ISwServiceOwnerConfigurable *> servicesList;
    // on cherche le service d execution
    //bool found = false;
    StreamWork::SwCore::ISwServiceOwnerConfigurable * persistantService = 0;
	for(int i=0; i<_exported_entities.count();i++) 
	{
		// si c 'est une interface Produite c est une perspective
		if (_exported_entities[i]->_type == _SwConfigurationExportedEntity::Ent_OwnerConfigurable)
		{
			persistantService = 
				dynamic_cast<StreamWork::SwCore::ISwServiceOwnerConfigurable *> (_exported_entities[i]);
            if (persistantService != 0)
                servicesList.append(persistantService);
		}	
	}

	return servicesList;	 
}

StreamWork::SwExecution::ISwExecution_Service * _SwConfiguration::getExecutionService()
{
    // on cherche le service d execution
    bool found = false;
    StreamWork::SwExecution::ISwExecution_Service * execService = 0;
	for(int i=0;found == false && i<_exported_entities.count();i++) 
	{
		// si c 'est une interface Produite c est une perspective
		if (_exported_entities[i]->_type == _SwConfigurationExportedEntity::Ent_Execution)
		{
			execService = 
				dynamic_cast<StreamWork::SwExecution::ISwExecution_Service *> (_exported_entities[i]);
            found = true;
		}	
	}

	return execService;	
}
