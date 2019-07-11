/**
@file Component.cpp
@brief Implementation avance d'un composant par defaut
@author Big
 */

#include "ExecutableComponent.h"
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <SwPins_Manager_Class.h>
#include <ISwProperty.h>
 
using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/** @brief Constructor */
ExecutableComponent::ExecutableComponent():SwComponent_Class() {
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _pins_service = NULL;
    _disable_service=false;
}
/** @brief Destructor */
ExecutableComponent::~ExecutableComponent() {
    _disable_service=true;
    //Desenregistrement des services
    this->UnregisterService(this->GetServiceName());
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_pins_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _pins_service;
}
/*! \brief Initialisation des ressources
\note tous les services du composants doivent  tre d clar s dans cette methodes*/
void ExecutableComponent::InitializeResources() throw(SwException) {
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
    this->RegisterService(this);
    _consumer_service->AttachInterfacesConsumerObserver(this);
    initializeComponent();
}
/*! \brief surcharge du setter pour signaler le changement d'activation */
void ExecutableComponent::setActive(bool active) {
    if (isActive()==active)
        return;
    SwComponent_Class::setActive(active);
    eventActivationChanged();
}
 /*! \brief Callback sur les changements de propri t s
     \note a surcharger pour ecouter les changements de propri t s*/
void ExecutableComponent::OnPropertyChange(ISwProperty * property) {
    if (!_disable_service)
        eventPropertyChange(property);
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Callback avant changement de la disponibilit  de l'interface
    \note a surcharger si necessaire*/
void ExecutableComponent::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    if (!_disable_service)
        eventBeforeInterfaceAvailability(interface_name,provider_host);
}
/*! \brief Callback apres changement de la disponibilit  de l'interface
    \note a surcharger si necessaire*/
void ExecutableComponent::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host){
    if (!_disable_service)
        eventAfterInterfaceAvailability(interface_name,provider_host);
}            
//----------------------------------------------------
// Interface ISwPin_Listener
//----------------------------------------------------
/*! \brief Callback sur reception d'une data
    \note a surcharger pour receptionner les data*/
void ExecutableComponent::OnReceiveData(SwPin * src,SwData_Class * data){
    if (!_disable_service)
        eventReceiveData(src,data);
}           
/*! \brief Initialisation du composant
    \note A Surcharger*/
void ExecutableComponent::initializeComponent() throw(SwException) {

}
/*! \brief evenement de changement de propriete
    \note A Surcharger*/
void ExecutableComponent::eventPropertyChange(ISwProperty * property){

}
/*! \brief evenement avant changement de la disponibilit  de l'interface
    \note A Surcharger*/
void ExecutableComponent::eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host){

}
/*! \brief evenement apres changement de la disponibilit  de l'interface
    \note A Surcharger*/
void ExecutableComponent::eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host){

}
/*! \brief evenement sur reception d'une data
    \note A Surcharger*/
void ExecutableComponent::eventReceiveData(SwPin * src,SwData_Class * data){

}
/*! \brief evenement sur changement d'activation
    \note A Surcharger*/
void ExecutableComponent::eventActivationChanged(){

}
/*! \brief Acces au fournisseur d'interface*/
ISwInterfaces_Provider & ExecutableComponent::getIProviderService() {
    return *_provider_service;
}
/*! \brief Acces au service consommateur d'interface*/
ISwInterfaces_Consumer & ExecutableComponent::getIConsumerService() {
    return *_consumer_service;
}
/*! \brief Acces au service  de proprietes*/
ISwProperties & ExecutableComponent::getPropertiesService() {
    return * _properties_service;
}
/*! \brief Acces au service de connexion */
ISwPins_Manager & ExecutableComponent::getPinsService() {
    return * _pins_service;
}
/*! \brief enable listening change for property */
void ExecutableComponent::enableListeningChangeForProperty(ISwProperty * property) {
    property->GetOnChangeSignal().iconnect(*this,&ExecutableComponent::OnPropertyChange);
}
/*! \brief enable listening for pin */
void ExecutableComponent::enableListeningForPin(SwPin * pin) {
    pin->RegisterListener(this);
}
//----------------------------------------------------
// Interface ISwExecutable_Service
//----------------------------------------------------
/*! \brief Initialisation */
void ExecutableComponent::Initialize(double start_time,ISwExecution_Service * executor) throw (StreamWork::SwCore::SwException) {

}
/*! \brief Demarrage */
void ExecutableComponent::Start(double current_time) throw (StreamWork::SwCore::SwException){

}           
/*! \brief Execution */
void ExecutableComponent::Execute(double current_time,bool is_first_call) throw (StreamWork::SwCore::SwException){

}            
/*! \brief Execution */
void ExecutableComponent::Stop(double current_time){

}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void ExecutableComponent::Liberate() {
    //Nothing to do
}
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * ExecutableComponent::GetHostComponent() {
    return this;
}


