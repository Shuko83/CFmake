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
#include "Properties/ISwProperty.h"


namespace StreamWork
{
	namespace SwCore
	{
		/**
		  @class SwServiceSaveConfiguration
		*/
		class SwServiceSaveConfiguration : public ISwAdminConfiguration,
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

			bool loadConfigurationFile(QString confName, QString confFileToBeLoaded) override;

			/**
			 * @brief	: Permet de récupérer tous les ConfCollectors registered
			 * @Return	: List<QHash<QString, ISwConfCollector*>, liste des pointeurs sur les Prefix/ConfCollectors
			 */
			QHash<QString, ISwConfCollector*> getConfCollectors(QString confName) override;

			/**
			 * @brief	: Permet de récupérer un ConfCollector particulier
			 * @Param	: QString, nom du confCollector voulu
			 * @Param	: QString, prefix du confCollector
			 * @Return	: ISwConfCollector*, pointeur sur le ConfCollector
			 */
			ISwConfCollector* getConfCollector(QString confName, QString prefix) override;

			/**
			 * @brief	: Permet d'enregistrer un ConfCollector
			 * @Param	: QString, nom de la configuration concernée
			 * @Param	: QString, prefix du confCollector
			 * @Param	: ISwConfCollector* pointeur vers le confCollector à registered
			 */
			bool registerConfCollector(QString confName, QString prefix, ISwConfCollector* confCollector, bool autoSave) override;

			/**
			 * @brief	: Permet désenregistrer un ConfCollector
			 * @Param	: QString, nom de la configuration concernée
			 * @Param	: ISwConfCollector* pointeur vers le confCollector à désenregistrer
			 */
			void unregisterConfCollector(QString confName, QString prefix, ISwConfCollector* confCollector) override;

			/**
			 * @brief	: Permet d'enregistrer un IConfSaver (confLoader)
			 * @Param	: QString, nom de la configuration concernée
			 * @Param	: IConfSaver* pointeur vers le confSaver à registered
			 */
			bool registerConfSaver(QString confName, ISwConfSaver* confSaver) override;

			/**
			 * @brief	: Permet désenregistrer un IConfSaver (confLoader)
			 * @Param	: QString, nom de la configuration concernée
			 * @Param	: IConfSaver* pointeur vers le confCollector à désenregistrer
			 */
			void unregisterConfSaver(QString confName) override;

			/**
			 * @brief	: Permet de savoir si une conf a été chargée ou non
			 * @Param	: QString, nom de la configuration concernée
			 * @return	: bool, true si configuration chargée (finalized via le composant ConfLoader)
			 */
			bool isConfLoaded(QString confName) override;


			/**
			 * @brief	: Permet register les listener pour les notifier par la suite
			 * @Param	: ISwConfigListener, listener du service
			 */
			bool registerConfigServiceListener(ISwConfigListener *listener) override;


			/**
			 * @brief	: Permet de désenregister les listeners
			 * @Param	: ISwConfigListener, listener du service
			 */
			void unregisterConfigServiceListener(ISwConfigListener *listener) override;


			/**
			 * @brief	: Permet de vider les Maps du service de conf
			 */
			void clearConfService() override;



			//---------------------------------------------------------------------
			// Interface ISwConfigurationManager
			//---------------------------------------------------------------------
			/**
			* @brief	: Permet d'enregistrer un ConfPropertiesObserver
			* @Param	: ISwPropertiesObserver* observer des properties
			*/
			bool registerConfPropertiesObserver(ISwPropertiesObserver * observer) override;

			/**
			* @brief	: Permet désenregistrer un ConfPropertiesObserver
			* @Param	: ISwPropertiesObserver* observer des properties
			*/
			void unregisterConfPropertiesObserver(ISwPropertiesObserver * observer) override;



			//---------------------------------------------------------------------
			// Interface ISwPropertiesObserver
			//---------------------------------------------------------------------
			/** @brief : Fonction appelée par les confcollectors lors du delete d'une property */
			void notifyPropertyDeleted(ISwProperty * propertyDeleted, QString propertyDecoratedName, QString confName = "") override;


			//---------------------------------------------------------------------
			// Interface ISwServiceConfiguration
			//---------------------------------------------------------------------

			/**
			 * @brief	: permet de créer un nouveau profil de configuration
			 * @Param	: QString : nom de la configuration concernée
			 * @Param	: QString : nom du nouveau profil de conf
			 * @Param	: bool : nouvelle config par défaut ou depuis la config courante
			 */
			bool createNewConfiguration(QString confName, QString confProfileName, bool fromCurrent) override;

			/**
			 * @brief	: permet de supprimer la configuration courante
			 * @Param	: QString : nom de la configuration concernée
			 */
			bool deleteConfiguration(QString confName, QString inProfileName) override;

			/**
			 * @brief	: permet de renomer le profil de configuration courant
			 * @Param	: QString : nom de la configuration concernée
			 * @Param	: QString : nouveau nom du profil de conf
			 */
			bool renameConfiguration(QString confName, QString newConfProfileName) override;

			/**
			 * @brief	: permet de changer de configuration courante
			 * @Param	: QString : nom de la configuration concernée
			 * @Param	: QString : nom du profil de conf à charger
			 */
			bool switchConfiguration(QString confName, QString confProfileName) override;

			/**
			 * @brief	: Permet de savoir si le profil courant chargé pour la configuration dont le nom est
						  passé en paramètres contient au moins une propriété ayant une valeur non usine.
			 * @Param	: QString : nom de la configuration concernée
			 */
			bool hasNonDefaultPropertyValuesInCurrentConfigProfile(const QString& configName, const QString& prefix = "all") override;

			/**
			 * @brief	: permet de recharger les valeurs des properties de
			 *			  la config courante ou de celle par défaut
			 * @Param	: QString : nom de la configuration concernée
			 * @Param	: QString : Groupe de paramètre (préfix) concerné par le restore
			 * @Param	: bool : restauration des valeurs par défaut ou courantes?
			 */
			bool restoreCancelConfiguration(QString confName, QString parametersConcerned, bool fromDefault, bool isStarlinxRunning) override;


			/**
			 * @brief	: permet de sauver ( createConfigurationFile + writeConfigurationFile)
			 *			  un XML file pour la configuration
			 * @Param	: QString : nom de la configuration concernée
			 * @Param	: QDomDocument : document de conf à sauvegarder
			 */
			bool saveConfigurationFile(QString confName, QDomDocument* doc = nullptr) override;

			/**
			* @brief	: dans le cas autoSave, permet de retenir une propriete qui sera ensuite sauvegardee dans le xml à la fermeture
			* @brief   : a utiliser dans le cas par exemple de la sauvegarde d'une property dont le composant sera delete avant le confCollector a la fermeture du stream
			* @Param	: QString : nom de la configuration concernée, ISwProperty propriete concernée, QString prefix concerné
			*/
			void saveOnePropertyOnConf(QString confName, ISwProperty *p, QString propCustomName, QString prefix) override;


			/**
			 * @brief	: permet de créer un XML file pour la configuration
			 *			  récupère les valeurs de la conf courante et concatène avec les autres confs
			 * @Param	: QString : nom de la configuration concernée
			 */
			bool createConfigurationFile(QString confName, QDomDocument &tempDoc) override;

			/**
			 * @brief	: permet de créer un XML file pour un profil d'une conf
			 *			  récupère les valeurs de la conf courante
			 * @Param	: QString : nom de la configuration concernée
			 */
			virtual void createCurrentConfProfile(QString confName, QDomDocument &tempDoc);

			/**
			 * @brief	: permet de sauvegarder les profils de configuration
			 * @Param	: QString : nom de la configuration concernée
			 * @Param	: QDomDocument : document de conf à sauvegarder
			 */
			bool writeConfigurationFile(QString confName, QDomDocument &doc) override;


			/**
			 * @brief	: permet d'importer une configuration
			 * @Param	: QString : nom de la configuration concernée
			 */
			bool importConfigurationFile(QString confName) override;


			/**
			* @brief	: permet d'exporter une configuration en XML ( createConfigurationFile + writeConfigurationFile)
			 * @Param	: QString : nom de la configuration concernée
			 */
			bool exportConfigurationFile(QString confName) override;


			/**
			 * @brief	: permet de récupérer une liste de tous les profils de confs dispo
			 * @Param	: QString : nom de la configuration concernée
			 * @return	: QList<QString> : nom de la configuration concernée
			 */
			QList<QString> getConfigurationProfilesList(QString confName) override;


			/**
			* @brief : permets de récupérer le nom du profil de la conf courante chargée pour un confName
			* @Param : QString : nom de la configuration concernée
			* @return : QString : nom du profil de la conf courante chargée
			*/
			QString getCurrentProfile(const QString& confName) override;

			/**
			* @brief	: permet de récupérer la liste des configurations enregistrées
			* @return	: QList<QString> : liste des configurations
			*/
			QList<QString> getAllConfigurations() override;

			/**
			 * @brief	: permet de récupérer l'interface d'administration de la conf
			 * @return	: ISwAdminConfiguration : pointeur sur l'interface d'admin de la conf (ici this)
			 */
			ISwAdminConfiguration* getAdmin() override;


			/**
			 * @brief	: permet de récupérer l'interface de gestion des properties de la conf
			 * @return	: ISwConfigurationManager : pointeur sur l'interface de gestion des properties de la conf
			 */
			ISwConfigurationManager* getManager() override;

			/**
			 * @brief	: permet de récupérer un pointeur sur une property
			 * @Param	: QString : nom de la configuration concernée
			 * @Param	: QString : prefix du composant ConfCollector
			 * @Param	: QString : nom décoré de la property
			 * @return	: ISwProperty* : pointeur sur la property
			 */
			ISwProperty* getProperty(QString confName, QString prefix, QString decoratedName) override;


			/**
			 * @brief	: permet de récupérer les pointeurs de toutes les properties d'une conf
			 * @Param	: QString : nom de la configuration concernée
			 * @Param	: QString : nom du groupe paramètres (page) concerné (prefix)
			 * @return	: QList <ISwProperty*> : liste des pointeurs sur les properties
			 */
			QHash<ISwProperty*, QString> getAllProperties(const QString& confName, const QString& prefixName) override;

			/**
			 * @brief	: permet de récupérer l'ordre de toutes les properties d'une conf pour les classer dans un treeview
			 * @Param	: QString : nom de la configuration concernée
			 * @Param	: QString : nom du groupe paramètres (page) concerné (prefix)
			 * @return	: QHash<QString, int> : liste des noms décorés des properties et leur ordre dans le treeview
			 */
			QHash<QString, int> getAllPropertiesOrder(QString confName, QString prefixName) override;

			/**
			* @brief	: permet d'updater la valeur d'une property depuis le fichier qui a été loadé
			* @Param	: QString : nom de la configuration concernée
            * @Param	: ISwProperty* : pointeur sur la propery concernée
			* @return	: bool : Update réussi ou pas
            */
			bool updateProperty(QString confName, ISwProperty* propToUpdate) override;


			/**
			* @brief	: permet d'updater la part XML de la configuration par défault (CFM_DEFAULT_FILENAME)
			* @Param	: QString : nom de la configuration concernée
			*/
			bool updateDefaultProfile(QString confName, QHash<QString, QString> inNewDefaultValues = QHash<QString, QString>()) override;



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
			* @brief : permets de mettre à jour la factory conf à partir du stream
			* @Param : QString : nom de la configuration concernée
			* @Param : QString : contenu à mettre à jour avec le nouveau "factory setting"
			*/
			QString updateFactorySetting(QString confName, QString confFileContent);

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
			bool setPropertiesValuesFromProfile(QString confName, QString confProfileName, bool isStarlinxRunning = false, QString parametersConcerned = "all");

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
