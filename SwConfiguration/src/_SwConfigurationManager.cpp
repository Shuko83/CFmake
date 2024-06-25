/*!
 \file _SwConfigurationManager.cpp
 \brief Implementation of the Class _SwConfigurationManager 
 \version 1.0
 \date 23-août-2006 18:59:26
 \author G.BLESSAS
*/
#include <QSet>
#include <QApplication>
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "_SwConfigurationManager.h"
#include <QMessageBox>
#include <QFileInfo>
#include "Tools/SwTime_ToolBox.h"
using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/*! \brief Constructeur */
_SwConfigurationManager::_SwConfigurationManager(): 
    SwComponent_Class(),
    _launchPropertiesFile(SwFileDescriptor::FileRead, "XML Files (*.start)")
{
   
    QString defaultPath = SW_APP->GetApplicationDirPath() + "/" +"configuration"+"/";
    defaultPath = defaultPath + "configuration.start";
    _launchPropertiesFile.setFileName(defaultPath);
    _provider_service=0;
    _consumer_service=0;
    _properties_service=0;
    _configurationsCount=0;
	_configurations_count_property = 0;
    _managedConfigurations = 0;
    _launchManager = 0;
    _isStarted = false;

//   _iReplayInterface = 0;
//    _iRecordInterface = 0;
    _mainWidget=new _SwConfigurationManagerGui(0);
    _actionStart = new _SwControllerActionStart();
    _actionStop = new _SwControllerActionStop();
    _actionRestart = new _SwControllerActionRestart();
    _actionSave = new _SwControllerActionSave();
    _actionExportConfig = new _SwControllerActionExportConfig();
    _actionLoadConfig = new _SwControllerActionLoad();

	_configurationControllerListeners.clear();
    _configurationModified = false;
    
}
/*! \brief Destructeur */
_SwConfigurationManager::~_SwConfigurationManager()
{

	_configurationControllerListeners.clear();
    //Desenregistrement des services
    if (_actionStart != 0)
        _actionStart->setController(0);
    if (_actionStop != 0)
        _actionStop->setController(0);
    if (_actionRestart != 0)
        _actionRestart->setController(0);
    if (_actionSave != 0)
    {
        _actionSave->setController(0);
        _actionSave->setLaunchManager(0);
    }
    if (_actionExportConfig != 0)
    {
        _actionExportConfig->setController(0);
        _actionExportConfig->setConfigSaver(0);
        _actionExportConfig->setLaunchManager(0);
    }
    if (_actionLoadConfig != 0)
    {
        _actionLoadConfig->setController(0);
        _actionLoadConfig->setConfigurationsList(0);
        _actionLoadConfig->setLaunchManager(0);  
    }

    if (_mainWidget != 0)
    {
        _mainWidget->setConfigurationController(0);
    }

    if (_managedConfigurations != 0) 
    {
        _managedConfigurations->setConfigurationController(0);
        _managedConfigurations->setSelectedConfiguration(0);
        _managedConfigurations->setEditor (0);
    }
    if (_launchManager != 0)
    {
        _launchManager->setEditor (0);
    }

	this->UnregisterService(_consumer_service->GetServiceName());
	this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());

    //Destruction des services
    delete _properties_service;
    delete _consumer_service;
    delete _provider_service;

    if (_mainWidget != 0)
    {
        delete _mainWidget;
        _mainWidget = 0;
    }
    if (_actionStart != 0)
    {
        delete _actionStart;
        _actionStart = 0;
    }
    if (_actionStop != 0)
    {
        delete _actionStop;
        _actionStop = 0;
    }
    if (_actionRestart != 0)
    {
        delete _actionRestart;
        _actionRestart = 0;
    }      
    if (_actionSave != 0)
    {
        delete _actionSave;
        _actionSave = 0;
    }     
    if (_actionExportConfig != 0)
    {
        delete _actionExportConfig;
        _actionExportConfig = 0;
    }
    if (_actionLoadConfig != 0)
    {
        delete _actionLoadConfig;
        _actionLoadConfig = 0;
    }
    if (_managedConfigurations != 0)
        delete _managedConfigurations;
    if (_launchManager != 0)
        delete _launchManager;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwConfigurationManager::InitializeResources() throw(SwException) {
    //Creation des service
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    // service d execution
    //this->RegisterService(this);

    _managedConfigurations = 
        new _SwManagedConfigurationsList(_provider_service,
                                         _consumer_service,
                                         _properties_service);
    _launchManager = 
        new _SwLaunchManager();

    _mainWidget->setConfigurationController(this);
    _actionStart->setController(this);
    _actionStop->setController(this);
    _actionRestart->setController(this);
    _actionSave->setController(this);
    _actionSave->setLaunchManager(_launchManager);

    _actionExportConfig->setController(this);
    _actionExportConfig->setConfigSaver(_managedConfigurations);
    _actionExportConfig->setLaunchManager(_launchManager);

    _actionLoadConfig->setController(this);
    _actionLoadConfig->setConfigurationsList(_managedConfigurations);
    _actionLoadConfig->setLaunchManager(_launchManager);

    _managedConfigurations->setEditor (_mainWidget);
    _managedConfigurations->setConfigurationController(this);
    _launchManager->setEditor (_mainWidget);

    //Exportation de l'interface QWidget
    _provider_service->RegisterProvidedInterface<QWidget>("Widget",(QWidget *) _mainWidget);

    //Exportation des interfaces d actions
    _provider_service->RegisterProvidedInterface<QAction>("StartAction", _actionStart->getAction());
    _provider_service->RegisterProvidedInterface<QAction>("StopAction", _actionStop->getAction());
    _provider_service->RegisterProvidedInterface<QAction>("RestartAction", _actionRestart->getAction());
    _provider_service->RegisterProvidedInterface<QAction>("SaveAction", _actionSave->getAction());
    _provider_service->RegisterProvidedInterface<QAction>("ExportConfiguration", _actionExportConfig->getAction());
    _provider_service->RegisterProvidedInterface<QAction>("LoadConfiguration", _actionLoadConfig->getAction());
    
    // creation de la propriété de renommage de la configuration
    _provider_service->RegisterProvidedInterface<ISwConfigurationNameProvider>(
										"ConfigurationNameProvider",
										_managedConfigurations);    
    //_consumer_service->RegisterConsumedInterface<StreamWork::SwRecord::ISwRecordManager>("IRecordManager",&_iRecordInterface);
    //_consumer_service->RegisterConsumedInterface<StreamWork::SwRecord::ISwReplayManager>("IReplayManager",&_iReplayInterface);


    //Gestion des configurations
    _configurations_count_property=_properties_service->CreateProperty<uint>("ConfigurationCOunt");
    _configurations_count_property->SetDescription("nombre de configuration gérées par le ConfigurationManager");  
    _configurations_count_property->SetValue(QVariant(_configurationsCount));
    _configurations_count_property->GetOnChangeSignal().iconnect(*this,&_SwConfigurationManager::OnPropertyChange);

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);
    _properties_service->CreatePropertyForQObject(this,"launchPropertiesFile",QString());
    _properties_service->CreatePropertiesForQObject(_mainWidget,"widget",true);
    _properties_service->CreatePropertiesForQObject(_launchManager,QString());

    //if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwConfigurationManager done\n"));

    
    SW_APP->RegisterExecutor(this);
    connect(qApp,SIGNAL(lastWindowClosed ()),this,SLOT(onQuit ()));
    
}

int _SwConfigurationManager::StreamExecute()
{
    
    if (_launchManager != 0)
    {
        _isStarted = false;
        signalControllerStartChanged();
        
        // chargement des donnees de demarrage et des parametres associes
        bool loaded = Load();
        if (loaded)
        {
            // si le chargement a fonctionné
            //if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("Application start properties loaded\n"));
            
            // par defaut on active l interface d edition de la configuration
            //_mainWidget->setEdition(true);

            // si c est le mode normal
            if (_launchManager->getStartMode().ToInt() == 0)
            {
                
                SetNormalConfiguration();
                _mainWidget->setEdition(true);
                setConfigurationEdited(false);
                
                if (_launchManager->getAutoStart())
                {
                    Start();
                }
            }
            // si c est le mode replay
            else
            {
                SetReplayConfiguration();
                setConfigurationEdited(false);
                _mainWidget->setEdition(true);
            }
        }
    }
    return 0;
}

int _SwConfigurationManager::StreamStop()
{
    return 0;
}

QString _SwConfigurationManager::getAbsoluteFilePath(QString fileName)
{
     QString res = fileName;
     QFileInfo fileInfo(fileName);
     if (!fileInfo.isAbsolute())
     {
          res = SW_APP->GetApplicationDirPath() + "/" + fileName;
     }
     return res;
}

void _SwConfigurationManager::SetReplayConfiguration()
{
    if (_launchManager->getStartMode().ToInt() == 0)
        askForSave();

     /*if (_iReplayInterface != 0)
    {

       _iReplayInterface->setRecordDirectory(getAbsoluteFilePath(_launchManager->getReplayDirectory().getFileName()));
       _managedConfigurations->LoadConfiguration(getAbsoluteFilePath(_iReplayInterface->getConfigurationFile()));
       // chargement de la configuration pour le replay
    }*/
}
void _SwConfigurationManager::SetNormalConfiguration()
{
    /*if (_iRecordInterface != 0)
    {
        // chargement des parametres d enregistrement
        _iRecordInterface->setRecordDirectory(getAbsoluteFilePath(_launchManager->getRecordDirectory().getFileName()));
       
    }*/
    bool configLoaded = LoadConfiguration();
    if (!configLoaded)
    {
        // should load config
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Configuration could not be loaded\n"));
        _launchManager->setAutoStart(false);
        _managedConfigurations->setSelectedConfiguration(0);
    }
}
/*! \brief Callback sur les changements de propriétés*/
void _SwConfigurationManager::OnPropertyChange(ISwProperty * property) 
{ 
    uint val;
    QString interface_name;

	if (property == 0)
		return ;

    if (_configurations_count_property == property) 
	{
        val=property->GetValue().toUInt();
        _managedConfigurations->setConfigurationCount(val);
	}
}

//----------------------------------------------------
// Interface ISwConfigController
//----------------------------------------------------
bool _SwConfigurationManager::LoadConfiguration()
{
    

    // chargement des parametres de demarrage
    bool configLoaded = _managedConfigurations->LoadConfiguration(getAbsoluteFilePath(_launchManager->getConfigurationFile().getFileName()));
    if (!configLoaded)
    {
         // si le chargement n a pas fonctionné
         if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Configuration properties not found\n"));
         if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Loading default values\n"));
         
         // si la propriété fichier des propriétés de configuration est ""
         if (_launchManager->getConfigurationFile().getFileName() == "")
         {
             QString defaultFilePath = SW_APP->GetApplicationDirPath() + "/" +"configuration"+"/";
             defaultFilePath = defaultFilePath + "currentConfiguration.conf";
             SwFileDescriptor filDesc(SwFileDescriptor::FileRead, "XML Files (*.conf)");
             filDesc.setFileName(defaultFilePath);
             _launchManager->setConfigurationFile(filDesc);
         }

         // si le fichier de definition des parametres de configuration n existe pas
         QFileInfo infoFile( getAbsoluteFilePath(_launchManager->getConfigurationFile().getFileName()));
         if (!infoFile.exists())
         {
            // on enregistre les parametres de configuration (creation du fichier)
            bool saved = _managedConfigurations->SaveConfiguration(getAbsoluteFilePath(_launchManager->getConfigurationFile().getFileName()));
            if (!saved)
            {
                if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Default properties file Creation Errror\n"));
                return false;
            }
         }

         // mainetenant le fichier de definition des parametres de configuration existe
         configLoaded =  _managedConfigurations->LoadConfiguration(getAbsoluteFilePath(_launchManager->getConfigurationFile().getFileName()));
         if (configLoaded)
         {
            if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("Default Configuration properties values loaded\n"));
            
         }
         else
         {
            if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Default Configuration values could not be loaded\n"));
            return false;
         }
    }
    else
    {
        //if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("Configuration properties values loaded\n"));
    }
    return configLoaded;
}

bool _SwConfigurationManager::Load()
{
    // chargement des parametres de demarrage
    bool loaded = _launchManager->LoadValues(getAbsoluteFilePath(_launchPropertiesFile.getFileName()));
    if (!loaded)
    {
         // si le chargement n a pas fonctionné
         if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Start up configuration not found\n"));
         if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Loading default values\n"));
         
         // si la propriété fichier de démarrage est ""
         if (_launchPropertiesFile.getFileName() == "")
         {
             QString defaultFilePath = SW_APP->GetApplicationDirPath() + "/" +"configuration"+"/";
             defaultFilePath = defaultFilePath + "configuration.start";
             _launchPropertiesFile.setFileName(defaultFilePath);
         }
         

         // si le fichier de definition des parametres de demarrage n existe pas
         QFileInfo infoFile(getAbsoluteFilePath(this->_launchPropertiesFile.getFileName()));
        
         if (!infoFile.exists())
         {
            // on enregistre les parametres de démarrage (creation du fichier)
            bool saved = _launchManager->SaveValues(getAbsoluteFilePath(this->_launchPropertiesFile.getFileName()));
            if (!saved)
            {
                if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Emergency,QString("Default properties file Creation Errror\n"));
                return false;
            }
         }

         // mainetenant le fichier de definition des parametres de demarrage existe
         loaded =  _launchManager->LoadValues(getAbsoluteFilePath(_launchPropertiesFile.getFileName()));
         if (loaded)
         {
            if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("Default start up values loaded\n"));
            
         }
         else
         {
            if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Default start up values could not be loaded\n"));
            return false;
         }
    }
    else
    {
       // if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("Start up values loaded\n"));
    }
    return loaded;
}

bool _SwConfigurationManager::Save()
{
    bool res = false;
    if (_launchManager!= 0 && _managedConfigurations != 0)
    {

        res = _launchManager->SaveValues(getAbsoluteFilePath(_launchPropertiesFile.getFileName()));
        // on ne sauvegarde pas la configuration si on est en replay
        if (_launchManager->getStartMode().ToInt() == 0)
        {
            res = res && SaveConfiguration();
            
        }
        setConfigurationEdited(!res);
    }
    return res;
}

bool _SwConfigurationManager::SaveConfiguration()
{

    bool saved = _managedConfigurations->SaveConfiguration(getAbsoluteFilePath(_launchManager->getConfigurationFile().getFileName()));
    if (!saved)
    {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Default properties file Creation Errror\n"));
    }
    return saved;
}

bool _SwConfigurationManager::Start()
{
    if (_managedConfigurations == 0 || _launchManager == 0)
        return false;
 
    if (_launchManager->getStartMode().ToInt() == 0)
    {
        _SwManagedConfiguration * config = _managedConfigurations->getSelectedConfiguration();
        if (config != 0 && config->getConfiguration() != 0)
        {
            //Save();

            ISwExecution_Service * execution = 0;
            execution =  config->getConfiguration()->getExecutionService();
            if (execution != 0)
            {   
                /*if (_iRecordInterface != 0 && _launchManager->getRecordActivated())
                {
                    // demarrage de l enregistrement
                    
//                    _iRecordInterface->setConfigurationFile(getAbsoluteFilePath(_launchManager->getConfigurationFile().getFileName()));
                    //_iRecordInterface->setConfigurationSaver(_managedConfigurations);
                    _iRecordInterface->setRecordDirectory(getAbsoluteFilePath(_launchManager->getRecordDirectory().getFileName()));
                    _iRecordInterface->startRecording();
                    
                }*/
                execution->StartExecution();
                _isStarted = true;
                signalControllerStartChanged();
                return true;
                
            }
        }
        else
        {
            if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Undefined configuration loaded\n"));
            _mainWidget->displayMessageBox("information","Select or load a valid configuration");
        }
    }
    else if (_launchManager->getStartMode().ToInt() == 1)
    {
        /*if (_iReplayInterface != 0)
        {

           // chargement de la configuration pour le replay
           _iReplayInterface->setRecordDirectory(getAbsoluteFilePath(_launchManager->getReplayDirectory().getFileName()));
           _managedConfigurations->LoadConfiguration(getAbsoluteFilePath(_iReplayInterface->getConfigurationFile()));

            if (!_managedConfigurations->getSelectedConfiguration() != 0 || _managedConfigurations->getSelectedConfiguration() == 0 || _managedConfigurations->getSelectedConfiguration()->getConfiguration() == 0)
            {
                _mainWidget->displayMessageBox("invalid record","configuration file of record could not be loaded");
            }
            else
            {

                _iReplayInterface->startReplay(_managedConfigurations->getSelectedConfiguration()->getConfiguration()->getExecutionService());
                _isStarted = true;
                signalControllerStartChanged();
                return true;
            }
        }*/
    }
     
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Unable to start application\n"));
        return false;
}


bool _SwConfigurationManager::Stop()
{
    if (_managedConfigurations == 0)
        return false;

    if (_launchManager->getStartMode().ToInt() == 0)
    {
        _SwManagedConfiguration * config = _managedConfigurations->getSelectedConfiguration();
        if (config != 0 && config->getConfiguration() != 0)

        {
            ISwExecution_Service * execution =  config->getConfiguration()->getExecutionService();
            
            if (execution != 0)
            {
                if (!execution->IsExecutionStopped())
                {
                    
                    execution->AskForStopExecution();
                    execution->StopExecution();
                    /*if (_iRecordInterface != 0 && _launchManager->getRecordActivated())
                    {
                        // arret de l enregistrement
                        _iRecordInterface->stopRecording();
                        
                    }*/
                    
                }
                _mainWidget->setStopped();
                _isStarted = false;
                signalControllerStartChanged();
                return true;
            }
           
        }
        else
            if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Error occured when configuration loaded\n"));
    }
    else if (_launchManager->getStartMode().ToInt() == 1)
    {
        /*if (_iReplayInterface != 0)
        {

            _iReplayInterface->stopReplay();
            _mainWidget->setStopped();
            _isStarted = false;
            signalControllerStartChanged();
            return true;
        }*/
    }
    return false;
}

//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwConfigurationManager::Liberate()
{

}  
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * _SwConfigurationManager::GetHostComponent()
{
    return this;
}        
  
void _SwConfigurationManager::askForSave()
{
    if (_configurationModified)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Configuration...");
        msgBox.setText("Do you want to save startup parameters and configuration now ?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
         
        if (ret == QMessageBox::Ok)
        {
            Save();
        }
    }
}
/*! \brief sur fin*/
void _SwConfigurationManager::onQuit() 
{

    if (_isStarted)
        Stop();
    

    disconnect(qApp,SIGNAL(lastWindowClosed ()),this,SLOT(onQuit ()));
    //disconnect(_mainWidget,SIGNAL(close()),this,SLOT(onQuit ()));

    _actionStart->setController(0);
    _actionStop->setController(0);
    _actionRestart->setController(0);
    _actionSave->setController(0);
    _actionSave->setLaunchManager(0);

    _actionExportConfig->setController(0);
    _actionExportConfig->setConfigSaver(0);
    _actionExportConfig->setLaunchManager(0);

    _actionLoadConfig->setController(0);
    _actionLoadConfig->setConfigurationsList(0);
    _actionLoadConfig->setLaunchManager(0);  

    askForSave();
    _managedConfigurations->setConfigurationController(0);
    _managedConfigurations->setSelectedConfiguration(0);

    _mainWidget->setConfigurationController(0);
    _mainWidget->setLaunchConfigurationModel(0);

    if (_managedConfigurations != 0) {
        _managedConfigurations->setEditor(0);
        _managedConfigurations->fullReset();
    }
    if (_launchManager != 0)
        _launchManager->setEditor(0);

}
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwConfigurationManager::BeforeInterfaceAvailabilityChange(
						QString interface_name,
						SwComponent_Class * provider_host)
{
    /*if (interface_name == "IRecordManager")
    {
        _iRecordInterface = 0;
    }*/
    if (interface_name == "IReplayManager")
    {
        //_iReplayInterface = 0;
    }
}

/*! \brief Apres changement de la disponibilité de l'interface */
void _SwConfigurationManager::AfterInterfaceAvailabilityChange(
						QString interface_name,
						SwComponent_Class * provider_host)
{
    /*if (interface_name == "IRecordManager")
    {
        if (_iRecordInterface != 0)
        {


        }
    }*/
    if (interface_name == "IReplayManager")
    {
        //if (_iReplayInterface != 0)
        {

        }
    }
	// sur changement d une configuration creer / maj des managed configurations
}           

//---------------------------------------------------------------------
// Interface ISwConfigurationController
//---------------------------------------------------------------------
bool _SwConfigurationManager::isStarted()
{
    return _isStarted;
}

/** @brief ajout d un listener */
void _SwConfigurationManager::addControllerListener(ISwConfigurationControllerListener * listener)
{
	if (listener != 0 && !_configurationControllerListeners.contains(listener))
		_configurationControllerListeners.append(listener);
}

/** @brief suppression d un listener */
void _SwConfigurationManager::removeControllerListener(ISwConfigurationControllerListener * listener)
{
    if (_configurationControllerListeners.contains(listener))
		_configurationControllerListeners.removeOne(listener);
}

/** @brief signale une modification de l etat du controller */
void _SwConfigurationManager::signalControllerStartChanged()
{
    QListIterator<ISwConfigurationControllerListener *> it(_configurationControllerListeners);
	it.toFront();
	while (it.hasNext())
	{
		ISwConfigurationControllerListener * listener = it.next();
		listener->OnStartedChanged(this);
	}
}
void _SwConfigurationManager::signalControllerConfigurationEditedChanged()
{
    QListIterator<ISwConfigurationControllerListener *> it(_configurationControllerListeners);
	it.toFront();
	while (it.hasNext())
	{
		ISwConfigurationControllerListener * listener = it.next();
		listener->OnConfigurationEditedChanged(this);
	}
}

SwFileDescriptor _SwConfigurationManager::getLauchPropertiesFile()
{

    return _launchPropertiesFile;
}           


void _SwConfigurationManager::setLauchPropertiesFile(SwFileDescriptor file)
{
    _launchPropertiesFile = file;

}           

void _SwConfigurationManager::setConfigurationEdited(bool value)
{
    _configurationModified = value;
    signalControllerConfigurationEditedChanged();
}
bool _SwConfigurationManager::getConfigurationEdited()
{
    return _configurationModified;
}
