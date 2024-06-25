/**
@file SwExtensionHelper.h
@brief Extension helper for easy management
@author Big
 */

#ifndef _STREAMWORK_SWCORE_SWEXTENSIONHELPER_H
#define _STREAMWORK_SWCORE_SWEXTENSIONHELPER_H

#include "Component/Base/SwComponent_Class.h"
#include "Extensions/ISwExtensionPoint.h"
#include "Extensions/ISwExtension.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class SwExtensionHelper
        @brief Extension helper for easy management
        */
        class BUILD_SWCORE SwExtensionHelper {
        public:
            /** @brief Constructor */
            SwExtensionHelper(SwComponent_Class * component);
            /** @brief Destructor */
            virtual ~SwExtensionHelper();
            /** @brief Renvoie une liste des points d'extension a un instant t, les objects doivent etre liberé apres usages */
            QList<ISwExtensionPoint *> getAllExtensionPoints();  
            /** @brief Renvoie un point d'extension donnée */
            ISwExtensionPoint * getExtensionPoint(QString type,QString name);
            /** @brief Renvoie une liste des extension a un instant t, les objects doivent etre liberé apres usages */
            QList<ISwExtension *> getAllExtensions();      
            /** @brief Renvoie une extension donnée */
            ISwExtension * getExtension(QString type,QString name);
        private:
            SwComponent_Class * _component;
        
        };
    
    }

}

#endif
