/**
 * @file _ReplayManager.h
 * @brief _ReplayManager
 * @version 1.0
 * @date Tue Apr 28 15:55:16 CEST 2009
 * @author F.Bighelli
 */

#ifndef __ReplayManager_H
#define __ReplayManager_H

/*
  * INCLUDES LOCAUX
  */
#include <SwEnum.h>
#include <SwComponent_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include "ISwReplayManager.h"
#include "_SwServiceRecording.h"
#include <SwFileDescriptor.h>
#include "_TimeLine.h"
#include "_ReplayWidget.h"
#include <ISwWidget.h>

//Check namespace needed (for exemple ISwAction need namespace StreamWork::SwGui)
using namespace StreamWork::SwCore;
using namespace StreamWork::SwRecord;
using namespace StreamWork::SwGui;

/**
 *	@class _ReplayManager
 *	@brief _ReplayManager
 */
class _ReplayManager : public SwComponent_Class,
    public ISwInterfaces_ConsumerObserver,
    public ISwReplayManager,
    public ISwWidget
{
    Q_OBJECT
    Q_PROPERTY(StreamWork::SwCore::SwFileDescriptor recordDirectory READ getRecordDirectory WRITE setRecordDirectory)
    Q_PROPERTY(bool replay READ getEnableReplay WRITE setEnableReplay)
    Q_PROPERTY(QString executor READ getExecutor WRITE setExecutor)
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

    /*@ service d'enregistrement */
    _SwServiceRecording *_serviceRecord;
    /* @brief repertoire d'enregistrement */
    SwFileDescriptor _repository;
    /* @brief activation du rejeu */
    bool _enableReplay;
    /* @brief adresse de l'executeur pour les tests */
    QString _executorPath;
    /* @brief executeur*/
    StreamWork::SwExecution::ISwExecution_Service * _executor;
    /* @brief ligne de temps*/
    _TimeLine _timeLine;
    /* @brief Widget de rejeu */
    _ReplayWidget * _widget;
public:
    /** @brief Constructeur */
    _ReplayManager(_SwServiceRecording * serviceRecord);
    /** @brief Destructeur */
    virtual ~_ReplayManager();
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
    // Interface ISwReplayManager
    //---------------------------------------------------------------------
    /*@brief definition du repertoire d'enregistrement */
    virtual void setRecordDirectory(QString directoryName);
    /*@brief acces au fichier de configuration */
    virtual QString getConfigurationFile();
    /*@brief demarrage du rejeu avec l'executeur de pilotage*/
    virtual void startReplay(StreamWork::SwExecution::ISwExecution_Service * exeService);
    /*@brief arret du rejeu */
    virtual void stopReplay();
    /*@brief ajout de listener*/
    virtual void addReplayManagerListener(ISwReplayManagerListener * listener);
    /*@brief suppression de listener*/
    virtual void removeReplayManagerListener(ISwReplayManagerListener * listener);
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
    /** @brief replay */
    bool getEnableReplay() const;
    void setEnableReplay(bool val);
    /** @brief executor */
    QString getExecutor() const;
    void setExecutor(QString val);


};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
