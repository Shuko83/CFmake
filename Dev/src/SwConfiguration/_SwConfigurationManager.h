/*!
 \file _SwConfigurationManager_H.h
 \brief Implementation of the Class _SwConfigurationManager realisant la fonction d'executeur
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwConfigurationManager_H
#define __SwConfigurationManager_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
/*
  * INCLUDES LOCAUX
  */
#include <SwEnum.h>
#include <SwComponent_Class.h>
#include <SwProperties_Class.h>
#include <SwPins_Manager_Class.h>
#include <ISwProperty.h>
#include <ISwPin_Listener.h>
#include <ISwAdminSetup.h>
#include <ISwExecutable_Service.h>
#include <ISwConfiguration.h>
#include <ISwInterfaces_ConsumerObserver.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <_SwManagedConfiguration.h>
#include <_SwManagedConfigurationsList.h>
#include <_SwConfigurationManagerGui.h>
#include <_SwLaunchManager.h>
#include <ISwExecutor.h>
#include <SwFileDescriptor.h>
#include <ISwConfigurationController.h>
//#include <ISwReplayManager.h>
#include <ISwRecordManager.h>
#include <ISwConfigurationControllerListener.h>
#include <_SwControllerActionStart.h>
#include <_SwControllerActionStop.h>
#include <_SwControllerActionRestart.h>
#include <_SwControllerActionExportConfig.h>
#include <_SwControllerActionSave.h>
#include <_SwControllerActionLoad.h>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;
using namespace StreamWork::SwConfiguration;   
/*!
	\class _SwConfigurationManager 
	\brief Implementation of the Class _SwConfigurationManager 
*/
class _SwConfigurationManager : 
	public SwComponent_Class, 
    public ISwInterfaces_ConsumerObserver,
    public ISwExecutor,
    public ISwConfigurationController
{
    Q_OBJECT

    Q_PROPERTY(StreamWork::SwCore::SwFileDescriptor launchPropertiesFile READ getLauchPropertiesFile WRITE setLauchPropertiesFile)
                
public:
    /*! \brief Constructeur */
    _SwConfigurationManager();
    /*! \brief Destructeur */
    virtual ~_SwConfigurationManager();

    /*! \brief Initialisation des ressources
          \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
    
    
    /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    void askForSave();

    //----------------------------------------------------
    // Interface ISwExecutor
    //----------------------------------------------------

    /*! \brief Execute le stream
    \return le resultat de l'operation */
	virtual int StreamExecute();

    //----------------------------------------------------
    // Interface ISwConfigController
    //----------------------------------------------------

    virtual void SetReplayConfiguration();
    virtual void SetNormalConfiguration();
	virtual bool Load();  
	virtual bool Start();            
	virtual bool Save();            
	virtual bool Stop();     

    virtual bool getConfigurationEdited();
    virtual void setConfigurationEdited(bool value);	

	bool LoadConfiguration();   
	bool SaveConfiguration();  

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
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
    /*! \brief Avant changement de la disponibilité de l'interface */
    virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
    /*! \brief Apres changement de la disponibilité de l'interface */
    virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
            

    SwFileDescriptor getLauchPropertiesFile();

    void setLauchPropertiesFile(SwFileDescriptor file);

	//---------------------------------------------------------------------
	// Interface ISwConfigurationController
	//---------------------------------------------------------------------
	
    virtual bool isStarted() ;

    /** @brief ajout d un listener */
    virtual void addControllerListener(ISwConfigurationControllerListener *) ;

    /** @brief suppression d un listener */
    virtual void removeControllerListener(ISwConfigurationControllerListener *) ;
              

public slots:
    
	/*! \brief sur fin*/
    void onQuit();

protected:

    QString getAbsoluteFilePath(QString fileName);

    /** @brief signale une modification de l etat du controller */
    void signalControllerStartChanged();
    /** @brief signale une modification de l etat du controller */
    void signalControllerConfigurationEditedChanged();

    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;



	// --- configurations ---
    /* nombre de configurations */
    uint _configurationsCount;

    /* propriété nombre de configurations */
    ISwProperty * _configurations_count_property;

    /* gestionnaires de configurations*/
    _SwManagedConfigurationsList * _managedConfigurations;

    /* gestionnaires de configurations*/
    _SwLaunchManager *              _launchManager;

	// --- ihm ---
    _SwConfigurationManagerGui      * _mainWidget;

    // --- actions --
    _SwControllerActionStart        * _actionStart;
    _SwControllerActionStop         * _actionStop;
    _SwControllerActionRestart      * _actionRestart;
    _SwControllerActionSave         * _actionSave;
    _SwControllerActionExportConfig * _actionExportConfig;
    _SwControllerActionLoad * _actionLoadConfig;

    // --- fichier de propriétés de chargement ---
    SwFileDescriptor                _launchPropertiesFile;


    //StreamWork::SwRecord::ISwReplayManager            *   _iReplayInterface;

    StreamWork::SwRecord::ISwRecordManager            *   _iRecordInterface;

    bool                                                  _isStarted;
    bool                                                  _configurationModified;
    QList<ISwConfigurationControllerListener *>           _configurationControllerListeners;

};
#endif 
