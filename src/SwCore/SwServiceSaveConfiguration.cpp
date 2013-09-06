/**
	@file : SwServiceSaveConfiguration.h
	@brief : Service permettant de gérer la sauvegarde de la conf
	@author : CGD
*/


#include <QMessageBox>
#include <QDebug>
//#include <QElapsedTimer>


#include "SwServiceSaveConfiguration.h"
#include "_SwPropertyPersistent_Toolbox.h"


using namespace StreamWork::SwCore;


//---------------------------------------------------------
// Constantes pour le contenu des fichiers XML 
//---------------------------------------------------------

// constantes pour les tags
const QString CFM_XML_TAG_FILE			= "ConfigurationsFile";
const QString CFM_XML_TAG_CONFIG		= "Config";
const QString CFM_XML_TAG_PROPERTY		= "property";

// nom du fichier XML par défaut
const QString CFM_DEFAULT_FILENAME		= "Factory settings";

// constantes pour les attributs
const QString CFM_XML_CONFIG_NAME       = "name";
const QString CFM_XML_CONFIG_CURRENT    = "current";
const QString CFM_XML_CONFIG_DEFAULT	= "default";

const QString CFM_XML_PROPERTY_PREFIX   = "prefix";
const QString CFM_XML_PROPERTY_NAME		= "pname";




//-----------------------------------------------------------------------
SwServiceSaveConfiguration::SwServiceSaveConfiguration()
{

}

//-----------------------------------------------------------------------
SwServiceSaveConfiguration::~SwServiceSaveConfiguration()
{

}

//-------------------------------------------------------------------------
// Interface ISwAdminConfiguration
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::loadConfigurationFile( QString confName, QString confFileToBeLoaded )
{
	// La méthode est appellée par le composant ConfLoader.
	// Les ISwConfCollector on été renseignés avant (dans le finalize des composants des sous streams ConfCollector)


	// Parsage du fichier de conf (InputFile) 
	// récupération de la conf courante et set dans "currentConf"
	// récupération de tous les profils de conf et set dans "confProfilesDatas"
	
	//QElapsedTimer timer;
	//timer.start();


	QString configInitiale = "";
	configInitiale = parseConfigurationFile(confName, confFileToBeLoaded);

	// /!\ note to self : le QDomDocument n'aime pas le récursif...
	if(configInitiale != "")
		parseConfigurationFile(confName, configInitiale);

	//qDebug() << "The parseConfigurationFile operation took" << timer.restart() << "milliseconds";

	//Récupération de la confCourrante 
	QString	currentConfigProfile = "";
	currentConfigProfile = getCurrentConf(confName);

	// set des valeurs des properties
	setPropertiesValues(confName, currentConfigProfile);

	//qDebug() << "The setPropertiesValues operation took" << timer.restart() << "milliseconds";

	// mettre à jour les _loadedConfs
	QHash<QString, bool>::iterator it = _loadedConfs.find(confName);
	if(it != _loadedConfs.end())
	{
		it.value() = true; 
	}

	notifyLiteners(confName);
}


//-------------------------------------------------------------------------
QHash<QString, ISwConfCollector*> SwServiceSaveConfiguration::getConfCollectors(QString confName)
{
	QHash<QString, ISwConfCollector*> confCols;

	// checker s'il y a une liste de confCollectors associé au confName 
	QHash<QString, QHash<QString, ISwConfCollector*>>::const_iterator it = _confCollectors.find(confName);
	if(it != _confCollectors.constEnd())
	{
		confCols = it.value();
	}
	return confCols;
}


//-------------------------------------------------------------------------
ISwConfCollector* SwServiceSaveConfiguration::getConfCollector( QString confName, QString prefix )
{
	ISwConfCollector* confCol = 0;

	// checker s'il y a un confCollector associé au confName et au prefix
	QHash<QString, QHash<QString, ISwConfCollector*>>::const_iterator it = _confCollectors.find(confName);
	if(it != _confCollectors.constEnd())
	{
		QHash<QString, ISwConfCollector*>::const_iterator it2 = it.value().find(prefix);
		if(it2 != it.value().constEnd())
		{
			confCol = it2.value();
		}
	}
	return confCol;
}


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::registerConfCollector( QString confName, QString prefix, ISwConfCollector* confCollector )
{
	bool insertCollector = false;

	//checker s'il y a déja une entrée dans _loadedConfs pour "confName"
	QHash<QString, bool>::iterator it = _loadedConfs.find(confName);
	if(it != _loadedConfs.end())
	{
		if(it.value() == true)
		{
			// ConfName déja loadée, erreur,
			 QMessageBox::warning(NULL,
								"Configuration already finalized and loaded",
								QString("Cannot add anymore ConfigurationPropertiesCollectors for configuration \"%1\" and prefix \"%2\"").arg(confName).arg(prefix));
		}
		else
		{
			insertCollector = true;
		}
	}
	else
	{
		_loadedConfs.insert(confName, false);
		insertCollector = true;
	}

	if(insertCollector)
	{
		//checker s'il y a déja un confCollector associé au confName
		QHash<QString, QHash<QString, ISwConfCollector*>>::iterator it = _confCollectors.find(confName);

		
		if(it != _confCollectors.end())
		{
			// Si l'entrée existe déja dans la QHash, on fusionne les properties dans les confCollectors
			if(it.value().contains(prefix))
			{
				QHash<QString, ISwProperty*> propertiesToMerge = confCollector->getProperties();

				QHashIterator<QString, ISwProperty*> it_properties(propertiesToMerge);
				while (it_properties.hasNext()) 
				{
					it_properties.next();
					ISwProperty* propertyToMerge = it_properties.value();
					it.value().value(prefix)->addExternalProperty(it_properties.key(), propertyToMerge);
				}				
			}
			// Sinon, on lui rajoute un confCollector
			else
			{
				it.value().insert(prefix, confCollector);
			}
		}
		// Sinon on rajoute une entrée dans la liste _confCollectors
		else
		{
			QHash<QString, ISwConfCollector*> coll;
			coll.insert(prefix, confCollector);

			_confCollectors.insert(confName, coll);
		}
	}
}


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::unregisterConfCollector( QString confName)
{
	_confCollectors.remove(confName);
}


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::registerConfSaver( QString confName, ISwConfSaver* confSaver )
{
	//check s'il y a déja un confCollector associé au confName
	QHash<QString, ISwConfSaver*>::iterator it = _confSavers.find(confName);

	// s'il n'y en a pas, on enregistre le confSaver
	if(it == _confSavers.end())
	{
		_confSavers.insert(confName, confSaver);
	}
	else
	{
		QMessageBox::warning(0,"Register ConfSaver",QString("Cannot regiter this confSaver because there is already a confSaver registered for configuration : %1").arg(confName));
	}
}


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::unregisterConfSaver( QString confName )
{
	_confSavers.remove(confName);
}


//-------------------------------------------------------------------------
bool SwServiceSaveConfiguration::isConfLoaded( QString confName )
{
	QHash<QString, bool>::const_iterator it = _loadedConfs.find(confName);
	if(it != _loadedConfs.constEnd())
	{
		return it.value();
	}
	else
		return false;
}


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::registerConfigServiceListener( ISwConfigListener *listener )
{
	if(_configurationServiceListeners.indexOf(listener) < 0)
	{
		_configurationServiceListeners.append(listener);

		// On notifie le listener immédiatement si la conf est déja chargée
		QString _listenerConfName = listener->getListenerName();

		QHash<QString, bool>::iterator it_loaded = _loadedConfs.find(_listenerConfName);
		if(it_loaded != _loadedConfs.end())
		{
			if(it_loaded.value() == true)
				listener->notifyConfiguration(_listenerConfName);
		}
	}
	else
	{
		QMessageBox::warning(0,"Register ConfigServiceListener",QString("Cannot regiter this ConfigServiceListener because there is already registered"));
	}
}

//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::unregisterConfigServiceListener( ISwConfigListener *listener )
{
	_configurationServiceListeners.removeOne(listener);
}


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::clearConfService()
{
	_confSavers.clear();
	_confCollectors.clear();
	_currentConfs.clear();
	_loadedConfs.clear();
	_confProfilesDatas.clear();
	_configsProfilesList.clear();
	_configurationServiceListeners.clear();
}



//-------------------------------------------------------------------------
// Interface ISwServiceConfiguration
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::createNewConfiguration( QString confName, QString confProfileName, bool fromCurrent )
{
	// Création d'un nouveau QDomElement qui est, selon @param bool :
	QDomElement newProfileConfDatas;
	QDomDocument doc;
	QDomElement root_node;
	root_node = doc.createElement(CFM_XML_TAG_FILE);
	doc.appendChild(root_node);


	// save des valeurs courantes des properties (cf saveConfigFile) dans newProfileConfDatas
	bool newConfCreated = false;
	if(fromCurrent)
	{
		newProfileConfDatas = doc.createElement(CFM_XML_TAG_CONFIG);
		newProfileConfDatas.setAttribute(CFM_XML_CONFIG_DEFAULT,		"false");
		newProfileConfDatas.setAttribute(CFM_XML_CONFIG_CURRENT,		"true");
		newProfileConfDatas.setAttribute(CFM_XML_CONFIG_NAME,		confProfileName);

		createQDomProfile(confName, doc, newProfileConfDatas);

		newConfCreated = true;
	}
	// copie du QDomElement de la conf par défaut (paramètres usine) 	
	else
	{
		QHash<QString, QHash<QString, QString>>::iterator it_profiles = _confProfilesDatas.find(confName);
		if(it_profiles != _confProfilesDatas.end())
		{
			QHash<QString, QString>::iterator it = it_profiles.value().find(CFM_DEFAULT_FILENAME);
			if(it != it_profiles.value().end())
			{
				QDomDocument tempDoc;
				tempDoc.setContent(it.value());
				QDomNode tempNode = tempDoc.firstChild();
				if (!tempNode.isNull() && tempNode.isElement()) 
				{
					newProfileConfDatas = tempNode.toElement();
				}

				newProfileConfDatas.setAttribute(CFM_XML_CONFIG_DEFAULT,		"false");
				newProfileConfDatas.setAttribute(CFM_XML_CONFIG_CURRENT,		"true");
				newProfileConfDatas.setAttribute(CFM_XML_CONFIG_NAME,		confProfileName);
				
				newConfCreated = true;
			}
		}
	}

	if(newConfCreated)
	{
		// Faire un append du QDomElement au QDomDocument créé au début (balise ConfigurationsFile ouverte)
		root_node.appendChild(newProfileConfDatas);

		QDomDocument tempDoc;
		QDomNode tempNode;

		// Il faut mettre les valeurs de current à "false" pour tous les autres profils de conf
		QHash<QString, QHash<QString, QString>>::iterator it_profiles = _confProfilesDatas.find(confName);
		if(it_profiles != _confProfilesDatas.end())
		{
			// Pour chaque autre QDomElement correspondant aux profils de conf, on passe leur value "current" à false
			QHashIterator<QString, QString> it_other_confs(it_profiles.value());
			while (it_other_confs.hasNext()) 
			{
				it_other_confs.next();

				// Récupération du QDomElement du profil de conf 
				QDomElement temp;
				tempDoc.clear();
				tempDoc.setContent(it_other_confs.value());
				QDomNode tempNode = tempDoc.firstChild();
				if (!tempNode.isNull() && tempNode.isElement()) 
				{
					temp = tempNode.toElement();
				}

				if(it_other_confs.key() != confProfileName && !temp.isNull())
				{
					// set de son attribut "current" à "false"
					temp.setAttribute(CFM_XML_CONFIG_CURRENT,	"false");
					root_node.appendChild(temp);

					// Remplacement dans _confProfilesDatas[confName] de la QString mise à jour
					tempDoc.clear();
					tempNode = tempDoc.importNode(temp, true).toElement();
					tempDoc.appendChild(tempNode);
					it_profiles.value().insert(it_other_confs.key(), tempDoc.toString());
				}
			}
			
			// Ajout d'une entrée dans confProfileDatas[confName] avec la key confProfileName et la valeur du QDomElement précédement créé.
			tempDoc.clear();
			tempNode = tempDoc.importNode(newProfileConfDatas, true).toElement();
			tempDoc.appendChild(tempNode);
			it_profiles.value().insert(confProfileName, tempDoc.toString());

			// Ajout de l'entrée dans  _configsProfilesList[confName]
			QHash<QString, QList<QString>>::iterator it = _configsProfilesList.find(confName);
			if(it != _configsProfilesList.end())
			{
				it.value().append(confProfileName);
			}

			// Modification de la QHash currentConfs pour que la confProfile courante soit la nouvelle créé.
			_currentConfs.insert(confName, confProfileName);

			// Changement des valeurs des properties
			setPropertiesValues(confName, confProfileName);

			// appel de la méthode writeConfigurationFile[confName]
			writeConfigurationFile(confName, doc);

			notifyLiteners(confName);
		}	
	}
}

//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::deleteConfiguration( QString confName )
{
	// Récupération de la confCourante (on ne peut supprimer que la conf courante)
	QString	currentConfigProfile = "";
	currentConfigProfile = getCurrentConf(confName);
	
	// On ne peux pas supprimer la config par défaut
	if(currentConfigProfile != CFM_DEFAULT_FILENAME)
	{
		// Suppression de l'entrée QDomElement dans _confProfilesDatas[confName] 
		// s'il y a au moins 3 éléments dans la liste : la défaut, la courante et une autre
		QHash<QString, QHash<QString, QString>>::iterator it_profiles = _confProfilesDatas.find(confName);
		if(it_profiles != _confProfilesDatas.end() && it_profiles.value().size() > 2)
		{
			// if remove operation succeed
			if(it_profiles.value().remove(currentConfigProfile) > 0)
			{
				// Suppression de l'entrée dans  _configsProfilesList[confName]
				// s'il y a au moins 2 éléments dans la liste : la courante et une autre
				QHash<QString, QList<QString>>::iterator it = _configsProfilesList.find(confName);
				if(it != _configsProfilesList.end() && it.value().size() > 1)
				{
					it.value().removeOne(currentConfigProfile);

					// Changement de la valeur dans _currentConfs[confName]
					// On se positionne sur la première de la liste
					_currentConfs.insert(confName, it.value().at(0));

					// Changement des valeurs des properties
					setPropertiesValues(confName, it.value().at(0));

					// appel de la méthode saveConfFile[confName]
					saveConfigurationFile(confName);

					notifyLiteners(confName);
				}
			}
		}
	}
}

//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::renameConfiguration( QString confName, QString newConfProfileName )
{
	// Récupération de la confCourante (on ne peut renomer que la conf courante)
	QString	oldCurrentConfigProfileName = "";
	oldCurrentConfigProfileName = getCurrentConf(confName);

	QString oldCurrentConfigDatas;

	QHash<QString, QHash<QString, QString>>::iterator it_profiles = _confProfilesDatas.find(confName);
	if(it_profiles != _confProfilesDatas.end() && it_profiles.value().size() > 2)
	{
		QHash<QString, QString>::iterator it = it_profiles.value().find(oldCurrentConfigProfileName);
		if(it != it_profiles.value().end())
		{
			// on récupère le QDomElement associé dans confProfilesDatas[confName][oldConfProfileName]
			oldCurrentConfigDatas = it.value();

			// on supprime l'element de la QHash
			it_profiles.value().remove(oldCurrentConfigProfileName);

			// on en ajoute un nouveau avec comme clé la newConfProfileName et comme value le QDomElement sauvé
			it_profiles.value().insert(newConfProfileName, oldCurrentConfigDatas);

			// On change également le nom du profil courant dans _currentConfs
			_currentConfs.insert(confName, newConfProfileName);

			// Modif de l'entrée dans  _configsProfilesList[confName]
			QHash<QString, QList<QString>>::iterator it = _configsProfilesList.find(confName);
			if(it != _configsProfilesList.end())
			{
				it.value().removeOne(oldCurrentConfigProfileName);
				it.value().append(newConfProfileName);
			}

			// appel de la méthode saveConfFile[confName]
			saveConfigurationFile(confName);
			
			notifyLiteners(confName);
		}
	}
}


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::switchConfiguration( QString confName, QString confProfileName )
{
	QHash<QString, QHash<QString, QString>>::iterator it_profiles = _confProfilesDatas.find(confName);
	if(it_profiles != _confProfilesDatas.end())
	{
		// On vérifie que le profil de conf est bien dans la liste
		QHash<QString, QString>::iterator it = it_profiles.value().find(confProfileName);
		if(it != it_profiles.value().end())
		{
			// Changement de valeurs de properties
			setPropertiesValues(confName, confProfileName);

			// On change également le nom du profil courant dans _currentConfs
			_currentConfs.insert(confName, confProfileName);

			// appel de la méthode saveConfFile[confName] (on a changé les valeurs de l'attribut "current" dans les QDomElements)
			saveConfigurationFile(confName);
		}
	}
}


// Pour les properties ayant le préfix "parametersConcerned"
// On recharge les valeurs de celle de la conf courante ou de celle de 
// la conf par défaut selon la valeur de "fromDefault"
//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::restoreCancelConfig( QString confName, QString parametersConcerned, bool fromDefault )
{
	// Récupération de la confCourante 
	QString	currentConfigProfile = "";
	currentConfigProfile = getCurrentConf(confName);


	// Si on veut une restauration de la conf courante (annulation des modifs en cours)
 	if(!fromDefault)
	{
		// On reload les properties de la conf courante
		switchConfiguration(confName, currentConfigProfile);
	}
	// Si on veut une restauration de la conf par défaut (reset properties)
	else
	{
		QDomDocument doc_default;

		QHash<QString, QHash<QString, QString>>::iterator it_profiles = _confProfilesDatas.find(confName);
		if(it_profiles != _confProfilesDatas.end())
		{
			// Récupération des QDom des profils courrants et défaut
			QHash<QString, QString>::iterator it_default = it_profiles.value().find(CFM_DEFAULT_FILENAME);
			
			if(it_default != it_profiles.value().end() )
			{
				doc_default.setContent(it_default.value());
			
				QDomElement xmlDefaultProfile = doc_default.firstChildElement(CFM_XML_TAG_CONFIG);
				QDomNodeList PropertiesElements = xmlDefaultProfile.elementsByTagName(CFM_XML_TAG_PROPERTY);
				for(int i=0; i<PropertiesElements.size(); i++)
				{
					QString prefix = "";
					QString decoratedName = "";
					QDomElement val;

					prefix = PropertiesElements.at(i).toElement().attribute(CFM_XML_PROPERTY_PREFIX);
					decoratedName = PropertiesElements.at(i).toElement().attribute(CFM_XML_PROPERTY_NAME);
					val = PropertiesElements.at(i).toElement();

					// On load la valeur de la property uniquement si le prefix 
					// est celui du groupe de paramètres que l'on veut restaurer
					if(prefix == parametersConcerned)
					{
						// Avec ces valeurs, on set au fur et à mesure toutes les values des Properties :
						// Le *ISwProperty est récupéré via confCollectors[confName][prefix]->getProperty(name)
						ISwProperty* prop = 0;

						QHash<QString, QHash<QString, ISwConfCollector*>>::const_iterator it = _confCollectors.find(confName);
						if(it != _confCollectors.constEnd())
						{
							QHash<QString, ISwConfCollector*>::const_iterator it2 = it.value().find(prefix);
							if(it2 != it.value().constEnd())
							{
								prop = it2.value()->getProperty(decoratedName);

								// Utilisation de la méthode LoadProperty(QDomElement, ISwProperty*) de SwPropertyPersistent
								// Le QDomElement associé est la ligne XML <property name : ...  value : ... >
								_SwPropertyPersistent_Toolbox::LoadProperty(val, prop);
							}
						}
					}
				}
			}
		}
	}
}


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::saveConfigurationFile( QString confName)
{
	// QDomDoc temporaire pour la création des profils de conf à renseigner dans _confProfilesDatas
	QDomDocument confFileToSave;

	createConfigurationFile(confName, confFileToSave);
	writeConfigurationFile(confName, confFileToSave);
}


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::createConfigurationFile( QString confName, QDomDocument &doc )
{
	// 1) Création d'un QDomDocument avec un noeud racine qui comportera tous les profils de conf
	QDomDocument tempDoc;
	QDomElement root_node;
	root_node = doc.createElement(CFM_XML_TAG_FILE);

	// Faire un append du QDomElement au QDomDocument créé au début (balise ConfigurationsFile ouverte)
	doc.appendChild(root_node);


	//2) Récupération de la confCourante 
	QString	currentConfigProfile = "";
	currentConfigProfile = getCurrentConf(confName);


	// 3) Création d'un QDomElement contenant une première balise <Config> pour la confCourante
	QDomElement elt_current_config;
	elt_current_config = doc.createElement(CFM_XML_TAG_CONFIG);
	elt_current_config.setAttribute(CFM_XML_CONFIG_DEFAULT,		"false");
	elt_current_config.setAttribute(CFM_XML_CONFIG_CURRENT,		"true");
	elt_current_config.setAttribute(CFM_XML_CONFIG_NAME,		currentConfigProfile);


	// récupération des valeurs courantes des properties pour setter la confCourante 
	createQDomProfile(confName, doc, elt_current_config); 


	// Fermer la balise <Config>
	root_node.appendChild(elt_current_config);



	// 4) Parcourir les confProfilesDatas[confName], pour chaque autre profil de conf 
	// Faire un append dans le root_node des QDomElement correspondants aux profiles de conf.
	QHash<QString, QHash<QString, QString>>::iterator it_profiles = _confProfilesDatas.find(confName);
	if(it_profiles != _confProfilesDatas.end())
	{
		QHashIterator<QString, QString> it_config(it_profiles.value());
		while (it_config.hasNext()) 
		{
			it_config.next();

			// Création du QDomNode du profil de conf à partir du QString dans _confProfilesDatas
			tempDoc.clear();
			tempDoc.setContent(it_config.value());
			QDomNode tempElem = tempDoc.firstChild();


			// SAUF LA COURANTE que l'on vient de recréer avec les nouvelles valeurs des properties, 
			// On récupère les autres conf pour les setter à (CURRENT = false) et les ajouter au XML
			tempDoc.clear();
			if(it_config.key() != currentConfigProfile && !tempElem.isNull())
			{
				// Copie du profil dans un nouveau QDomElement attaché au bon QDomDocument
				QDomElement node = doc.importNode(tempElem, true).toElement();
				QDomElement internalNode = tempDoc.importNode(tempElem, true).toElement();

				// Changement de la valeur de l'attribut CURRENT
				node.setAttribute(CFM_XML_CONFIG_CURRENT,	"false");
				internalNode.setAttribute(CFM_XML_CONFIG_CURRENT,	"false");

				// Ajout dans le QDomDocument correspondant le QDomElement MAJ
				root_node.appendChild(node);
				tempDoc.appendChild(internalNode);
			}
			else
			{
				// transformer le "elt_current_config" en QDomNode pour l'ajouter au QDomDocument
				QDomNode e = tempDoc.importNode(elt_current_config, true);
				tempDoc.appendChild(e);
			}

			// Remplacement aussi dans _confProfilesDatas[confName] (MAJ)
			it_profiles.value().insert(it_config.key(), tempDoc.toString());
		}
	}
}


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::writeConfigurationFile( QString confName, QDomDocument &doc )
{
	// Transformer le QDomDocument en QString
	QString confFileContent = "";
	confFileContent = doc.toString(4);

	// Faire appel à confSavers[confName]->saveCallBack(QString) avec les datas de la nouvelle conf
	QHash<QString, ISwConfSaver*>::const_iterator it_savers = _confSavers.find(confName);
	if(it_savers != _confSavers.end() && confFileContent != 0)
	{
		it_savers.value()->saveCallBack(confFileContent);
	}
}


//-------------------------------------------------------------------------
QList<QString> SwServiceSaveConfiguration::getConfigurationProfilesList( QString confName )
{
	QList<QString> profilesList;

	// Construit une QList, depuis _confProfilesDatas, contenant les noms de tous les profiles de conf dispo
	QHash<QString, QHash<QString, QString>>::const_iterator it = _confProfilesDatas.find(confName);
	if(it != _confProfilesDatas.constEnd())
	{
		QHashIterator<QString, QString> it2(it.value());
		while (it2.hasNext()) 
		{
			it2.next();
			
			QString profileName = it2.key();

			// On n'ajoute pas la config par défaut dans la liste
			if(profileName != CFM_DEFAULT_FILENAME)
				profilesList.append(profileName);
		}
	}
	return profilesList;
}



//-------------------------------------------------------------------------
QString SwServiceSaveConfiguration::getCurrentConf( QString confName )
{
	QString	_currentConfigProfile = "";

	QHash<QString, QString>::const_iterator it_currentConf = _currentConfs.find(confName);
	if(it_currentConf != _currentConfs.constEnd())
	{
		return it_currentConf.value();
	}
	else
		return "";
}


//-------------------------------------------------------------------------
ISwAdminConfiguration* SwServiceSaveConfiguration::getAdmin()
{
	return this;
}


//-------------------------------------------------------------------------
ISwProperty* SwServiceSaveConfiguration::getProperty( QString confName, QString prefix, QString decoratedName )
{
	ISwProperty* returnedProp = 0;

	// On ne peux pas utiliser l'opérateur [] de la QHash car il créé une entrée si l'élément n'existe pas
	QHash<QString, QHash<QString, ISwConfCollector*>>::const_iterator it = _confCollectors.find(confName);

	if(it != _confCollectors.end())
	{
		QHash<QString, ISwConfCollector*>::const_iterator it2 = it.value().find(prefix);
		
		if(it2 != it.value().end())
		{
			ISwConfCollector *collector = it2.value();
			if(collector)
				returnedProp = collector->getProperty(decoratedName);
		}
	}
	return returnedProp;
}



//-------------------------------------------------------------------------
// Private Méthodes
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QString SwServiceSaveConfiguration::parseConfigurationFile(QString confName, QString inConfigFileToParse)
{
	bool inDefaultConfig = false;
	int nbProfiles = 0;

	// Créer un seul QDomDoc pour toute la fonction sinon plantage...
	QDomDocument tempDoc;

	// Création du QDomDocument à partir du QString contenant les données du fichier de conf
	QDomDocument _xmlConfigFileDocument;
	_xmlConfigFileDocument.setContent(inConfigFileToParse);

	// On récupère la liste des configs dans le fichier
	QDomNodeList ConfigElements = _xmlConfigFileDocument.documentElement().elementsByTagName(CFM_XML_TAG_CONFIG);

	// Pour chaque config dans le fichier, on récupère les datas (set dans "confProfilesDatas")
	// et set de la confCourrante dans "currentConf"
	for(int i=0; i<ConfigElements.size(); i++)
	{
		QString profilName = ConfigElements.at(i).toElement().attribute(CFM_XML_CONFIG_NAME);
		if(profilName != "") 
		{
			// Récupération du QString associé à chaque QDomElement
			tempDoc.clear();
			QDomNode xmlDatas  = tempDoc.importNode(ConfigElements.at(i), true);
			tempDoc.appendChild(xmlDatas);
			QString configDatas = tempDoc.toString();

			//ajout dans QHash<QString, QHash<QString, QString>> _confProfilesDatas;
			QHash<QString, QString> aConfig;
			QHash<QString, QHash<QString, QString>>::iterator it = _confProfilesDatas.find(confName);
			if(it != _confProfilesDatas.end())
			{
				it.value().insert(profilName, configDatas);
			}
			else
			{
				aConfig.insert(profilName, configDatas);
				_confProfilesDatas.insert(confName, aConfig);
			}
			

			// Ajout de la Config (récupération de son nom) dans la liste si elle n'y est pas déja
			// /!\ On n'ajoute pas la config contenant les paramètres pas défaut!!
			if(profilName != CFM_DEFAULT_FILENAME)
			{
				QHash<QString, QList<QString>>::iterator it2 = _configsProfilesList.find(confName);
				if(it2 != _configsProfilesList.end())
				{	
					if(!it2.value().contains(profilName))
						it2.value().append(profilName);
				}
				else
				{
					QList<QString> profiles;
					profiles.append(profilName);
					_configsProfilesList.insert(confName, profiles);
				}
				nbProfiles++;
			}
			else
				inDefaultConfig = true;
		}

		// récupération de la conf courrante et set dans "currentConf"
		// /!\ Il ne peux y avoir qu'une seule config tagguée comme courrante dans le fichier de conf
		// Si l'élément a un attribut "current" sans être la config par défaut
		QString configAttribut2 = ConfigElements.at(i).toElement().attribute(CFM_XML_CONFIG_CURRENT);
		if(configAttribut2 != "" && profilName != CFM_DEFAULT_FILENAME && profilName != "")		
		{
			if(configAttribut2 == "true")			// Et qu'il est à true
			{
				_currentConfs.insert(confName, profilName);
			}
		}	
	}


	// Check s'il y a un fichier de conf présent sur la machine (et valide)
	// Check s'il y a une conf par défaut et une conf courrante, sinon on les créées 
	if(!inDefaultConfig || nbProfiles == 0)
	{
		QDomDocument doc;
		QDomElement elt_new_config;
		QDomElement root_node;
		root_node = doc.createElement(CFM_XML_TAG_FILE);

		if(!inDefaultConfig)
		{
			elt_new_config = doc.createElement(CFM_XML_TAG_CONFIG);
			elt_new_config.setAttribute(CFM_XML_CONFIG_DEFAULT,		"true");
			elt_new_config.setAttribute(CFM_XML_CONFIG_CURRENT,		"false");
			elt_new_config.setAttribute(CFM_XML_CONFIG_NAME,		CFM_DEFAULT_FILENAME);

			createQDomProfile(confName, doc, elt_new_config);

			// Fermer la balise <Config>
			root_node.appendChild(elt_new_config);			
		}
		if(nbProfiles == 0)
		{
			elt_new_config = doc.createElement(CFM_XML_TAG_CONFIG);
			elt_new_config.setAttribute(CFM_XML_CONFIG_DEFAULT,		"false");
			elt_new_config.setAttribute(CFM_XML_CONFIG_CURRENT,		"true");
			elt_new_config.setAttribute(CFM_XML_CONFIG_NAME,		"default config");

			createQDomProfile(confName, doc, elt_new_config);
			
			// Fermer la balise <Config>
			root_node.appendChild(elt_new_config);	
		}

		// Faire un append du QDomElement au QDomDocument créé au début (balise ConfigurationsFile ouverte)
		doc.appendChild(root_node);
		
		// Transformer le QDomDocument en QString
		QString confFileContent = "";
		confFileContent = doc.toString(4);

		// Faire appel à confSavers[confName]->saveCallBack(QString) avec les datas de la nouvelle conf
		QHash<QString, ISwConfSaver*>::const_iterator it_savers = _confSavers.find(confName);
		if(it_savers != _confSavers.end() && confFileContent != 0)
		{
			it_savers.value()->saveCallBack(confFileContent);
		}

		// Nécessaire de reloader le fichier maintenant qu'il a été créé et initié une première fois
		return confFileContent;	
	}
	return "";
}




//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::setPropertiesValues( QString confName, QString confProfileName )
{
	// Récupération des datas (QString) pour le profil voulu
	QString profileDatas;

	QHash<QString, QHash<QString, QString>>::const_iterator it = _confProfilesDatas.find(confName);
	if(it != _confProfilesDatas.constEnd())
	{
		QHash<QString, QString>::const_iterator it2 = it.value().find(confProfileName);
		if(it2 != it.value().constEnd())
		{
			profileDatas = it2.value();
		}
	}

	QDomDocument doc;
	QDomElement xmlProfileDatas;
	doc.setContent(profileDatas);
	xmlProfileDatas = doc.firstChildElement(CFM_XML_TAG_CONFIG);


	// Parsage du QDom, pour chaque élément, on récupère :  prefix - decoratedName - value (QDom)
	QDomNodeList PropertiesElements = xmlProfileDatas.elementsByTagName(CFM_XML_TAG_PROPERTY);
	for(int i=0; i<PropertiesElements.size(); i++)
	{
		QString prefix = "";
		QString decoratedName = "";
		QDomElement val;

		prefix = PropertiesElements.at(i).toElement().attribute(CFM_XML_PROPERTY_PREFIX);
		decoratedName = PropertiesElements.at(i).toElement().attribute(CFM_XML_PROPERTY_NAME);
		val = PropertiesElements.at(i).toElement();

		// Avec ces valeurs, on set au fur et à mesure toutes les values des Properties :
		// Le *ISwProperty est récupéré via confCollectors[confName][prefix]->getProperty(name)
		ISwProperty* prop = 0;

		QHash<QString, QHash<QString, ISwConfCollector*>>::const_iterator it = _confCollectors.find(confName);
		if(it != _confCollectors.constEnd())
		{
			QHash<QString, ISwConfCollector*>::const_iterator it2 = it.value().find(prefix);
			if(it2 != it.value().constEnd())
			{
				prop = it2.value()->getProperty(decoratedName);

				// Utilisation de la méthode LoadProperty(QDomElement, ISwProperty*) de SwPropertyPersistent
				// Le QDomElement associé est la ligne XML <property name : ...  value : ... >
				_SwPropertyPersistent_Toolbox::LoadProperty(val, prop);
			}
		}
	}
}



//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::createQDomProfile(QString confName, QDomDocument &doc, QDomElement &elt_config)
{
	// récupération des valeurs courantes des properties pour setter la conf par défault 
	QHash<QString, QHash<QString, ISwConfCollector*>>::const_iterator it = _confCollectors.find(confName);
	if(it != _confCollectors.end())
	{
		// pour chaque préfixe de confCollector[confName]  :
		QHashIterator<QString, ISwConfCollector*> it_prefixes(it.value());
		while (it_prefixes.hasNext()) 
		{
			it_prefixes.next();

			// récupérer le pointeur sur l'ISwConfCollector
			ISwConfCollector *collector = it_prefixes.value();

			// faire un getProperties() et parcourir toutes les properties (y compris les externals)
			QHash<QString, ISwProperty*> props = collector->getProperties();

			QHashIterator<QString, ISwProperty*> it_props(props);
			while (it_props.hasNext()) 
			{
				it_props.next();

				// Pour chaque * ISwProperty faire appel à la méthode SaveProperty de SwProperyPersistent_ToolBox
				_SwPropertyPersistent_Toolbox::SaveProperty(elt_config, doc, it_props.key(), it_props.value(), it_prefixes.key());
			}
		}
	}
}


//-------------------------------------------------------------------------
void SwServiceSaveConfiguration::notifyLiteners(QString confName)
{
	for(int i=0; i<_configurationServiceListeners.size(); i++)
	{
		// On notifie uniquement les Listeners concernés par la confName
		if(_configurationServiceListeners.at(i)->getListenerName() == confName)
			_configurationServiceListeners.at(i)->notifyConfiguration(confName);
	}
}


