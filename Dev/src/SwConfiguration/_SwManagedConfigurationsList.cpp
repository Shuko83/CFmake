/*!
\file _SwManagedConfigurationsList.cpp
\date 
\brief  
\author  
\version 
 */
#define CL_CONFIGURATION_INTERFACE_NAME_PREFIX "Configuration_" 
#define CL_CONFIGURATION_INTERFACE_NAME "%1"
#define CL_CONFIGURATION_INTERFACE_NAME_PROPERTY "Configuration%1.name"
#define CL_CONFIGURATION_INTERFACE_RECORDABLE_PROPERTY "Configuration%1.recordable"

#include "_SwManagedConfigurationsList.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QDomNodeList>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <SwApplication.h>

using namespace StreamWork::SwConfiguration;
using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwManagedConfigurationsList::_SwManagedConfigurationsList()
{
    _provider_service=0;
    _consumer_service=0;
    _properties_service=0;
    _configurationsCount=0;
    _editor = 0;
    _selectedConfiguration = 0;
    _configurationNamesListeners.clear();
    _configurationController = 0;
}

void _SwManagedConfigurationsList::setConfigurationController(ISwConfigurationController * controller)
{
    _configurationController = controller;
    if (_selectedConfiguration != 0)
    {
        _selectedConfiguration->setConfigurationController(controller);
    }
}
/*! \brief Constructeur */
_SwManagedConfigurationsList::_SwManagedConfigurationsList(
                SwInterfaces_Provider_Class * provider_service,
                SwInterfaces_Consumer_Class * consumer_service,
                SwProperties_Class * properties_service)
{
    _provider_service=provider_service;
    _consumer_service=consumer_service;
    _properties_service=properties_service;
    
    if (_consumer_service != 0)
        _consumer_service->AttachInterfacesConsumerObserver(this);
       
										
    //_editor->onConfigurationListChanged();
    _configurationsCount=0;
    _selectedConfiguration = 0;
    _configurationNamesListeners.clear();
    _configurationController = 0;
}

/*! \brief Destructeur */
_SwManagedConfigurationsList::~_SwManagedConfigurationsList()
{
    
    _configurationNamesListeners.clear();
    _configurations.clear();
  	_configurationsNames.clear();
  	_configurationsRecordable.clear();
  	_configurationsNamesProperties.clear();
  	_configurationsRecordableProperties.clear();
  	QMap<int,_SwManagedConfiguration *>:: iterator it;
  	it = _managedConfigurations.begin();
  	while (it != _managedConfigurations.end())
  	{
  		_SwManagedConfiguration * managedConfig = it.value();
  		delete managedConfig;
          it++;
  	}
	 _managedConfigurations.clear();
}
void _SwManagedConfigurationsList::fullReset() {
	QMap<int,_SwManagedConfiguration *>:: iterator it;
	it = _managedConfigurations.begin();
	while (it != _managedConfigurations.end())
	{
		_SwManagedConfiguration * managedConfig = it.value();
		delete managedConfig;
        it++;
	}
	_managedConfigurations.clear();
}
    
void _SwManagedConfigurationsList::setEditor(ISwConfigurationEditor * editor)
{
    _editor = editor;
    if (_editor != 0)
    {
        _editor->setManagedConfigurationsModel(this);
        _editor->onConfigurationListChanged();
    }
}

_SwManagedConfiguration * _SwManagedConfigurationsList::getSelectedConfiguration()
{
    return _selectedConfiguration;
}

void _SwManagedConfigurationsList::setSelectedConfiguration(_SwManagedConfiguration * configuration)
{
    if (_selectedConfiguration != 0)
    {
        
        _selectedConfiguration->setEnabled(false);
        _selectedConfiguration->setConfigurationController(0);
    }

    _selectedConfiguration = configuration;

    if (_selectedConfiguration != 0)
    {
        _selectedConfiguration->setConfigurationController(_configurationController);
        _selectedConfiguration->setEnabled(true);
    }
    if (_editor != 0)
        _editor->onCurrentConfigurationChanged();
    
    notifyListNameChanged();    
}

void _SwManagedConfigurationsList::AddConfigurationNameListener (ISwConfigurationNameListener * listener)
{
    if(!_configurationNamesListeners.contains(listener) && (listener != 0)) 
    {
        _configurationNamesListeners.append(listener);
    }
}

void _SwManagedConfigurationsList::RemoveConfigurationNameListener (ISwConfigurationNameListener * listener)
{
    _configurationNamesListeners.removeOne(listener);
} 
QString _SwManagedConfigurationsList::GetSelectedName()
{
    if (_selectedConfiguration != 0)
        return _selectedConfiguration->getName();
    else
        return "Undefined";
}
void _SwManagedConfigurationsList::notifyListNameChanged() 
{
    QListIterator<ISwConfigurationNameListener *> iterator(_configurationNamesListeners);
    ISwConfigurationNameListener * listener = 0;

    while(iterator.hasNext()) 
    {
        listener = iterator.next();
        if(listener != 0) 
        {
            listener->OnNameModified();
        }
    } 
}

bool _SwManagedConfigurationsList::LoadConfiguration(QString filename)
{

        QFile file;
        QDomDocument doc;
        QString xml_error;
        int error_line,error_column;
        bool res = false;

        QString desc;
        file.setFileName(filename);
        if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            //QMessageBox::warning(NULL,"StreamWorkEditor alert",QString("Fail to open file stream %1").arg(_streamFileName));
            return false;
        }
        desc=QString(file.readAll());
        file.close();
        QFileInfo fileInfo(file);
        //Recherche du repertoire associer
        QDir repository = fileInfo.dir();
        QString subdir = fileInfo.completeBaseName()+"_files";
        if ( !repository.exists(subdir)) 
        {
            SW_ALERT.Alert(AlertLvl_Warning,"Error loading configurations additional files: dir %s does not exist",repository.dirName()+QDir::separator()+subdir);
            return false;
        }
        repository.cd(subdir);
                
        //parsing xml du document
        if (!doc.setContent(desc,&xml_error,&error_line,&error_column)) 
        {
            QString msg=QString("XML Parsing error:%1 at position %2,%3\n%4").arg(xml_error).arg(error_line).arg(error_column).arg(desc);
            QMessageBox::warning(NULL,"Loading Failed",msg);
            return false;
        }

        //A ce stade le doc est chargé
        QDomElement config = doc.firstChildElement("Configuration");
        if (config.hasAttribute("Name"))
        {
            QString configName = config.attribute("Name");
            // on recherche la configuration sauvée
            QMap<int,_SwManagedConfiguration *>:: iterator it;
	        it = _managedConfigurations.begin();
            bool found = false;
	        while (!found && it != _managedConfigurations.end())
	        {
		        _SwManagedConfiguration * managedConfig = it.value();
		        if (managedConfig != 0 && managedConfig->getName().compare(configName)==0)
                {
                    this->setSelectedConfiguration(managedConfig);
                    QDomElement configProperties = config.firstChildElement("Properties");
                    // remplacement des path de fichiers relatifs en absolus a la lecture
                    
                    processLoadConfigurationNode(&configProperties,&repository);
                    
                    _selectedConfiguration->Load(configProperties); 
                    if (_editor != 0)
                        _editor->onCurrentConfigurationChanged();
                    found = true;
                    res = true;
                }
                it++;
	        }
            
        }

    return res;
}

bool _SwManagedConfigurationsList::SaveConfiguration(QString filename)
{
    if (_selectedConfiguration != 0)
    {
        QDomDocument doc;
        QByteArray desc;
        QFile file;

        QDomElement config = doc.createElement("Configuration");
        doc.appendChild(config);
        config.setAttribute("Name",_selectedConfiguration->getName());
        QDomElement configProperties = doc.createElement("Properties");
        config.appendChild(configProperties);

        //ICI remplir le doc 
        if (_selectedConfiguration != 0)
            _selectedConfiguration->Save(configProperties,doc); 

        //Construction du repertoire associer
        QFileInfo fInfo(filename);
        QDir d=fInfo.dir();
        QString subdir=fInfo.completeBaseName()+"_files";
        if ( !d.exists(subdir) && !d.mkdir(subdir)) 
        {
            return false;
        }
        d.cd(subdir);

        //Traitement du fichier de configuration
        processSaveConfigurationNode(&(doc.documentElement()),&d);

        //Recuperation du stream
        desc=doc.toByteArray(4); //Indentation de quatre espace
        //Ouverture d'un fichier en ecriture
        file.setFileName(filename);
        if (file.open(QIODevice::WriteOnly  | QIODevice::Truncate)==false) {
            //QMessageBox::critical(NULL,"StreamWorkEditor critical",QString("Fail to save stream in file %1").arg(_streamFileName));
            return false;
        }

        //Ecriture du fichier
        file.write(desc);
        //Fermeture du fichier
        file.close();
        return true;
    }
    return false;
}

/** @brief  traitement de la configuration */
void _SwManagedConfigurationsList::processSaveConfigurationNode(QDomElement * elt,QDir * targetRepository) {
    if (elt->hasAttribute("fdesc")) {
        QFile f(elt->attribute("fdesc"));
        QFileInfo fi(elt->attribute("fdesc"));
        QString new_name=targetRepository->absolutePath()+
               QDir::separator()+fi.fileName();
        QFileInfo newfi(new_name);
        if (fi.isFile() && f.exists()) {
            if( newfi.absolutePath()!=fi.absolutePath()) {
                QFile old(new_name);
                if(old.exists()) {
                    QFile::remove(new_name);   
                }
                f.copy(new_name); 
            }
            elt->setAttribute("fdesc",/*targetRepository->dirName () + QDir::separator() + */fi.fileName());
        }
    }
    QDomElement selt=elt->firstChildElement();
    while (!selt.isNull()) {
        processSaveConfigurationNode(&selt,targetRepository);
        selt=selt.nextSiblingElement();
    }
}
/** @brief  traitement de la configuration */
void _SwManagedConfigurationsList::processLoadConfigurationNode(QDomElement * elt,QDir * targetRepository) {
    if (elt->hasAttribute("fdesc")) {
        QString fileName = elt->attribute("fdesc");
        QString new_name=targetRepository->absolutePath()+
               QDir::separator()+fileName;
        
        QFileInfo newfi(new_name);
        if (newfi.exists() && newfi.isFile()) {
            
            elt->setAttribute("fdesc",new_name);
        }
        else
        {
            SW_ALERT.Alert(AlertLvl_Warning,"Error loading configurations additional file: file %s does not exist",newfi.filePath());
        }
    }
    QDomElement selt=elt->firstChildElement();
    while (!selt.isNull()) {
        processLoadConfigurationNode(&selt,targetRepository);
        selt=selt.nextSiblingElement();
    }
}

void _SwManagedConfigurationsList::setConfigurationCount(int value)
{
        uint val=value;
        
        if (val==_configurationsCount) return;
        if (val<_configurationsCount) 
		{
            for (uint i=val;i<_configurationsCount;i++) 
			{
				removeConfigurationConsumed(i-1);  
            }
        } else 
		{
            for (uint i=_configurationsCount;i<val;i++) 
			{
                addConfigurationConsumed(i);
            }
        }
        _configurationsCount = val;  
        //_editor->setConfigurationsList(_managedConfigurations.values());
}

/*! \brief Callback sur les changements de propriétés*/
void _SwManagedConfigurationsList::OnPropertyChange(ISwProperty * property) 
{
    
	QMap<int,ISwProperty *>::iterator it;
	it = _configurationsNamesProperties.begin();

	bool found = false;
	while (!found && it != _configurationsNamesProperties.end())
	{
		if (property == it.value())
		{
			renameConfigurationConsumed(it.key(),property->GetValue().toString());
			_configurationsNames[it.key()] = property->GetValue().toString();
			
			found = true;
		}
		it++;
	}    
	QMap<int,ISwProperty *>::iterator itRecord;
	itRecord = _configurationsRecordableProperties.begin();
	while (!found && itRecord != _configurationsRecordableProperties.end())
	{
		if (property == itRecord.value())
		{
			_configurationsRecordable[itRecord.key()] = property->GetValue().toBool();
			if (_managedConfigurations[itRecord.key()] != 0 )
			   _managedConfigurations[itRecord.key()]->setRecordAvailable(_configurationsRecordable[itRecord.key()]);
			found = true;
		}
		itRecord++;
	} 
}



void _SwManagedConfigurationsList::addConfigurationConsumed( int index)
{
    if (_consumer_service != 0)
    {
		  QString configurationName =
			   QString(CL_CONFIGURATION_INTERFACE_NAME).arg(index);
		  QString configurationPropertyName =
			   QString(CL_CONFIGURATION_INTERFACE_NAME_PROPERTY).arg(index);
			QString configurationRecordableProperty =
			   QString(CL_CONFIGURATION_INTERFACE_RECORDABLE_PROPERTY).arg(index);
			   
      _configurations[index] = 0;
  		_configurationsNames[index] = configurationName;
      _configurationsRecordable[index] = true;
      
  		// nom de l'interface et de la configuration
  		_configurationsNamesProperties[index]=_properties_service->CreateProperty<QString>(configurationPropertyName);
  		_configurationsNamesProperties[index]->SetValue(QVariant(_configurationsNames[index]));
      _configurationsNamesProperties[index]->GetOnChangeSignal().iconnect(*this,&_SwManagedConfigurationsList::OnPropertyChange);
  		_configurationsNamesProperties[index]->SetDescription("nom de la configuration");  
  		
  		// activation de l'enregistrement
  		_configurationsRecordableProperties[index]=_properties_service->CreateProperty<bool>(configurationRecordableProperty);
  		_configurationsRecordableProperties[index]->SetValue(QVariant(_configurationsRecordable[index]));
      _configurationsRecordableProperties[index]->GetOnChangeSignal().iconnect(*this,&_SwManagedConfigurationsList::OnPropertyChange);
  		_configurationsRecordableProperties[index]->SetDescription("disponibilite de l enregistrement"); 
		
      // gestionnaire de configuration
      _SwManagedConfiguration * managedConfig = new _SwManagedConfiguration();
	    managedConfig->setConfiguration(_configurations[index],_configurationsNames[index]);
	    _managedConfigurations[index] = managedConfig;

  		// creation de l interface de la configuration
      _consumer_service->RegisterConsumedInterface<ISwConfiguration>(
  										CL_CONFIGURATION_INTERFACE_NAME_PREFIX + configurationName,
  										&( _configurations[index]));
  										
  		// notification de l editeur								
      if (_editor != 0)
          _editor->onConfigurationListChanged();
    }
}
void _SwManagedConfigurationsList::removeConfigurationConsumed(int index)
{
    if (_consumer_service != 0)
    {
		    QString configurationPropertyName =
			     QString(CL_CONFIGURATION_INTERFACE_NAME_PROPERTY).arg(index);
        QString configurationRecordableName =
			     QString(CL_CONFIGURATION_INTERFACE_RECORDABLE_PROPERTY).arg(index);
    		_consumer_service->UnregisterConsumedInterface(CL_CONFIGURATION_INTERFACE_NAME_PREFIX + _configurationsNames[index]);
    		_properties_service->DestroyProperty(configurationPropertyName);
    		_properties_service->DestroyProperty(configurationRecordableName);
    		_configurations.remove(index);
    		_configurationsNames.remove(index);
    		_configurationsNamesProperties.remove(index);   
    		_configurationsRecordable.remove(index);
    		_configurationsRecordableProperties.remove(index);
        if (_managedConfigurations.contains(index) &&  _managedConfigurations[index] != 0)
        {
            delete _managedConfigurations[index];
		    _managedConfigurations.remove(index);
        }
    }
    if (_editor != 0)
        _editor->onConfigurationListChanged();
}
void _SwManagedConfigurationsList::renameConfigurationConsumed( int index, QString NewName)
{
    if (NewName == _configurationsNames[index])
        return;
	if (_configurations.find (index) != _configurations.end())
	{
		_consumer_service->UnregisterConsumedInterface(CL_CONFIGURATION_INTERFACE_NAME_PREFIX + _configurationsNames[index]);
		_configurations[index] = 0;
		_configurationsNames[index] = NewName;
        _managedConfigurations[index]->setConfiguration(_configurations[index],NewName);
        _consumer_service->RegisterConsumedInterface<ISwConfiguration>(
										CL_CONFIGURATION_INTERFACE_NAME_PREFIX + NewName,
										&( _configurations[index]));
        if (_editor != 0)
            _editor->onConfigurationListChanged();
        
       
	}
}
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwManagedConfigurationsList::BeforeInterfaceAvailabilityChange(
						QString interface_name,
						SwComponent_Class * provider_host)
{
	QMap<int, QString>::iterator nameIterator;
	nameIterator = _configurationsNames.begin();
	bool found = false;
	while(!found && nameIterator != _configurationsNames.end())
	{
		QString interfaceName = CL_CONFIGURATION_INTERFACE_NAME_PREFIX+
								nameIterator.value();
		if (interfaceName.compare(interface_name) == 0)
		{

			_SwManagedConfiguration * managedConfig = _managedConfigurations[nameIterator.key()];
			if (managedConfig != 0)
			{
                managedConfig->setConfiguration(0,nameIterator.value());
			}
			found = true;
		}
		nameIterator ++;
	}
}

/*! \brief Apres changement de la disponibilité de l'interface */
void _SwManagedConfigurationsList::AfterInterfaceAvailabilityChange(
						QString interface_name,
						SwComponent_Class * provider_host)
{
	QMap<int, QString>::iterator nameIterator;
	nameIterator = _configurationsNames.begin();
	bool found = false;
	while(!found && nameIterator != _configurationsNames.end())
	{
		QString interfaceName = CL_CONFIGURATION_INTERFACE_NAME_PREFIX+
								nameIterator.value();
		if (interfaceName.compare(interface_name) == 0)
		{
			_SwManagedConfiguration * managedConfig = _managedConfigurations[nameIterator.key()];
			if (managedConfig != 0)
			{
                managedConfig->setConfiguration(_configurations[nameIterator.key()],nameIterator.value());
				
			}
			found = true;
		}
		nameIterator++;
	}

	// sur changement d une configuration creer / maj des managed configurations
}           

QList<_SwManagedConfiguration *> _SwManagedConfigurationsList::getConfigurationsList()
{
    return _managedConfigurations.values();
}
