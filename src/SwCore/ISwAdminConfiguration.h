
/**
@file ISwAdminConfiguration.h
@brief Service permettant d'administration du service de gestion de la configuration
@author CGD
 */

#ifndef _STREAMWORK_SWCORE_ISWADMINCONFIGURATION_H
#define _STREAMWORK_SWCORE_ISWADMINCONFIGURATION_H


#include "ISwConfSaver.h"
#include "ISwConfCollector.h"
#include "ISwConfigListener.h"

namespace StreamWork 
{
    namespace SwCore 
    {
        /**
         *  @class ISwAdminConfiguration
         *  @brief Service permettant d'administration du service de gestion de la configuration
		 *  @example
         */
        class ISwAdminConfiguration  
        {
        public:
            /**
             * @brief	: permet de charger un fichier xml de configuration
             * @Param	: QString, nom de la configuration concernée
             * @Param	: QString, data du fichier de configuration
             */
			virtual void loadConfigurationFile (QString confName, QString confFileToBeLoaded) = 0;

            /**
             * @brief	: Permet de récupérer tous les ConfCollectors registered
             * @Return	: List<ISwConfCollector*>, liste des pointeurs sur les ConfCollectors
             */
            virtual QHash<QString, ISwConfCollector*> getConfCollectors (QString confName) = 0;

			/**
             * @brief	: Permet de récupérer un ConfCollector particulier
             * @Param	: QString, nom du confCollector voulu
             * @Param	: QString, prefix du confCollector
             * @Return	: ISwConfCollector*, pointeur sur le ConfCollector 
             */
            virtual ISwConfCollector* getConfCollector (QString confName, QString prefix) = 0;

			/**
             * @brief	: Permet d'enregistrer un ConfCollector
             * @Param	: QString, nom de la configuration concernée
             * @Param	: QString, prefix du confCollector
             * @Param	: ISwConfCollector* pointeur vers le confCollector à registered
             */
            virtual void registerConfCollector (QString confName, QString prefix, ISwConfCollector* confCollector) = 0;

			/**
             * @brief	: Permet désenregistrer un ConfCollector
             * @Param	: QString, nom de la configuration concernée
             * @Param	: ISwConfCollector* pointeur vers le confCollector à désenregistrer
             */
            virtual void unregisterConfCollector (QString confName) = 0;

			/**
             * @brief	: Permet d'enregistrer un IConfSaver (confLoader)
             * @Param	: QString, nom de la configuration concernée
             * @Param	: IConfSaver* pointeur vers le confSaver à registered
             */
            virtual void registerConfSaver (QString confName, ISwConfSaver* confSaver) = 0;

			/**
             * @brief	: Permet désenregistrer un IConfSaver (confLoader)
             * @Param	: QString, nom de la configuration concernée
             * @Param	: IConfSaver* pointeur vers le confCollector à désenregistrer
             */
            virtual void unregisterConfSaver (QString confName) = 0;

			/**
             * @brief	: Permet de savoir si une conf a été chargée ou non
             * @Param	: QString, nom de la configuration concernée
             * @return	: bool, true si configuration chargée (finalized via le composant ConfLoader)
             */
            virtual bool isConfLoaded (QString confName) = 0;

			/**
             * @brief	: Permet register les listener pour les notifier par la suite
             * @Param	: ISwConfigListener, listener du service    
             */
            virtual void registerConfigServiceListener (ISwConfigListener *listener) = 0;

			/**
             * @brief	: Permet de désenregister les listeners
             * @Param	: ISwConfigListener, listener du service    
             */
            virtual void unregisterConfigServiceListener (ISwConfigListener *listener) = 0;
  
        };
    }
}

#endif
