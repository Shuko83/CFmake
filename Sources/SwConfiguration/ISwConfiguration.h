/**
@file ISwConfiguration.h
@brief Interface Configuration
@author G.Blessas
 */

#ifndef _STREAMWORK_SWCONFIGURATION_ISWCONFIGURATION_H
#define _STREAMWORK_SWCONFIGURATION_ISWCONFIGURATION_H


#include "ISwConfigurationListener.h"
#include "Component/Services/ISwProperties.h"
#include "ISwExecution_Service.h"
#include "Component/Services/ISwServiceOwnerConfigurable.h"

namespace StreamWork {

    namespace SwConfiguration {
    
		using namespace StreamWork::SwCore;

        /**
        @class ISwConfiguration
        @brief Interface Configuration
        */
        class ISwConfiguration {

        public:

			virtual void setActivated(bool value) = 0;

			virtual bool isActivated() = 0; 

			virtual ISwProperties * getProperties()  = 0;

            /** @brief ajout d un listener */
            virtual void addListener(ISwConfigurationListener *) = 0;

            /** @brief suppression d un listener */
            virtual void removeListener(ISwConfigurationListener *) = 0;

	        virtual QList<StreamWork::SwCore::ISwServiceOwnerConfigurable *> getServiceOwnerConfigurable() = 0;
	    
			virtual StreamWork::SwExecution::ISwExecution_Service * getExecutionService()=0;
        };

    }
    
}
#endif
