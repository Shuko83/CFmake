/**
 * @file _RecordPoint.h
 * @brief Record Point
 * @version 1.0
 * @date Tue Apr 28 15:55:16 CEST 2009
 * @author F.Bighelli
 */

#ifndef __RecordPoint_H
#define __RecordPoint_H

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
#include <SwUUID.h>
#include <SwEnum.h>
#include <SwFileDescriptor.h>
#include <ISwExecutable_Service.h>
#include "ISwRecordPoint.h"
#include "ISwRecordDataCodec.h"

//Check namespace needed (for exemple ISwAction need namespace StreamWork::SwGui)
using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;
using namespace StreamWork::SwRecord;

/**
 *	@class _RecordPoint
 *	@brief Record Point
 */
class _RecordPoint : public StreamWork::SwCore::SwComponent_Class,
    public ISwPin_Listener,
    public ISwInterfaces_ConsumerObserver,
    public StreamWork::SwRecord::ISwRecordPoint,
    public ISwExecutable_Service
{
    Q_OBJECT
    Q_PROPERTY(StreamWork::SwCore::SwUUID identifier READ getIdentifier WRITE setIdentifier)
    Q_PROPERTY(StreamWork::SwCore::SwEnum dataType READ getDataType WRITE setDataType)
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
    /** @brief pin entrant */
    SwPin * _pinIn;
    /** @brief pin sortant */
    SwPin * _pinOut;
 	//--------------------------------------------------------------
	//Properties
	//--------------------------------------------------------------
    /* pin type */
    SwEnum _dataType;
    /** @brief identifier */
    SwUUID _identifier;
    /** @brief codec */
    ISwRecordDataCodec * _codec;
    /** @brief recordManager */
    ISwRecordManager * _recordManager;
    /** @brief temps courant */
    double _currentTime;
    /** @brief queue d'encodage */
    QLinkedList<SwData_Class *> _recordQueue;
    /** @brief queue en attente de données */
    QLinkedList<SwData_Class *> _waitingQueue;
    /** @brief queue pour emission de données */
    QLinkedList<SwData_Class *> _sendingQueue;
public:
    /** @brief Constructeur */
    _RecordPoint();
    /** @brief Destructeur */
    virtual ~_RecordPoint();
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
 	//--------------------------------------------------------------
	//Properties ISwRecordPoint
	//--------------------------------------------------------------
    /* @brier identifiant */
    virtual SwUUID getRecordIdentifier();
    /* @brier name */
    virtual QString getRecordName();
    /* @brief construction d'une clef */
    virtual bool buildKey(QXmlStreamReader * reader);
    /* @brief soumission d'une clef pour l'emission*/
    virtual void submitKey();
    /* @brief clean des clefs existantes*/
    virtual void cleanKeys();
    /* @brief assignation du manager d'enregistrement */
    virtual void setRecordManager(ISwRecordManager * recordManager);
 	//--------------------------------------------------------------
	//Properties getter and setter
	//--------------------------------------------------------------
    /** @brief dataType */
    SwEnum getDataType() const;
    void setDataType(const SwEnum & val);
    /** @brief identifier */
    SwUUID getIdentifier() const;
    void setIdentifier(const SwUUID & id);
    //---------------------------------------------------------------------
    // Interface ISwExecutable_Service
    //---------------------------------------------------------------------
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
