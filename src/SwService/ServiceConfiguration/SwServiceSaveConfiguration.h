/**
  @file : SwServiceSaveConfiguration.h
  @brief : Service permettant de gérer la sauvegarde de la conf
  @author : CGD
 */

#ifndef _STREAMWORK_SWCORE__SWSERVICESAVECONFIGURATION_H
#define _STREAMWORK_SWCORE__SWSERVICESAVECONFIGURATION_H

#include "ISwAdminConfiguration.h"
#include "ISwServiceConfiguration.h"
#include "ISwConfPropertiesObserver.h"
#include "ISwConfigListener.h"

#include <QDomElement>


namespace StreamWork 
{
    namespace SwCore 
    {
        /**
          @class SwServiceSaveConfiguration
        */
        class SwServiceSaveConfiguration :  public QObject,
											public ISwAdminConfiguration, 
											public ISwServiceConfiguration ,
											public ISwConfPropertiesObserver
        {
			Q_OBJECT
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
             * @Return	: List<ISwConfCollector*>, liste des pointeurs sur les ConfCollectors
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
             * @brief	: Permet d'enregistrer un ConfPropertiesObserver
             * @Param	: ISwConfPropertiesObserver* observer des properties
             */
			virtual bool registerConfPropertiesObserver(  ISwConfPropertiesObserver * observer  );

			/**
             * @brief	: Permet désenregistrer un ConfPropertiesObserver
             * @Param	: ISwConfPropertiesObserver* observer des properties
             */
			virtual void unregisterConfPropertiesObserver(  ISwConfPropertiesObserver * observer );



			/**
             * @brief	: Permet de vider les Maps du service de conf
             */
			virtual void clearConfService();



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
			virtual bool saveConfigurationFile( QString confName);


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
			virtual QString getCurrentConf( QString confName);

			/**
             * @brief	: permet de récupérer l'interface d'administration de la conf
             * @return	: ISwAdminConfiguration : pointeur sur l'interface d'admin de la conf (ici this)
             */
			virtual ISwAdminConfiguration* getAdmin();

			
			/**
             * @brief	: permet de récupérer l'interface de gestion des properties de la conf
             * @return	: ISwConfPropertiesObserver : pointeur sur l'interface de gestion des properties de la conf
             */
			virtual ISwConfPropertiesObserver* getConfPropertiesObserver();

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
             * @return	: QList <ISwProperty*> : liste des pointeurs sur les properties
             */
			virtual QHash<ISwProperty*, QString> getAllProperties (QString confName);
			
			/**
             * @brief	: permet de récupérer l'ordre de toutes les properties d'une conf pour les classer dans un treeview
             * @Param	: QString : nom de la configuration concernée
             * @return	: QHash<QString, int> : liste des noms décorés des properties et leur ordre dans le treeview
             */
			virtual QHash<QString, int> getAllPropertiesOrder (QString confName);

			/**
             * @brief	: permet de récupérer l'arborescence d'une property d'après son pointeur
             * @Param	: ISwProperty* : pointeur sur la propery concernée
             * @return	: QString : Nom de la property concernée
             */
			//virtual QString getConstructedNameForProperty( ISwProperty* property);


			//---------------------------------------------------------------------
			// Interface ISwConfPropertiesObserver
			//---------------------------------------------------------------------
			/** @brief : Fonction appelée par les confcollectors lors du delete d'une property */
			virtual void onPropertyDeleted( ISwProperty * propertyDeleted, QString propertyDecoratedName, QString confName );

		
		private:

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
			QList<ISwConfPropertiesObserver*>	_configurationPropertiesListeners;
			

			/**
			* @brief : permets de parser le fichier des confs
			* @Param : QString : nom de la configuration concernée
			* @Param : QString : fichier à parser contenant les données de conf
			* @Param : bool : true s'il n'y avait pas de fichier de config 
			*			et qu'il a du être créé pour la premiere fois (need un parse à nouveau)
			*/
			QString parseConfigurationFile(QString confName, QString inConfigFileToParse);

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
			void createQDomProfile(QString confName, QDomDocument &doc, QDomElement &elt_config);

			/**
			* @brief : notifie les listeners du service de configuration
			* @Param : QString : nom de la configuration concernée
			*/
			void notifyServiceListeners(QString confName, bool profilesNotif);

			public slots:
				void onQuit();
		};
    }
}

#endif
