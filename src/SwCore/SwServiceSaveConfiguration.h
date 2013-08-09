
/**
@file SwServiceSaveConfiguration.h
@brief Service permettant de gérer la sauvegarde de la conf
@author CGD
 */

#ifndef _STREAMWORK_SWCORE__SWSERVICESAVECONFIGURATION_H
#define _STREAMWORK_SWCORE__SWSERVICESAVECONFIGURATION_H



// includes locaux
#include "ISwAdminConfiguration.h"
#include "ISwServiceConfiguration.h"
#include "ISwConfigListener.h"

// includes globaux
#include <QDomElement>



namespace StreamWork 
{
    namespace SwCore 
    {
        /**
        @class SwServiceSaveConfiguration
        */
        class SwServiceSaveConfiguration :  public ISwAdminConfiguration, 
											public ISwServiceConfiguration 
        {
        public:
            /** @brief Constructor */
            SwServiceSaveConfiguration();

            /** @brief Destructor */
            virtual ~SwServiceSaveConfiguration();

			//---------------------------------------------------------------------
			// Interface ISwAdminConfiguration
			//---------------------------------------------------------------------
			
			virtual void loadConfigurationFile( QString confName, QString confFileToBeLoaded );

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
			virtual void registerConfCollector( QString confName, QString prefix, ISwConfCollector* confCollector );
			
			/**
             * @brief	: Permet désenregistrer un ConfCollector
             * @Param	: QString, nom de la configuration concernée
             * @Param	: ISwConfCollector* pointeur vers le confCollector à désenregistrer
             */
			virtual void unregisterConfCollector( QString confName);

			/**
             * @brief	: Permet d'enregistrer un IConfSaver (confLoader)
             * @Param	: QString, nom de la configuration concernée
             * @Param	: IConfSaver* pointeur vers le confSaver à registered
             */
			virtual void registerConfSaver( QString confName, ISwConfSaver* confSaver );

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
			virtual void registerConfigServiceListener( ISwConfigListener *listener );


			/**
             * @brief	: Permet de désenregister les listeners
             * @Param	: ISwConfigListener, listener du service    
             */
            virtual void unregisterConfigServiceListener (ISwConfigListener *listener);


			//---------------------------------------------------------------------
			// Interface ISwServiceConfiguration
			//---------------------------------------------------------------------

			/**
             * @brief	: permet de créer un nouveau profil de configuration
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nom du nouveau profil de conf
             * @Param	: bool : nouvelle config par défaut ou depuis la config courante
             */
			virtual void createNewConfiguration( QString confName, QString confProfileName, bool fromCurrent );

			/**
             * @brief	: permet de supprimer la configuration courante
             * @Param	: QString : nom de la configuration concernée
             */
			virtual void deleteConfiguration( QString confName );

			/**
             * @brief	: permet de renomer le profil de configuration courant
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nouveau nom du profil de conf
             */
			virtual void renameConfiguration( QString confName, QString newConfProfileName );

			/**
             * @brief	: permet de changer de configuration courante
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nom du profil de conf à charger
             */
			virtual void switchConfiguration( QString confName, QString confProfileName);



			/**
             * @brief	: permet de recharger les valeurs des properties de 
             *			  la config courante ou de celle par défaut
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : Groupe de paramètre (préfix) concerné par le restore
             * @Param	: bool : restauration des valeurs par défaut ou courantes?
             */
			virtual void restoreCancelConfig( QString confName, QString parametersConcerned, bool fromDefault);


			/**
             * @brief	: permet de sauver ( createConfigurationFile + writeConfigurationFile)
             *			  un XML file pour la configuration
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QDomDocument : document de conf à sauvegarder
             */
			virtual void saveConfigurationFile( QString confName);


			/**
             * @brief	: permet de créer un XML file pour la configuration
             *			  récupère les valeurs de la conf courante et concatène avec les autres confs
             * @Param	: QString : nom de la configuration concernée
             */
			virtual void createConfigurationFile( QString confName, QDomDocument &tempDoc );


			/**
             * @brief	: permet de sauvegarder les profils de configuration
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QDomDocument : document de conf à sauvegarder
             */
			virtual void writeConfigurationFile( QString confName, QDomDocument &doc  );


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
             * @brief	: permet de récupérer un pointeur sur une property
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : prefix du composant ConfCollector
             * @Param	: QString : nom décoré de la property
             * @return	: ISwProperty* : pointeur sur la property
             */
			virtual ISwProperty* getProperty( QString confName, QString prefix, QString decoratedName );

			
		private:

			/** @brief : QHash<confName, ISwConfSaver*> */
			QHash<QString, ISwConfSaver*> _confSavers;

			/** @brief : QHash<confName, QHash<prefix, ISwConfCollector*>> */
			QHash<QString, QHash<QString, ISwConfCollector*>> _confCollectors;

			/** @brief : QHash<confName, currentConfProfileName*> */
			QHash<QString, QString> _currentConfs;

			/** @brief : QHash<confName, loadedOrNot> */
			QHash<QString, bool> _loadedConfs;

			/** @brief : QHash<confName, QHash<confProfileName, confProfileDatas*>> */
			QHash<QString, QHash<QString, QString>> _confProfilesDatas;

			/** @brief : QHash<confName, QList<confProfileName>> */
			QHash<QString, QList<QString>>	_configsProfilesList;

			/** @brief : Liste des listeners du service pour notif lors de mise à jour d'une conf */
			QList<ISwConfigListener*>	_configurationServiceListeners;


			

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
			*/
			void setPropertiesValues( QString confName, QString confProfileName );

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
			void notifyLiteners(QString confName);

        };
    }
}

#endif
