/**
  @file : SwServiceSaveConfiguration.h
  @brief : Service permettant de gérer la sauvegarde de la conf
  @author : CGD
 */

#ifndef _STREAMWORK_SWCORE__SWSERVICESAVECONFIGURATION_H
#define _STREAMWORK_SWCORE__SWSERVICESAVECONFIGURATION_H

#include <QDomElement>

#include "ISwAdminConfiguration.h"
#include "ISwServiceConfiguration.h"
#include "ISwConfigListener.h"

#include "ISwConfigurationManager.h"
#include "ISwPropertiesObserver.h"
#include "ISwProperty.h"


namespace StreamWork 
{
    namespace SwCore 
    {
        /**
          @class SwServiceSaveConfiguration
        */
        class SwServiceSaveConfiguration :  public ISwAdminConfiguration, 
											public ISwServiceConfiguration,
											public ISwConfigurationManager
        {
        public:

            /** @brief Constructor */
            SwServiceSaveConfiguration();

            /** @brief Destructor */
            virtual ~SwServiceSaveConfiguration();

			//---------------------------------------------------------------------
			// Interface ISwAdminConfiguration
			//---------------------------------------------------------------------
			
			virtual bool loadConfigurationFile( QString confName, QString confFileToBeLoaded );

			/**
             * @brief	: Permet de récupérer tous les ConfCollectors registered
             * @Return	: List<QHash<QString, ISwConfCollector*>, liste des pointeurs sur les Prefix/ConfCollectors
             */
			virtual QHash<QString, ISwConfCollector*> getConfCollectors(QString confName);

			/**
             * @brief	: Permet de récupérer un ConfCollector particulier
             * @Param	: QString, nom du confCollector voulu
             * @Param	: QString, prefix du confCollector
             * @Return	: ISwConfCollector*, pointeur sur le ConfCollector 
             */
			virtual ISwConfCollector* getConfCollector( QString confName, QString prefix );
			
			/**
             * @brief	: Permet d'enregistrer un ConfCollector
             * @Param	: QString, nom de la configuration concernée
             * @Param	: QString, prefix du confCollector
             * @Param	: ISwConfCollector* pointeur vers le confCollector à registered
             */
			virtual bool registerConfCollector( QString confName, QString prefix, ISwConfCollector* confCollector, bool autoSave );
			
			/**
             * @brief	: Permet désenregistrer un ConfCollector
             * @Param	: QString, nom de la configuration concernée
             * @Param	: ISwConfCollector* pointeur vers le confCollector à désenregistrer
             */
			virtual void unregisterConfCollector( QString confName, QString prefix, ISwConfCollector* confCollector );

			/**
             * @brief	: Permet d'enregistrer un IConfSaver (confLoader)
             * @Param	: QString, nom de la configuration concernée
             * @Param	: IConfSaver* pointeur vers le confSaver à registered
             */
			virtual bool registerConfSaver( QString confName, ISwConfSaver* confSaver );

			/**
             * @brief	: Permet désenregistrer un IConfSaver (confLoader)
             * @Param	: QString, nom de la configuration concernée
             * @Param	: IConfSaver* pointeur vers le confCollector à désenregistrer
             */
			virtual void unregisterConfSaver( QString confName);

			/**
             * @brief	: Permet de savoir si une conf a été chargée ou non
             * @Param	: QString, nom de la configuration concernée
             * @return	: bool, true si configuration chargée (finalized via le composant ConfLoader)
             */
			virtual bool isConfLoaded( QString confName );


			/**
             * @brief	: Permet register les listener pour les notifier par la suite
             * @Param	: ISwConfigListener, listener du service    
             */
			virtual bool registerConfigServiceListener( ISwConfigListener *listener );


			/**
             * @brief	: Permet de désenregister les listeners
             * @Param	: ISwConfigListener, listener du service    
             */
            virtual void unregisterConfigServiceListener (ISwConfigListener *listener);


			/**
             * @brief	: Permet de vider les Maps du service de conf
             */
			virtual void clearConfService();



			//---------------------------------------------------------------------
			// Interface ISwConfigurationManager
			//---------------------------------------------------------------------
			/**
			* @brief	: Permet d'enregistrer un ConfPropertiesObserver
			* @Param	: ISwPropertiesObserver* observer des properties
			*/
			virtual bool registerConfPropertiesObserver(ISwPropertiesObserver * observer);

			/**
			* @brief	: Permet désenregistrer un ConfPropertiesObserver
			* @Param	: ISwPropertiesObserver* observer des properties
			*/
			virtual void unregisterConfPropertiesObserver(ISwPropertiesObserver * observer);



			//---------------------------------------------------------------------
			// Interface ISwPropertiesObserver
			//---------------------------------------------------------------------
			/** @brief : Fonction appelée par les confcollectors lors du delete d'une property */
			virtual void notifyPropertyDeleted(ISwProperty * propertyDeleted, QString propertyDecoratedName, QString confName = "");


			//---------------------------------------------------------------------
			// Interface ISwServiceConfiguration
			//---------------------------------------------------------------------

			/**
             * @brief	: permet de créer un nouveau profil de configuration
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nom du nouveau profil de conf
             * @Param	: bool : nouvelle config par défaut ou depuis la config courante
             */
			virtual bool createNewConfiguration( QString confName, QString confProfileName, bool fromCurrent );

			/**
             * @brief	: permet de supprimer la configuration courante
             * @Param	: QString : nom de la configuration concernée
             */
			virtual bool deleteConfiguration( QString confName, QString inProfileName);

			/**
             * @brief	: permet de renomer le profil de configuration courant
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nouveau nom du profil de conf
             */
			virtual bool renameConfiguration( QString confName, QString newConfProfileName );

			/**
             * @brief	: permet de changer de configuration courante
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nom du profil de conf à charger
             */
			virtual bool switchConfiguration( QString confName, QString confProfileName);


			/**
             * @brief	: permet de recharger les valeurs des properties de 
             *			  la config courante ou de celle par défaut
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : Groupe de paramètre (préfix) concerné par le restore
             * @Param	: bool : restauration des valeurs par défaut ou courantes?
             */
			virtual bool restoreCancelConfiguration( QString confName, QString parametersConcerned, bool fromDefault, bool isStarlinxRunning);


			/**
             * @brief	: permet de sauver ( createConfigurationFile + writeConfigurationFile)
             *			  un XML file pour la configuration
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QDomDocument : document de conf à sauvegarder
             */
			virtual bool saveConfigurationFile(QString confName, QDomDocument* doc = nullptr);

             /**
             * @brief	: dans le cas autoSave, permet de retenir une propriete qui sera ensuite sauvegardee dans le xml à la fermeture
             * @brief   : a utiliser dans le cas par exemple de la sauvegarde d'une property dont le composant sera delete avant le confCollector a la fermeture du stream
             * @Param	: QString : nom de la configuration concernée, ISwProperty propriete concernée, QString prefix concerné
             */
            virtual void saveOnePropertyOnConf(QString confName, ISwProperty *p, QString propCustomName, QString prefix);


			/**
             * @brief	: permet de créer un XML file pour la configuration
             *			  récupère les valeurs de la conf courante et concatène avec les autres confs
             * @Param	: QString : nom de la configuration concernée
             */
			virtual bool createConfigurationFile( QString confName, QDomDocument &tempDoc );

			/**
             * @brief	: permet de créer un XML file pour un profil d'une conf
             *			  récupère les valeurs de la conf courante 
             * @Param	: QString : nom de la configuration concernée
             */
			virtual void createCurrentConfProfile( QString confName, QDomDocument &tempDoc );

			/**
             * @brief	: permet de sauvegarder les profils de configuration
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QDomDocument : document de conf à sauvegarder
             */
			virtual bool writeConfigurationFile( QString confName, QDomDocument &doc  );

			
			/**
             * @brief	: permet d'importer une configuration
             * @Param	: QString : nom de la configuration concernée
             */
			virtual bool importConfigurationFile (QString confName );


			/**
			* @brief	: permet d'exporter une configuration en XML ( createConfigurationFile + writeConfigurationFile)
             * @Param	: QString : nom de la configuration concernée
             */
			virtual bool exportConfigurationFile (QString confName);


			/**
             * @brief	: permet de récupérer une liste de tous les profils de confs dispo
             * @Param	: QString : nom de la configuration concernée
             * @return	: QList<QString> : nom de la configuration concernée
             */
			virtual QList<QString> getConfigurationProfilesList( QString confName );


			/**
			* @brief : permets de récupérer le nom du profil de la conf courante chargée pour un confName
			* @Param : QString : nom de la configuration concernée
			* @return : QString : nom du profil de la conf courante chargée
			*/
			virtual QString getCurrentProfile(QString confName);

			/**
			* @brief	: permet de récupérer la liste des configurations enregistrées
			* @return	: QList<QString> : liste des configurations
			*/
			QList<QString> getAllConfigurations();

			/**
             * @brief	: permet de récupérer l'interface d'administration de la conf
             * @return	: ISwAdminConfiguration : pointeur sur l'interface d'admin de la conf (ici this)
             */
			virtual ISwAdminConfiguration* getAdmin();

			
			/**
             * @brief	: permet de récupérer l'interface de gestion des properties de la conf
             * @return	: ISwConfigurationManager : pointeur sur l'interface de gestion des properties de la conf
             */
			virtual ISwConfigurationManager* getManager();

			/**
             * @brief	: permet de récupérer un pointeur sur une property
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : prefix du composant ConfCollector
             * @Param	: QString : nom décoré de la property
             * @return	: ISwProperty* : pointeur sur la property
             */
			virtual ISwProperty* getProperty( QString confName, QString prefix, QString decoratedName );


			/**
             * @brief	: permet de récupérer les pointeurs de toutes les properties d'une conf
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nom du groupe paramètres (page) concerné (prefix)
             * @return	: QList <ISwProperty*> : liste des pointeurs sur les properties
             */
			virtual QHash<ISwProperty*, QString> getAllProperties(QString confName, QString prefixName);
			
			/**
             * @brief	: permet de récupérer l'ordre de toutes les properties d'une conf pour les classer dans un treeview
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nom du groupe paramètres (page) concerné (prefix)
             * @return	: QHash<QString, int> : liste des noms décorés des properties et leur ordre dans le treeview
             */
			virtual QHash<QString, int> getAllPropertiesOrder(QString confName, QString prefixName);

			/**
			* @brief	: permet d'updater la valeur d'une property depuis le fichier qui a été loadé
			* @Param	: QString : nom de la configuration concernée
			* @Param	: QString : prefix du composant ConfCollector
             * @Param	: ISwProperty* : pointeur sur la propery concernée
			* @return	: bool : Update réussi ou pas
             */
			virtual bool updateProperty(QString confName, QString prefix, ISwProperty* propToUpdate);


			/**
			* @brief	: permet d'updater la part XML de la configuration par défault (CFM_DEFAULT_FILENAME)
			* @Param	: QString : nom de la configuration concernée
			*/
			virtual bool updateDefaultProfile(QString confName, QHash<QString, QString> inNewDefaultValues = QHash<QString, QString>());


		
		private:

			bool _hasCreatedFactoryFile;

			/** @brief : QHash<confName, ISwConfSaver*> */
			QHash<QString, ISwConfSaver*> _confSavers;

			/** @brief : QHash<confName, QHash<prefix, ISwConfCollector*>> */
			QHash<QString, QHash<QString, ISwConfCollector*>> _confCollectors;

			/** @brief : QHash<confName, currentConfProfileName*> */
			QHash<QString, QString> _currentConfs;

			/** @brief : QHash<confName, loadedOrNot> */
			QHash<QString, bool> _loadedConfs;

			/** @brief : QList<confName> */
			QList<QString> _autoSaveConfs;

			/** @brief : QHash<confName, QHash<confProfileName, confProfileDatas (QDomDocument.toString())>> */
			QHash<QString, QHash<QString, QString>> _confProfilesDatas;

			/** @brief : QHash<confName, QList<confProfileName>> */
			QHash<QString, QList<QString>>	_configsProfilesList;

			/** @brief : Liste des listeners du service pour notif lors de mise à jour d'une conf */
			QList<ISwConfigListener*>	_configurationServiceListeners;

			/** @brief : Liste des observers du service pour notif lors d'une suppression de property */
			QList<ISwPropertiesObserver*>	_configurationPropertiesListeners;

            
            /** @brief permet de stocker les proprietes avec saveOnePropertyOnConf */
            QDomDocument _alreadyDestroyedPropertiesToSaveDoc;
            QDomElement _alreadyDestroyedPropertiesToSaveRootElt;

			

			/**
			* @brief : permets de parser le fichier des confs
			* @Param : QString : nom de la configuration concernée
			* @Param : QString : fichier à parser contenant les données de conf
			* @Param : bool : true s'il n'y avait pas de fichier de config
			*			et qu'il a du être créé pour la premiere fois (need un parse à nouveau)
			*/
			QString parseConfigurationFile(QString confName, QString inConfigFileToParse);

			/**
			* @brief : Crée un profile de conf par défaut (a partir des factory settings)
			* @Param : QString : nom de la configuration concernée
			* @Param : QDomElemment : root_node
			*/
			void createDefaultFromFactory(QString confName, QDomElement &root_node);

			/**
			* @brief : permets de parser le fichier des confs
			* @Param : QString : nom de la configuration concernée
			* @Param : QString : nom du profil de conf pour lequel setter les valeurs des properties
			* @Param : bool : valeur de play du système
			*/
			bool setPropertiesValuesFromProfile( QString confName, QString confProfileName, bool isStarlinxRunning = false, QString parametersConcerned = "all");

			/**
			* @brief : permets de créer un QDom avec les valeurs des properties correspondant à un profil de conf 
			* @Param : QString : nom de la configuration concernée
			* @Param : QDomDocument : CDomDoc concerné
			* @Param : QDomElement : à remplir avec les valeurs des properties des collectors associés
			*/
			void createQDomProfile(QString confName, QDomDocument &doc, QDomElement &elt_config, QHash<QString, QString> inNewDefaultValues = QHash<QString, QString>());

			/**
			* @brief : notifie les listeners du service de configuration
			* @Param : QString : nom de la configuration concernée
			*/
			void notifyServiceListeners(QString confName, bool profilesNotif);

			
			/**
			* @brief : met à jour dans _confProfilesDatas les données des profils de conf
			* @Param : QHash<QString, QHash<QString, QString>>::iterator : itérateur sur la configuration concernée
			* @Param : QString : nom du profil concerné
			* @Param : QDomElement : nouvelles valeurs des datas du profil
			*/
			bool updateConfProfilesDatas(QHash<QString, QHash<QString, QString>>::iterator it_profiles, QString profile, QDomElement newProfileConfDatas);

			bool updateDefaultProfileFromCurrent(QDomNodeList &DefaultElements, QDomNodeList &PropertiesElements, QDomElement &newProfileConfDatas);
		};
    }
}

#endif
