/*!
 \file SwWizard.cpp
 \brief Component Wizard
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/


#include <SwApplication.h>
#include <SwMacros.h>
#include "SwWizard.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
SwWizard::SwWizard():SwComponent_Class() {
    _properties_service=NULL;
    _provider_service=NULL;
    _pins_service=NULL;
	_consumer_service=NULL;
	
	//RESET Interfaces
    _i_WizardEntry=NULL; /* Remplacer null par le bon handle pour les interfaces fournies */

	//RESET Pins

	//RESET Properties
	//INITIALIZER LES AUTRES ATTRIBUTS DE LA CLASSE
	
}
/*! \brief Destructeur */
SwWizard::~SwWizard() {

	//Destruction properties automatiques	
	//Desenregistrement automatiques des interfaces
	//Destruction Pins

    //Desenregistrement des services
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
    delete _wizard;
}
/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void SwWizard::InitializeResources() throw(SwException) {
    QVariant tmp;
    
    _wizard=new SwWizardFrontEnd(NULL);
    _i_WizardEntry=_wizard;
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
    
    //--------------------------------------
    //Definition Interfaces fournis
    //--------------------------------------
	_provider_service->RegisterProvidedInterface<ISwAction>("WizardEntry",_i_WizardEntry);	

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
    //--------------------------------------
    //Autres
    //--------------------------------------
}

/*! \brief Callback sur les changements de propriétés*/
void SwWizard::OnPropertyChange(ISwProperty * property) {
}

//----------------------------------------------------
// Interface ISwPin_Listener
//----------------------------------------------------
/*! \brief Sur reception d'une donnée
\warning 
- Si vous gardez une reference sur la donnée reçues au dela de la portée de la methode suivante
  faire data->AddRef(); puis lorsque vous liberer la donnée faire data->Release();
- Si vous souhaitez modifier une donnée recue , cloner la d'abord (new_data=data->Clone()) et
  modifiez ensuite le clone (new clone), ne pas oublier de liberer le clone lorsque celui n'est plus utilisé (new_data->Release())
*/
void SwWizard::OnReceiveData(SwPin * src,SwData_Class * data) {    
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void SwWizard::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}
/*! \brief Apres changement de la disponibilité de l'interface */
void SwWizard::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}

