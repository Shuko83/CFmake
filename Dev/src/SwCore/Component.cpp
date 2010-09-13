/**
@file Component.cpp
@brief Implementation avance d'un composant par defaut
@author Big
 */

#include "Component.h"
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <SwPins_Manager_Class.h>
 
using namespace StreamWork::SwCore;

/** @brief Constructor */
Component::Component():SwComponent_Class() {
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _disable_service=false;
}
/** @brief Destructor */
Component::~Component() {
    _disable_service=true;
    terminateComponent();
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_pins_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
}
/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void Component::InitializeResources() throw(SwException) {
    //Creation des service
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    _pins_service=new SwPins_Manager_Class(this) ;
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_pins_service);
    this->RegisterService(_provider_service);
    
    initializeComponent();
}
 /*! \brief Callback sur les changements de propriétés
     \note a surcharger pour ecouter les changements de propriétés*/
void Component::OnPropertyChange(ISwProperty * property) {
    if (!_disable_service)
        eventPropertyChange(property);
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Callback avant changement de la disponibilité de l'interface
    \note a surcharger si necessaire*/
void Component::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    if (!_disable_service)
        eventBeforeInterfaceAvailability(interface_name,provider_host);
}
/*! \brief Callback apres changement de la disponibilité de l'interface
    \note a surcharger si necessaire*/
void Component::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host){
    if (!_disable_service)
        eventAfterInterfaceAvailability(interface_name,provider_host);
}            
//----------------------------------------------------
// Interface ISwPin_Listener
//----------------------------------------------------
/*! \brief Callback sur reception d'une data
    \note a surcharger pour receptionner les data*/
void Component::OnReceiveData(SwPin * src,SwData_Class * data){
    if (!_disable_service)
        eventReceiveData(src,data);
}           
/*! \brief Initialisation du composant
    \note A Surcharger*/
void Component::initializeComponent() throw(SwException) {

}
/*! \brief terminaison du composante
    \note A Surcharger*/
void Component::terminateComponent(){

}
/*! \brief evenement de changement de propriete
    \note A Surcharger*/
void Component::eventPropertyChange(ISwProperty * property){

}
/*! \brief evenement avant changement de la disponibilité de l'interface
    \note A Surcharger*/
void Component::eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host){

}
/*! \brief evenement apres changement de la disponibilité de l'interface
    \note A Surcharger*/
void Component::eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host){

}
/*! \brief evenement sur reception d'une data
    \note A Surcharger*/
void Component::eventReceiveData(SwPin * src,SwData_Class * data){

}
/*! \brief Acces au fournisseur d'interface*/
ISwInterfaces_Provider & Component::getIProviderService() {
    return *_provider_service;
}
/*! \brief Acces au service consommateur d'interface*/
ISwInterfaces_Consumer & Component::getIConsumerService() {
    return *_consumer_service;
}
/*! \brief Acces au service  de proprietes*/
ISwProperties & Component::getPropertiesService() {
    return * _properties_service;
}
/*! \brief Acces au service de connexion */
ISwPins_Manager & Component::getPinsService() {
    return * _pins_service;
}

