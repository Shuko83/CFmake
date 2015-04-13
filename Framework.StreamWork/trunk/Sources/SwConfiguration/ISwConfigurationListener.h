/**
@file ISwConfigurationListener.h
@brief Interface Configuration
@author G.Blessas
 */

#ifndef _STREAMWORK_SWCONFIGURATION_ISWCONFIGURATIONLISTENER_H
#define _STREAMWORK_SWCONFIGURATION_ISWCONFIGURATIONLISTENER_H

namespace StreamWork {

    namespace SwConfiguration {
    
        class ISwConfiguration;
        /**
        @class ISwConfigurationListener
        @brief Interface Configuration Listener
        */
        class ISwConfigurationListener {

        public:
            
            /** @brief sur changement des propriétés*/
            virtual void OnPropertiesChanged(ISwConfiguration *)=0;

            /** @brief sur changement des propriétés*/
            virtual void OnActivate(ISwConfiguration *)=0;

            /** @brief sur changement des propriétés*/
            virtual void OnDisable(ISwConfiguration *)=0;

            /** @brief sur changement des propriétés*/
            //virtual void OnRecordAvailableChanged(ISwConfiguration *)=0;
        };

    }
    
}
#endif
