/**
@file ISwConfigurationControllerListener.h
@brief Interface Configuration Controller Listener
@author G.Blessas
 */

#ifndef _STREAMWORK_SWCONFIGURATION_ISWCONFIGURATIONCONTROLLERLISTENER_H
#define _STREAMWORK_SWCONFIGURATION_ISWCONFIGURATIONCONTROLLERLISTENER_H

namespace StreamWork {

    namespace SwConfiguration {
    
        class ISwConfigurationController;
        /**
        @class ISwConfigurationControllerListener
        @brief Interface Configuration Controller Listener
        */
        class ISwConfigurationControllerListener
        {

        public:

            virtual ~ISwConfigurationControllerListener(){};

            /** @brief sur changement d etat*/
            virtual void OnStartedChanged(ISwConfigurationController *)=0;

            /** @brief sur changement d etat*/
            virtual void OnConfigurationEditedChanged(ISwConfigurationController *)=0;
        };

    }
    
}
#endif
