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
#include <SwPins_Manager_Class.h>
#include <SwUUID.h>
#include <SwEnum.h>
#include <SwFileDescriptor.h>
#include "ISwRecordPoint.h"
#include "ISwRecordDataCodec.h"
#include "ISwAdminSetup.h"
#include "_SwConfigurationExportedEntity.h"
#include "SwAssistedComponent.h"
#include "ISwFinalizer.h"

//Check namespace needed (for exemple ISwAction need namespace StreamWork::SwGui)
using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;
using namespace StreamWork::SwRecord;
using namespace StreamWork::SwFoundation;


/**
 * Define
 */
#define CL_CONFIG_XML_NODE_PROPERTY "property"
#define CL_CONFIG_XML_NODE_ATT_NAME "name"
#define CL_CONFIG_XML_NODE_ATT_EXP_NAME "ename"
#define CL_CONFIG_XML_NODE_ATT_PATH "path"
#define CL_CONFIG_XML_NODE_ATT_IDX "_idhost"

/**
 *	@class _RecordPoint
 *	@brief Record Point
 */
class _RecordPoint : public SwAssistedComponent,
    public StreamWork::SwRecord::ISwRecordPoint,
	virtual public ISwAdminSetup,
	virtual public ISwFinalizer
{
    Q_OBJECT
    Q_PROPERTY(StreamWork::SwCore::SwUUID identifier READ getIdentifier WRITE setIdentifier)
    Q_PROPERTY(StreamWork::SwCore::SwEnum dataType READ getDataType WRITE setDataType)
protected:

	//Pins
	//--------------------------------------------------------------
    /** @brief pin entrant */
    SwPin * _pinIn;
    /** @brief pin sortant */
    SwPin * _pinOut;


	/** @brief Finalizer index */
	quint64 h_index;

	bool _isRecording;

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

	/* Liste des entites exportes */
	QList<_SwConfigurationExportedEntity *> _exported_entities; 
public:

    /**
     * @brief    : Constructeur
     */
    _RecordPoint();

    /**
     * @brief    : Destructeur
     */
    virtual ~_RecordPoint();

	 /**
     * @brief    : Initialisation du composant
     * @note	 : A surcharger
     */
    virtual void initializeComponent() throw(SwException);

    //----------------------------------------------------
    // Interface ISwPin_Listener
    //----------------------------------------------------
               
    /**
     * @brief    : Calback sur reception d'une data
     * @param	 : SwPin * src - Pointeur sur la pin
     * @param	 : SwData_Class * data - Pointeur sur la data ? (a valider)
	 * @warning	 : Si vous gardez une reference sur la donnée reçues au dela de la portée de la methode suivante
	 *			   utiliser un SwRefPtr sur la donnée
	 *			   Si vous souhaitez modifier une donnée recue il faut d'abord en faire une copy
     */
    virtual void eventReceiveData(SwPin * src,SwData_Class * data);
 	
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

    //----------------------------------------------------
	// Interface ISwExecutable_Service
	//----------------------------------------------------
	
	/**
	 * @brief    : Initialisation du composant executable
	 * @param	 : double start_time - le temps de début
	 * @param	 : ISwExecution_Service * executor - Pointeur sur le service d'exécution
	 */
	virtual void Initialize(double start_time,ISwExecution_Service * executor) throw (SwException);   

	/**
	 * @brief    : Démarage (Premier pas d'execution)
	 * @param	 : double current_time - Temps de début
	 */
	virtual void Start(double current_time) throw (SwException);            

	/**
	 * @brief    : Boucle d'éxecution
	 * @param	 : double current_time - Temps d'éxecution
	 * @param	 : bool is_first_call - Si c'est le premier appel
	 */
	virtual void Execute(double current_time,bool is_first_call) throw (SwException);  

	/**
	 * @brief    : Methode appelé au stop
	 * @param	 : double current_time - Temps d'éxecution
	 */
	virtual void Stop(double current_time);         


	//---------------------------------------------------------------------
	// Interface ISwService
	//---------------------------------------------------------------------            
	
	/**
	 * @brief    : Methode appelé lors du "Setup" dans l'éditor
	 */
	virtual void AdminSetup();


	//----------------------------------------------------
	// Interface ISwPersistent
	//----------------------------------------------------

	/**
	* @brief    : Methode permettant de charger des donnees
	* @param	 : QDomElement & elt - Noeud parent
	* @param	 : ISwFinalizerManager & finalizer_manager - Manager de finalisation
	*/
	virtual void Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager);

	/**
	* @brief    : methode permettant de sauver des donnees
	* @param	 : QDomElement & elt - Noeud parent
	* @param	 : QDomDocument & doc - Document parent
	*/
	virtual void Save(QDomElement & elt,QDomDocument &doc);


	//----------------------------------------------------
	// Interface ISwPersistent
	//----------------------------------------------------

	/**
	 * @brief    : finalize l'operation correspondant a l'index d'historique
	 * @param	 : quint64 historic_index -  historic_index index d'historique
	 * @return   : bool false si la finalisation n'a pas eu lieu et true si ok
	 */
	virtual bool Finalize(quint64 historic_index);


	void OnComponentPropertyChange(ISwProperty * prop);

protected:

	void registerPropertiesListener();

};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
