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
#include <SwComponent_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include "ISwRecordManager.h"
#include <ISwExecutable_Service.h>
#include "SwRecordConstantes.h"
#include "_SwServiceRecording.h"
#include <SwFileDescriptor.h>
#include <ISwWidget.h>
#include "_RecordWidget.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
using namespace StreamWork::SwExecution;
using namespace StreamWork::SwRecord;

/**
 *	@class _RecordManager
 *	@brief Record Manager
 */
class _RecordManager : public SwComponent_Class,
    public ISwInterfaces_ConsumerObserver,
    public ISwRecordManager,
    public ISwExecutable_Service,
    public ISwWidget
{
    Q_OBJECT
    Q_PROPERTY(StreamWork::SwCore::SwFileDescriptor recordDirectory READ getRecordDirectory WRITE setRecordDirectory)
    Q_PROPERTY(bool record READ getEnableRecording WRITE setEnableRecording)
    Q_PROPERTY(int maxRecordSize READ getMaxRecordSize WRITE setMaxRecordSize)
    Q_PROPERTY(StreamWork::SwCore::SwFileDescriptor recordConfiguration READ getRecordConfiguration WRITE setRecordConfiguration)
    Q_PROPERTY(bool enableInternalWidget READ getEnableInternalWidget WRITE setEnableInternalWidget)
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

 	//--------------------------------------------------------------
	//Properties
	//--------------------------------------------------------------

 	//--------------------------------------------------------------
	//Handle interfaces
	//--------------------------------------------------------------

 	//--------------------------------------------------------------
	//Pins
	//--------------------------------------------------------------
    
    /* @brief service d'enregistrement */
    _SwServiceRecording *_serviceRecord;
    /* @brief repertoire d'enregistrement */
    SwFileDescriptor _repository;
    /* @brief configuration */
    SwFileDescriptor _configuration;
    /* @brief activation de l'enregistrement */
    bool _enableRecording;
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
    /* @brief mapping record_point id */
    QMap<ISwRecordPoint *,int> _recordPointMapping;
    /* @brief taille max du fichier d'enregistrement */
    int _maxRecordSize;
    /* @brief widget info */
    _RecordWidget * _widget;
    /* @brief size */
    int _totalSize;
    /* @brief listeners */
    QList<ISwRecordManagerListener *> _listeners;
    /* @brief disable internal widget */
    bool _enabledInternalWidget;
    /* @brief configuration saver */
    StreamWork::SwConfiguration::ISwConfigurationSaver * _configSaver;
public:
    /** @brief Constructeur */
    _RecordManager(_SwServiceRecording * serviceRecord);
    /** @brief Destructeur */
    virtual ~_RecordManager();
    /** 
     * @brief Initialisation des ressources
     * @note tous les services du composants doivent être déclarés dans cette methodes
     */
    virtual void InitializeResources() throw(SwException);
    /** @brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
	/** @brief Avant changement de la disponibilité de l'interface */
	virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);
	/** @brief Apres changement de la disponibilité de l'interface */
	virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);
    //---------------------------------------------------------------------
    // Interface ISwRecordManager
    //---------------------------------------------------------------------
    /*@brief definition du repertoire d'enregistrement */
    virtual void setRecordDirectory(QString directoryName);
    /*@brief definition du fichier de configuration */
    virtual void setConfigurationFile(QString fileName);
    /*@brief definition de l outil de sauvegarde du fichier de configuration */
    virtual void setConfigurationSaver(StreamWork::SwConfiguration::ISwConfigurationSaver * configSaver);
    /*@brief demarrage de l'enregistrement */
    virtual void startRecording();
    /*@brief arret de l'enregistrement */
    virtual void stopRecording();
    /*@brief demande de creation de clef d'enregistrement*/
    virtual QXmlStreamWriter *queryRecordKey(ISwRecordPoint * recordPoint,double currentTime);
    /*@brief finalisation de la clef d'enregistrement*/
    virtual void finalizeRecordKey();
    /*@brief ajout de listener*/
    virtual void addRecordManagerListener(ISwRecordManagerListener * listener);
    /*@brief suppression de listener*/
    virtual void removeRecordManagerListener(ISwRecordManagerListener * listener);
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
    //---------------------------------------------------------------------
    // Interface ISwWidget
    //---------------------------------------------------------------------
    /*! \brief Renvoie le widget
    \return le widget */
	virtual QWidget & GetWidget();
 	//--------------------------------------------------------------
	//Properties getter and setter
	//--------------------------------------------------------------
    /** @brief recordDirectory */
    SwFileDescriptor getRecordDirectory() const;
    void setRecordDirectory(const SwFileDescriptor & val);
    /** @brief record */
    bool getEnableRecording() const;
    void setEnableRecording(bool val);
    /** @brief recordMaxSize */
    int getMaxRecordSize() const;
    void setMaxRecordSize(int val);
    /** @brief recordConfiguration */
    SwFileDescriptor getRecordConfiguration() const;
    void setRecordConfiguration(const SwFileDescriptor & val);
    /** @brief enable internal widget */
    bool getEnableInternalWidget() const;
    void setEnableInternalWidget(bool val);

protected:
    /** @brief  Construction et enregistrement du mapping des points d'enregistrement */
    void buildRecordPointMapping();
    /** @brief  creation d'un writer pour les données*/
    void createWriterData();
    /** @brief  fermetude du writer pour les données */
    void closeWriterData();
    /** @brief  sauvegarde de la configuration */
    void saveConfiguration();
    
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
