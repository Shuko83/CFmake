/**
 * @file TestComponent.h
 * @brief NoDescription
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */

#ifndef _TestComponent_H
#define _TestComponent_H

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
#include <SwFileDescriptor.h>



//Check namespace needed (for exemple ISwAction need namespace StreamWork::SwGui)
using namespace StreamWork::SwCore;

/**
 *	@class TestComponent
 *	@brief NoDescription
 */
class TestComponent : public SwComponent_Class,
    public ISwPin_Listener,
    public ISwInterfaces_ConsumerObserver
{
    Q_OBJECT
    Q_PROPERTY(StreamWork::SwCore::SwEnum test READ getTest WRITE setTest)
    Q_PROPERTY(QString testString READ getTestText WRITE setTestText)
    Q_PROPERTY(StreamWork::SwCore::SwFileDescriptor recordConfiguration READ getRecordConfiguration WRITE setRecordConfiguration)
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
    SwEnum _test;
    QString _testString;
    /* @brief configuration */
    SwFileDescriptor _configuration;
 	//--------------------------------------------------------------
	//Handle interfaces
	//--------------------------------------------------------------

 	//--------------------------------------------------------------
	//Pins
	//--------------------------------------------------------------

public:
    /** @brief Constructeur */
    TestComponent();
    /** @brief Destructeur */
    virtual ~TestComponent();
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

    /** @brief record */
    SwEnum getTest() const;
    void setTest(SwEnum val);
    QString getTestText() const;
    void setTestText(QString val);
    /** @brief recordConfiguration */
    SwFileDescriptor getRecordConfiguration() const;
    void setRecordConfiguration(const SwFileDescriptor & val);

};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
