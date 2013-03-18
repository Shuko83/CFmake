/**
 * @file TestAssistedComponent.h
 * @brief NoDescription
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */

#ifndef _TestAssistedComponent_H
#define _TestAssistedComponent_H

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
#include <SwInterfaces_Consumer_Class.h>
#include <ISwServiceOwnerConfigurable.h>
#include <SwAssistedComponent.h>

//Check namespace needed (for exemple ISwAction need namespace StreamWork::SwGui)
using namespace StreamWork::SwCore;

/**
 *	@class TestAssistedComponent
 *	@brief NoDescription
 */
class TestAssistedComponent : public StreamWork::SwFoundation::SwAssistedComponent
{
    Q_OBJECT
		Q_PROPERTY(StreamWork::SwCore::SwEnum test READ getTest WRITE setTest)
		Q_PROPERTY(QString MaStringDeTest READ getMaStringDeTest WRITE setMaStringDeTest)
   /* Q_PROPERTY(QString testString READ getTestText WRITE setTestText)
    Q_PROPERTY(StreamWork::SwCore::SwFileDescriptor recordConfiguration READ getRecordConfiguration WRITE setRecordConfiguration)
    Q_PROPERTY(QColor testColor READ getTestColor WRITE setTestColor)*/
protected:

	

 	//--------------------------------------------------------------
	//Properties
	//--------------------------------------------------------------
    SwEnum _test;
	QString MaStringDeTest;
	QString getMaStringDeTest() const { return MaStringDeTest; }
	void setMaStringDeTest(QString val) { MaStringDeTest = val; }
	//     QString _testString;
//     /* @brief configuration */
//     SwFileDescriptor _configuration;

    //QColor testColor;
 	//--------------------------------------------------------------
	//Handle interfaces
	//--------------------------------------------------------------

 	//--------------------------------------------------------------
	//Pins
	//--------------------------------------------------------------

public:
    /** @brief Constructeur */
    TestAssistedComponent();
    /** @brief Destructeur */
    virtual ~TestAssistedComponent();


	/** 
     * @brief Initialisation des ressources
     * @note tous les services du composants doivent être déclarés dans cette methodes
     */
    virtual void initializeComponent() throw(SwException);
    /** @brief Callback sur les changements de propriétés*/
    //void OnPropertyChange(ISwProperty * property);
    //----------------------------------------------------
    // Interface ISwPin_Listener
    //----------------------------------------------------
    /** @brief Callback sur les changements de propriétés*/
	/** @brief Sur reception d'une donnée*/
	//void OnReceiveData(SwPin * src,SwData_Class * data);
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
	/** @brief Avant changement de la disponibilité de l'interface */
	//virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);
	/** @brief Apres changement de la disponibilité de l'interface */
	//virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);

    /** @brief record */
    SwEnum getTest() const;
    void setTest(SwEnum val);
	void interfaceAvailable(QString interfaceName);
	void interfaceUnavailable(QString interfaceName);
	/*QString getTestText() const;
    void setTestText(QString val);
    SwFileDescriptor getRecordConfiguration() const;
    void setRecordConfiguration(const SwFileDescriptor & val);
    QColor getTestColor() const;
    void setTestColor(QColor val);*/
    
	//---------------------------------------------------------------------
    // Interface ISwServiceOwnerConfigurable
    //---------------------------------------------------------------------
    /*! \brief methode permettant de charger des donnees de configuration*/
//    virtual void LoadConfiguration(QDomElement & parent);
    /*! \brief methode permettant de sauver des donnees de configuration*/
 //   virtual void SaveConfiguration(QDomElement & parent,QDomDocument & doc);

};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
