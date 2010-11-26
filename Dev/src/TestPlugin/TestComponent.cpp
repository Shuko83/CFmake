/**
 * @file TestComponent.cpp
 * @brief NoDescription
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */


#include <SwApplication.h>
#include <SwMacros.h>
#include "TestComponent.h"

using namespace StreamWork::SwCore;

/** @brief Constructeur */
TestComponent::TestComponent():SwComponent_Class() {
    _properties_service=NULL;
    _provider_service=NULL;
    _pins_service=NULL;
	_consumer_service=NULL;

    _test.AddKey(1,QString("Toto"));
    _test.AddKey(2,QString("Titi"));
    _test.FromInt(1);
    _testString="titi";
    _configuration=SwFileDescriptor(SwFileDescriptor::FileRead,QString(),QString());
	//RESET Interfaces

	//RESET Pins

	//RESET Properties
	//INITIALIZER LES AUTRES ATTRIBUTS DE LA CLASSE

}
/** @brief Destructeur */
TestComponent::~TestComponent() {

	//Destruction properties automatiques
	//Desenregistrement automatiques des interfaces
	//Destruction Pins

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
}
/** 
 * @brief Initialisation des ressources
 * @note tous les services du composants doivent être déclarés dans cette methodes
 */
void TestComponent::InitializeResources() throw(SwException) {
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

    //--------------------------------------
    //Definition Properties
    //--------------------------------------
    _properties_service->CreatePropertiesForQObject(this,QString(),true);
    //--------------------------------------
    //Autres
    //--------------------------------------
}

/** @brief Callback sur les changements de propriétés*/
void TestComponent::OnPropertyChange(ISwProperty * property) {
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
void TestComponent::OnReceiveData(SwPin * src,SwData_Class * data) {
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/** @brief Avant changement de la disponibilité de l'interface */
void TestComponent::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}
/** @brief Apres changement de la disponibilité de l'interface */
void TestComponent::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}
/** @brief record */
SwEnum TestComponent::getTest() const {
    return _test;
}
void TestComponent::setTest(SwEnum val) {
    _test=val;
}
QString TestComponent::getTestText() const {
    return _testString;
}
void TestComponent::setTestText(QString val) {
    if (val!="toto") {
        _testString=val;
    }
}
/** @brief recordConfiguration */
SwFileDescriptor TestComponent::getRecordConfiguration() const {
    return _configuration;
}
void TestComponent::setRecordConfiguration(const SwFileDescriptor & val) {
    _configuration=val;
}
QColor TestComponent::getTestColor() const {
    return testColor;
}
void TestComponent::setTestColor(QColor val) {
    testColor=val;
}
//---------------------------------------------------------------------
// Interface ISwServiceOwnerConfigurable
//---------------------------------------------------------------------
/*! \brief methode permettant de charger des donnees de configuration*/
void TestComponent::LoadConfiguration(QDomElement & parent) {
    QDomElement child=parent.firstChildElement("TestNode");
    if (!child.isNull() && child.hasAttribute("Name")) {
        qDebug("%s: LoadConfiguration:%s",GetName().toLatin1().data(),child.attribute("Name").toLatin1().data());
    }

}
/*! \brief methode permettant de sauver des donnees de configuration*/
void TestComponent::SaveConfiguration(QDomElement & parent,QDomDocument & doc) {
    QDomElement child=doc.createElement("TestNode");
    child.setAttribute("Name",_testString);
    parent.appendChild(child);
}


