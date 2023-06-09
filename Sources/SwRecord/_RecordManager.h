/**
 * @file _RecordManager.h
 * @brief Record Manager
 * @version 1.0
 * @date Tue Apr 28 15:55:16 CEST 2009
 * @author F.Bighelli
 */

#ifndef __RecordManager_H
#define __RecordManager_H

/*
  * INCLUDES LOCAUX
  */
#include <QDomDocument>
#include <QDomElement>
#include <SwEnum.h>
#include "ISwRecordManager.h"
#include "SwRecordConstantes.h"
#include "_SwServiceRecording.h"
#include <SwFileDescriptor.h>
#include "SwAssistedComponent.h"

using namespace StreamWork::SwRecord;
using namespace StreamWork::SwFoundation;

/**
 *	@class _RecordManager
 *	@brief Record Manager
 */
class _RecordManager : public SwAssistedComponent,
    virtual public ISwRecordManager
{
    Q_OBJECT
    Q_PROPERTY(StreamWork::SwCore::SwFileDescriptor recordDirectory READ getRecordDirectory WRITE setRecordDirectory)
    Q_PROPERTY(int maxRecordSize READ getMaxRecordSize WRITE setMaxRecordSize)

protected:

    
    /* @brief service d'enregistrement */
    _SwServiceRecording *_serviceRecord;
    /* @brief repertoire d'enregistrement */
    SwFileDescriptor _repository;
	/* @brief path vers le rep de tous les enregistrements */
	QString _mainDir;
    /* @brief activation de l'enregistrement */
    bool _isRecording;
    /* @brief writer */
    QXmlStreamWriter * _writer;
    /* @brief file writer */
    QFile * _fileWriter;
    /* @brief writer data*/
    QXmlStreamWriter * _writerData;
    /* @brief file writer data*/
    QFile * _fileWriterData;
    /* @brief data counter*/
    int _dataCounter;
    /* @brief initial time */
    double _itime;
	/* @brief current time */
	double _currentTime;

	QStringList _recordsDir;

    /* @brief mapping record_point id */
    QMap<ISwRecordPoint *,int> _recordPointMapping;
    /* @brief taille max du fichier d'enregistrement */
    int _maxRecordSize;
    /* @brief size */
    int _totalSize;
    /* @brief listeners */
    QList<ISwRecordManagerListener *> _listeners;


public:
    /**
     * @brief    : Constructeur
     * @param	 : _SwServiceRecording * serviceRecord - Pointeur sur le service de recording
     */
    _RecordManager(_SwServiceRecording * serviceRecord);
    
	/**
     * @brief    : Destructeur
     */
    virtual ~_RecordManager();

	 /**
     * @brief    : Initialisation du composant
     * @note	 : A surcharger
     */
    virtual void initializeComponent() throw(SwException);

    
    //---------------------------------------------------------------------
    // Interface ISwRecordManager
    //---------------------------------------------------------------------
    /**
     * @brief    : Definition du repertoire d'enregistrement
     * @param	 : QString directoryName - Path du répertoire
     */
    virtual void setRecordDirectory(QString directoryName);
	/**
	 * @brief    : Definition du repertoire contenant tout les enregistrements
	 * @param	 : QString directoryName - Path du répertoire
	 */
	virtual void setMainDir(QString directoryName);
    /**
     * @brief    : Démarre l'enregistrement
     */
    virtual void startRecording();

    /**
     * @brief    : Arret de l'enregistrement
     */
    virtual void stopRecording();

    /**
     * @brief    : Demande de creation de clef d'enregistrement
     * @return   : QXmlStreamWriter * - Permet d'écrire dans ce writer
     * @param	 : ISwRecordPoint * recordPoint - Pointeru vers le recordPoint qui veut la clef
     * @param	 : double currentTime - Le temps courant au moment de la demande
     */
    virtual QXmlStreamWriter *queryRecordKey(ISwRecordPoint * recordPoint,double currentTime);

    /**
     * @brief    : Finalisation de la clef d'enregistrement
     * @return   : void
     */
    virtual void finalizeRecordKey();

	/**
     * @brief    : Demande de creation de clef d'enregistrement pour les propriétés
     * @return   : QXmlStreamWriter * - Permet d'écrire dans ce writer
     * @param	 : ISwRecordPoint * recordPoint - Pointeur vers le recordPoint qui veut la clef
     * @param	 : double currentTime - Le temps courant au moment de la demande
     */
    virtual QXmlStreamWriter *queryPropertyKey(ISwRecordPoint * recordPoint,double currentTime);

    /**
     * @brief    : Finalisation de la clef d'enregistrement
     * @return   : void
     */
    virtual void finalizePropertyKey();

    /**
     * @brief    : Ajout de listener
     * @param	 : ISwRecordManagerListener * listener - Pointeur sur un listener
     */
    virtual void addRecordManagerListener(ISwRecordManagerListener * listener);

    /**
     * @brief    : Suppression de listener
     * @param	 : ISwRecordManagerListener * listener -  Pointeur sur un listener
     */
    virtual void removeRecordManagerListener(ISwRecordManagerListener * listener);

	/**
	 * @brief    : Permet de recupérer l'état du recordManager
	 * @return   : bool true si l'enregistrement est en cours
	 */
	virtual bool isRecording();
   
	//----------------------------------------------------
	// Interface ISwExecutable_Service
	//----------------------------------------------------
	
	/**
	 * @brief    : Initialisation du composant executable
	 * @param	 : double start_time - le temps de début
	 * @param	 : ISwExecution_Service * executor - Pointeur sur le service d'exécution
	 */
	virtual void Initialize(double start_time, StreamWork::SwExecution::ISwExecution_Service * executor) throw (SwException);   

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

	//--------------------------------------------------------------
	//Properties getter and setter
	//--------------------------------------------------------------
    
	/** @brief recordDirectory */
    SwFileDescriptor getRecordDirectory() const;
    void setRecordDirectory(const SwFileDescriptor & val);

    /** @brief record */
    bool getRecordingState() const;
    void setRecordingState(bool val);

    /** @brief recordMaxSize */
    int getMaxRecordSize() const;
    void setMaxRecordSize(int val);


protected:

    /** @brief  Construction et enregistrement du mapping des points d'enregistrement */
    void buildRecordPointMapping();
    /** @brief  creation d'un writer pour les données*/
    void createWriterData();
    /** @brief  fermetude du writer pour les données */
    void closeWriterData();
    
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
