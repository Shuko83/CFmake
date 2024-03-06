/**
@file ISwExtensionPoint.h
@brief Extension point
@author Big
 */

#ifndef _STREAMWORK_SWCORE_ISWEXTENSIONPOINT_H
#define _STREAMWORK_SWCORE_ISWEXTENSIONPOINT_H

#include "Extensions/ISwExtension.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class ISwExtensionPoint
        @brief Extension point
        */
        class ISwExtensionPoint {
        public:
            virtual ~ISwExtensionPoint() {}
            /** @brief renvoie le type */
            virtual QString getType()=0;

            /** @brief renvoie le nom */
            virtual QString getName()=0;

            /** @brief renvoie le composant (le status doit etre concret)*/
            virtual SwComponent_Class * getComponent()=0;

            /** @brief renvoie l'extension derriere le point d'extension*/
            virtual ISwExtension * getExtension()=0;

            /** @brief etends avec l'extension passé en parametre*/
            virtual void extends(ISwExtension *)=0;

            /** @brief supprimer l'extension*/
            virtual ISwExtension * removeExtension()=0;
        
        };
    
    }

}

#endif
