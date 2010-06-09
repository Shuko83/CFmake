/*!
 \file _SwGuiCompToPropertiesModel.cpp
 \brief Implementation of the Class _SwGuiCompToPropertiesModel generant un model QT a partir des propriétés d'un composant fournit
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QObject>
#include <SwApplication.h>
#include <SwMacros.h>
#include <QStandardItem>
#include "_SwGuiCompToPropertiesModel.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*! \brief Constructeur */
_SwGuiCompToPropertiesModel::_SwGuiCompToPropertiesModel(): SwComponent_Class(){

    _provider_service=NULL;
	_consumer_service=NULL;
    _properties_service=NULL;
    _provided_component=NULL;
	_handle=NULL;
    _model=NULL;
}
/*! \brief Destructeur */
_SwGuiCompToPropertiesModel::~_SwGuiCompToPropertiesModel(){
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _model;
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompToPropertiesModel::InitializeResources() throw(SwException) {
    //Creation des service
    _properties_service=new SwProperties_Class(this);
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);
    _model=new _SwPropertiesModelImpl(NULL);

    //Exportation de l'interface QAbstractItemModel
    _provider_service->RegisterProvidedInterface<QAbstractItemModel>("Model",(QAbstractItemModel *)_model);
    //Exportation de l'interface ISwQAbstractItemViewSlots
    _provider_service->RegisterProvidedInterface<ISwQAbstractItemViewSlots>("ViewSlots",(ISwQAbstractItemViewSlots *)_model);

    //Importation de l'interface ISwInterfaceProvider
    _consumer_service->RegisterConsumedInterface<ISwComponentProvider>("ComponentProvider",&_handle);
 
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

	//Enregistrement des propriétés
    //PAS de propriétés

    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiTreeView done\n"));

}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompToPropertiesModel::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    if (_handle!=NULL && interface_name==QString("ComponentProvider")) {
        _handle->OnProvidedComponentChange.idisconnect(*this,&_SwGuiCompToPropertiesModel::OnProvidedComponentChange);
        _model->SetProperties(NULL,QString());
        _provider_service->SetInterfaceUnavailable("Model");
        if (_provided_component!=NULL) {
            _provided_component=NULL;
        }
    }
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompToPropertiesModel::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
	if (_handle!=NULL && interface_name==QString("ComponentProvider")) {
        _handle->OnProvidedComponentChange.iconnect(*this,&_SwGuiCompToPropertiesModel::OnProvidedComponentChange);
        OnProvidedComponentChange(_handle);
    }
}
//---------------------------------------------------------------------
// Slot de changement de composant fournit
//---------------------------------------------------------------------
void _SwGuiCompToPropertiesModel::OnProvidedComponentChange(ISwComponentProvider * provider) {
	SwComponent_ClassPtr component;
	ISwProperties * properties;

    if (_provided_component!=NULL) {
        _model->SetProperties(NULL,QString());
        _provider_service->SetInterfaceUnavailable("Model");
        _provided_component=NULL;
    }
    component=_handle->ProvideComponent();
    if (component==NULL) {
        return;
    }
    _provided_component=component;
    properties=dynamic_cast<ISwProperties *>(_provided_component->QueryService(CG_SW_SERVICE_PROPERTIES));
    if (properties!=NULL) {
        _model->SetProperties(properties,_provided_component->GetName());
        _provider_service->SetInterfaceAvailable("Model");
    }
}
