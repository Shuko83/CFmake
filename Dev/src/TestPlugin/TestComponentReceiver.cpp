/**
 * @file TestComponentReceiver.cpp
 * @brief NoDescription
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */


#include <SwApplication.h>
#include <SwMacros.h>
#include "TestComponentReceiver.h"
#include "DataTest.h"

using namespace StreamWork::SwCore;

/** @brief Constructeur */
TestComponentReceiver::TestComponentReceiver():SwComponent_Class() {
    _properties_service=NULL;
    _provider_service=NULL;
    _pins_service=NULL;
	_consumer_service=NULL;

	//RESET Interfaces

	//RESET Pins

	//RESET Properties
	//INITIALIZER LES AUTRES ATTRIBUTS DE LA CLASSE

}
/** @brief Destructeur */
TestComponentReceiver::~TestComponentReceiver() {

	//Destruction properties automatiques
	//Desenregistrement automatiques des interfaces
	//Destruction Pins
    _pins_service->UnregisterPin(_pin);
    delete _pin;
    //Desenregistrement des services
    //Desenregistrement des services
    this->UnregisterService(this->GetServiceName());
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
void TestComponentReceiver::InitializeResources() throw(SwException) {
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
    //Enregistrement des services
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
    _pin=new SwPin(_pins_service,QString("in"),CG_DATA_TEST);
    _pin->RegisterListener(this);
    _pins_service->RegisterPin(_pin);
    //--------------------------------------
    //Definition Properties
    //--------------------------------------
    //--------------------------------------
    //Autres
    //--------------------------------------
}

/** @brief Callback sur les changements de propriétés*/
void TestComponentReceiver::OnPropertyChange(ISwProperty * property) {
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
void TestComponentReceiver::OnReceiveData(SwPin * src,SwData_Class * data) {
    DataTest * d=(DataTest *)data;
    QString s=QString("R(%1) at:%2  on channel %3>>%4").arg(GetName()).arg(_ltime,20,'f',3).arg(d->Channel,5).arg(d->toString());  
    qDebug(s.toLatin1().data());
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/** @brief Avant changement de la disponibilité de l'interface */
void TestComponentReceiver::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}
/** @brief Apres changement de la disponibilité de l'interface */
void TestComponentReceiver::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}
//----------------------------------------------------
// Interface ISwExecutable_Service
//----------------------------------------------------
/*! \brief Initialisation */
void TestComponentReceiver::Initialize(double start_time,ISwExecution_Service * executor) throw (SwException) {
    _ltime=start_time;
}
/*! \brief Demarrage */
void TestComponentReceiver::Start(double current_time) throw (SwException){
    _ltime=current_time;

}            
/*! \brief Execution */
void TestComponentReceiver::Execute(double current_time,bool is_first_call) throw (SwException){
    _ltime=current_time;

}            
/*! \brief Execution */
void TestComponentReceiver::Stop(double current_time){
    _ltime=current_time;

}           
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void TestComponentReceiver::Liberate() {

}
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * TestComponentReceiver::GetHostComponent() {
    return this;
}            

