/**
 * @file TestComponentEmitter.h
 * @brief NoDescription
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */

#ifndef _TestComponentEmitter_H
#define _TestComponentEmitter_H

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
#include <ISwExecutable_Service.h>



//Check namespace needed (for exemple ISwAction need namespace StreamWork::SwGui)
using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/**
 *	@class TestComponentEmitter
 *	@brief NoDescription
 */
class TestComponentEmitter : public SwComponent_Class,
    public ISwPin_Listener,
    public ISwInterfaces_ConsumerObserver,
    public ISwExecutable_Service
{
protected:

	//--------------------------------------------------------------
	//Services
	//--------------------------------------------------------------
	/** @brief service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /** @brief service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /** @brief service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /** @brief service de gestion des pins */
    SwPins_Manager_Class * _pins_service;

 	//--------------------------------------------------------------
	//Properties
	//--------------------------------------------------------------

 	//--------------------------------------------------------------
	//Handle interfaces
	//--------------------------------------------------------------

 	//--------------------------------------------------------------
	//Pins
	//--------------------------------------------------------------
    SwPin * _pin;


    int _count;
public:
    /** @brief Constructeur */
    TestComponentEmitter();
    /** @brief Destructeur */
    virtual ~TestComponentEmitter();
    /** 
     * @brief Initialisation des ressources
     * @note tous les services du composants doivent être déclarés dans cette methodes
     */
    virtual void InitializeResources() throw(SwException);
    /** @brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    //----------------------------------------------------
    // Interface ISwPin_Listener
    //----------------------------------------------------
    /** @brief Callback sur les changements de propriétés*/
	/** @brief Sur reception d'une donnée*/
	void OnReceiveData(SwPin * src,SwData_Class * data);
     //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
	/** @brief Avant changement de la disponibilité de l'interface */
	virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);
	/** @brief Apres changement de la disponibilité de l'interface */
	virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);
    //----------------------------------------------------
    // Interface ISwExecutable_Service
    //----------------------------------------------------
	/*! \brief Initialisation */
	void Initialize(double start_time,ISwExecution_Service * executor) throw (SwException);            
	/*! \brief Demarrage */
	void Start(double current_time) throw (SwException);            
	/*! \brief Execution */
	void Execute(double current_time,bool is_first_call) throw (SwException);            
	/*! \brief Execution */
	void Stop(double current_time);            
	//---------------------------------------------------------------------
	// Interface ISwService
	//---------------------------------------------------------------------            
	/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
	lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
	void Liberate();  
	//---------------------------------------------------------------------
	// Interface ISwHost
	//---------------------------------------------------------------------
	/*! \brief acces a son composant hote */
	SwComponent_Class * GetHostComponent();            
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
