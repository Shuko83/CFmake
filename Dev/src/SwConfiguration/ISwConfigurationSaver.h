/**
@file ISwConfigurationSaver.h
@brief Interface ISwConfigurationSaver
@author 
 */

#ifndef _STREAMWORK_SWCONFIGURATION_ISWCONFIGURATIONSAVER_H
#define _STREAMWORK_SWCONFIGURATION_ISWCONFIGURATIONSAVER_H



namespace StreamWork {

    namespace SwConfiguration {
    
		using namespace StreamWork::SwCore;

        /**
        @class ISwConfiguration
        @brief Interface Configuration
        */
        class ISwConfigurationSaver {

        public:

            virtual ~ISwConfigurationSaver() {}
			virtual bool SaveConfiguration(QString filename) = 0;
			

        };

    }
    
}
#endif
