/**
@file ISwServicesManager_Listener.h
@brief Observer d'un manager de service
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWCORE_ISWSERVICESMANAGER_LISTENER_H
#define _STREAMWORK_SWCORE_ISWSERVICESMANAGER_LISTENER_H

#include "ISwService.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class ISwServicesManager_Listener
        @brief Observer d'un manager de service
        */
        class ISwServicesManager_Listener {

        public:
	        /*! \brief sur ajout d'un service */
	        virtual void OnRegisterService(ISwService * service)=0;            
	        /*! \brief sur suppression d'une  interface */
	        virtual void OnUnregisterService(ISwService * service)=0;            
        };

    }
    
}
#endif
