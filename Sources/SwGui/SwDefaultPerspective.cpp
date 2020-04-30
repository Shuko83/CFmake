/**
@file SwDefaultPerspective.cpp
@brief Perspective par default
@author F.Bighelli
 */

#include "SwDefaultPerspective.h"
 
using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/** @brief Constructor */
SwDefaultPerspective::SwDefaultPerspective() {
    _provider_service=0;
    _consumer_service=0;
    _properties_service=0;
    _visible=true;
}
/*! \brief Destructeur */
SwDefaultPerspective::~SwDefaultPerspective() {
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;

}
/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void SwDefaultPerspective::InitializeResources() throw(SwException) {
    //Creation des service
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    //Creation de l'interface principale
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    //Exportation de l'interface QWidget
    _provider_service->RegisterProvidedInterface<ISwPerspective>("Perspective",(ISwPerspective *)this);

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

    _properties_service->CreatePropertiesForQObject(this,"Perspective",true);
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void SwDefaultPerspective::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {

}            
/*! \brief Apres changement de la disponibilité de l'interface */
void SwDefaultPerspective::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {

}            
//---------------------------------------------------------------------
// Interface ISwPerspective
//---------------------------------------------------------------------
/** @brief acces icon */
QString SwDefaultPerspective::getPerspectiveIcon() {
    return _iconPath;
}
/** @brief acces texte */
QString SwDefaultPerspective::getPerspectiveLabel() {
    return _label;
}
/** @brief acces description */
QString SwDefaultPerspective::getPerspectiveDescription() {
    return _description;
}
/** @brief Construction de la vue */
void SwDefaultPerspective::buildPerspectiveView(QWidget * widget) {

}
/** @brief Destruction de la vue */
void SwDefaultPerspective::destroyPerspectiveView(QWidget * widget) {

}
/** @brief Permet de savoir si la persective est active */
bool SwDefaultPerspective::isPerspectiveEnabled() {
    return false;
}
/** @brief Enregistrement de l'observer */
void SwDefaultPerspective::registerISwPerspectiveObserver(ISwPerspectiveObserver *observer) {
    _pObservers.push_back(observer);
}
/** @brief Desregistrement de l'observer */
void SwDefaultPerspective::unregisterISwPerspectiveObserver(ISwPerspectiveObserver *observer) {
    _pObservers.removeOne(observer);
}
/** @brief sur changement de l'activation */
void SwDefaultPerspective::OnEnablePerspectiveChange() {
    for(int i=0;i<_pObservers.count();i++) {
        _pObservers[i]->OnEnablePerspectiveChange(this);
    }
}
/** @brief Permet de savoir si la persective est visible */
bool SwDefaultPerspective::isPerspectiveVisible() {
    return _visible;
}
/** @brief Permet de savoir si la persective est visible */
void SwDefaultPerspective::setPerspectiveVisible(bool visibility) {
    if(_visible==visibility)
        return;
    _visible=visibility;
    for(int i=0;i<_pObservers.count();i++) {
        _pObservers[i]->OnVisiblePerspectiveChange(this);
    }
}
//---------------------------------------------------------------------
// Interface getter and setter
//---------------------------------------------------------------------
/** @brief iconPath getter*/
QString SwDefaultPerspective::getIconPath() {
    return _iconPath;
}
/** @brief iconPath getter*/
void SwDefaultPerspective::setIconPath(QString value){
    _iconPath=value;
}
/** @brief label getter*/
QString SwDefaultPerspective::getLabel(){
    return _label;
}
/** @brief label getter*/
void SwDefaultPerspective::setLabel(QString value){
    _label=value;
}
/** @brief label getter*/
QString SwDefaultPerspective::getDescription(){
    return _description;
}
/** @brief description getter*/
void SwDefaultPerspective::setDescription(QString value){
    _description=value;
}

