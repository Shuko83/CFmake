/*!
 \file SwWizard.h
 \brief Component Wizard
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/

#ifndef _SwWizard_H
#define _SwWizard_H

/*
  * INCLUDES LOCAUX
  */
#include <SwEnum.h>
#include <SwComponent_Class.h>
#include <SwProperties_Class.h>
#include <SwPins_Manager_Class.h>
#include <ISwProperty.h>
#include <ISwPin_Listener.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include "SwWizardFrontEnd.h"

#include <ISwAction.h> 


//Check namespace needed (for exemple ISwAction need namespace StreamWork::SwGui)
using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
	\class SwWizard
	\brief Component Wizard
*/
class SwWizard : public SwComponent_Class, public ISwPin_Listener, public ISwInterfaces_ConsumerObserver {
protected:
	/* \brief wizard front-end */
    SwWizardFrontEnd * _wizard;
	//--------------------------------------------------------------
	//Services
	//--------------------------------------------------------------
	/* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des pins */
    SwPins_Manager_Class * _pins_service;

 	//--------------------------------------------------------------
	//Properties
	//--------------------------------------------------------------

 	//--------------------------------------------------------------
	//Handle interfaces
	//--------------------------------------------------------------
    /* Interface WizardEntry*/
	ISwAction * _i_WizardEntry;

 	//--------------------------------------------------------------
	//Pins
	//--------------------------------------------------------------

public:
    /*! \brief Constructeur */
    SwWizard();
    /*! \brief Destructeur */
    virtual ~SwWizard();
    /*! \brief Initialisation des ressources
          \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
    /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    //----------------------------------------------------
    // Interface ISwPin_Listener
    //----------------------------------------------------
    /*! \brief Callback sur les changements de propriétés*/
	/*! \brief Sur reception d'une donnée*/
	void OnReceiveData(SwPin * src,SwData_Class * data);            
     //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
	/*! \brief Avant changement de la disponibilité de l'interface */
	virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
	/*! \brief Apres changement de la disponibilité de l'interface */
	virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
