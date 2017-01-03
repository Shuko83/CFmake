/**
	@file : ISwServiceConfiguration.h
	@brief : Interface du service permettant la sauvegarde de la configuration Starlinx
	@author : CGD
 */

#ifndef _STREAMWORK_SWCORE_ISWSERVICECONFIGURATION_H
#define _STREAMWORK_SWCORE_ISWSERVICECONFIGURATION_H

#include "ISwService.h"
#include "ISwAdminConfiguration.h"
#include "ISwPropertiesObserver.h"
#include <QDomDocument>

//Nom du service
#define CG_SW_SERVICE_SAVECONFIGURATION "ServiceConfiguration"



namespace StreamWork 
{
    namespace SwCore 
    {
        /**
         *  @class ISwServiceConfiguration
         *  @brief Service permettant la sauvegarde de la configuration Starlinx
		 *  @example
		 *  		ISwServiceConfiguration * timer = dynamic_cast<ISwServiceConfiguration*>(SW_APP->QueryService(CG_SW_SERVICE_SAVECONFIGURATION));
         */
        class ISwServiceConfiguration  : public ISwService 
        {
        public:
           
			/**
             * @brief	: permet de créer un nouveau profil de configuration
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nom du nouveau profil de conf
             * @Param	: bool : nouvelle config par défaut ou depuis la config courante
             */
			virtual bool createNewConfiguration (QString confName, QString confProfileName, bool fromCurrent) = 0;


            /**
             * @brief	: permet de supprimer la configuration courante
             * @Param	: QString : nom de la configuration concernée
             */
			virtual bool deleteConfiguration (QString confName, QString inProfileName) = 0;


			/**
             * @brief	: permet de renomer le profil de configuration courant
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nouveau nom du profil de conf
             */
			virtual bool renameConfiguration (QString confName, QString newConfProfileName) = 0;


			/**
             * @brief	: permet de changer de configuration courante
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nom du profil de conf à charger
             */
			virtual bool switchConfiguration( QString confName, QString confProfileName) = 0;
			

			/**
             * @brief	: permet de recharger les valeurs des properties de 
             *			  la config courante ou de celle par défaut
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : Groupe de paramètre (préfix) concerné par le restore
             * @Param	: bool : restauration des valeurs par défaut ou courantes?
             */
			virtual bool restoreCancelConfiguration( QString confName, QString parametersConcerned, bool fromDefault, bool isStarlinxRunning) = 0;


			/**
             * @brief	: permet de sauver ( createConfigurationFile + writeConfigurationFile)
             *			  un XML file pour la configuration
             * @Param	: QString : nom de la configuration concernée
             */
            virtual bool saveConfigurationFile(QString confName) = 0;
	        
             /**
             * @brief	: dans le cas autoSave, permet de retenir une propriete qui sera ensuite sauvegardee dans le xml à la fermeture
             * @brief   : a utiliser dans le cas par exemple de la sauvegarde d'une property dont le composant sera delete avant le confCollector a la fermeture du stream
             * @Param	: QString : nom de la configuration concernée, ISwProperty propriete concernée, QString prefix concerné
             */
            virtual void saveOnePropertyOnConf(QString confName, ISwProperty *p, QString propCustomName, QString prefix) = 0;

			/**
             * @brief	: permet de créer un XML file pour la configuration
             *			  récupère les valeurs de la conf courante et concatène avec les autres confs
             * @Param	: QString : nom de la configuration concernée
             */
			virtual bool createConfigurationFile( QString confName, QDomDocument &doc) = 0;


			/**
             * @brief	: permet de sauvegarder les profils de configuration
             * @Param	: QString : nom de la configuration concernée
             */
			virtual bool writeConfigurationFile (QString confName, QDomDocument &doc ) = 0;


			/**
             * @brief	: permet d'importer une configuration
             * @Param	: QString : nom de la configuration concernée
             */
			virtual bool importConfigurationFile (QString confName ) = 0;


			/**
			* @brief	: permet d'exporter une configuration en XML ( createConfigurationFile + writeConfigurationFile)
             * @Param	: QString : nom de la configuration concernée
             */
			virtual bool exportConfigurationFile (QString confName) = 0;


			/**
             * @brief	: permet de récupérer une liste de tous les profils de confs dispo
             * @Param	: QString : nom de la configuration concernée
             * @return	: QList<QString> : nom de la configuration concernée
             */
			virtual QList<QString> getConfigurationProfilesList (QString confName) = 0;

			/**
             * @brief	: permet de récupérer le profil de conf courant pour une conf donnée
             * @Param	: QString : nom de la configuration concernée
             * @return	: QString : nom de la configuration courante
             */
			virtual QString getCurrentProfile(QString confName) = 0;

			/**
			* @brief	: permet de récupérer la liste des configurations enregistrées
			* @return	: QList<QString> : liste des configurations
			*/
			virtual QList<QString> getAllConfigurations() = 0;

			/**
             * @brief	: permet de récupérer l'interface d'administration de la conf
             * @return	: ISwAdminConfiguration : pointeur sur l'interface d'admin de la conf
             */
			virtual ISwAdminConfiguration* getAdmin () = 0;

			/**
             * @brief	: permet de récupérer l'interface de gestion des properties de la conf
             * @return	: ISwPropertiesObserver : pointeur sur l'interface de gestion des properties de la conf
             */
			virtual ISwPropertiesObserver* getConfPropertiesObserver() = 0;

			
			/**
             * @brief	: permet de récupérer un pointeur sur une property
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : prefix du composant ConfCollector
             * @Param	: QString : nom de la property
             * @return	: ISwProperty* : pointeur sur la property
             */
			virtual ISwProperty* getProperty (QString confName, QString prefix, QString propertyName) = 0;

			/**
             * @brief	: permet de récupérer les pointeurs de toutes les properties d'une conf
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nom du groupe paramètres (page) concerné (prefix)
             * @return	: QHash<ISwProperty*, QString> : liste des pointeurs sur les properties et leur prefix associé
             */
			virtual QHash<ISwProperty*, QString> getAllProperties(QString confName, QString prefixName) = 0;

			/**
             * @brief	: permet de récupérer l'ordre de toutes les properties d'une conf pour les classer dans un treeview
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nom du groupe paramètres (page) concerné (prefix)
             * @return	: QHash<QString, int> : liste des noms décorés des properties et leur ordre dans le treeview
             */
			virtual QHash<QString, int> getAllPropertiesOrder(QString confName, QString prefixName) = 0;
            
			/**
			* @brief	: permet d'updater la valeur d'une property depuis le fichier qui a été loadé
			* @Param	: QString : nom de la configuration concernée
			* @Param	: QString : prefix du composant ConfCollector
			* @Param	: ISwProperty* : pointeur sur la propery concernée
			* @return	: bool : Update réussi ou pas
			*/
			virtual bool updateProperty(QString confName, QString prefix, ISwProperty* propToUpdate) = 0;


			/**
			* @brief	: permet d'updater la part XML de la configuration par défault (CFM_DEFAULT_FILENAME)
			* @Param	: QString : nom de la configuration concernée
			*/
			virtual bool updateDefaultProfile(QString confName, QHash<QString, QString> inNewDefaultValues = QHash<QString, QString>()) = 0;


            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------

            /**
			 * @brief	: Est appele uniquement par le service manager aupres duquel le service est enregistré
						  lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée
             */
            virtual void Liberate(){/*Rien a faire*/}   

            /**
             * @brief	: Renvoie le nom du service
             * @return	: QString - le nom du service
             */
            QString GetServiceName(){return QString(CG_SW_SERVICE_SAVECONFIGURATION); }

            /**
             * @brief	: Renvoie le nom du service reel (le nom de l'interface)
             * @return	: QString - le nom du service reel (le nom de l'interface)
             * @note	: au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()
             */
            QString GetServiceRealName() {return QString(typeid(this).name());}   
        };
	}
}

#endif
