/**
@file ISwServiceConfiguration.h
@brief Interface du service permettant la sauvegarde de la configuration Starlinx
@author CGD
 */

#ifndef _STREAMWORK_SWCORE_ISWSERVICECONFIGURATION_H
#define _STREAMWORK_SWCORE_ISWSERVICECONFIGURATION_H

#include "ISwService.h"
#include "ISwAdminConfiguration.h"
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
			virtual void createNewConfiguration (QString confName, QString confProfileName, bool fromCurrent) = 0;

            /**
             * @brief	: permet de supprimer la configuration courante
             * @Param	: QString : nom de la configuration concernée
             */
			virtual void deleteConfiguration (QString confName) = 0;

			/**
             * @brief	: permet de renomer le profil de configuration courant
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nouveau nom du profil de conf
             */
			virtual void renameConfiguration (QString confName, QString newConfProfileName) = 0;

			/**
             * @brief	: permet de changer de configuration courante
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : nom du profil de conf à charger
             */
			virtual void switchConfiguration( QString confName, QString confProfileName) = 0;

			

			/**
             * @brief	: permet de recharger les valeurs des properties de 
             *			  la config courante ou de celle par défaut
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : Groupe de paramètre (préfix) concerné par le restore
             * @Param	: bool : restauration des valeurs par défaut ou courantes?
             */
			virtual void restoreCancelConfig( QString confName, QString parametersConcerned, bool fromDefault) = 0;

			

			/**
             * @brief	: permet de sauver ( createConfigurationFile + writeConfigurationFile)
             *			  un XML file pour la configuration
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QDomDocument : document de conf à sauvegarder
             */
			virtual void saveConfigurationFile( QString confName ) = 0;


			/**
             * @brief	: permet de créer un XML file pour la configuration
             *			  récupère les valeurs de la conf courante et concatène avec les autres confs
             * @Param	: QString : nom de la configuration concernée
             */
			virtual void createConfigurationFile( QString confName, QDomDocument &doc) = 0;


			/**
             * @brief	: permet de sauvegarder les profils de configuration
             * @Param	: QString : nom de la configuration concernée
             */
			virtual void writeConfigurationFile (QString confName, QDomDocument &doc ) = 0;


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
			virtual QString getCurrentConf(QString confName) = 0;

			/**
             * @brief	: permet de récupérer l'interface d'administration de la conf
             * @return	: ISwAdminConfiguration : pointeur sur l'interface d'admin de la conf
             */
			virtual ISwAdminConfiguration* getAdmin () = 0;

			
			/**
             * @brief	: permet de récupérer un pointeur sur une property
             * @Param	: QString : nom de la configuration concernée
             * @Param	: QString : prefix du composant ConfCollector
             * @Param	: QString : nom de la property
             * @return	: ISwProperty* : pointeur sur la property
             */
			virtual ISwProperty* getProperty (QString confName, QString prefix, QString propertyName) = 0;
            

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
