/**
@file ISwExtension.h
@brief Extensions
@author Big
 */

#ifndef _STREAMWORK_SWCORE_ISWEXTENSION_H
#define _STREAMWORK_SWCORE_ISWEXTENSION_H

#include "SwComponent_Class.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class ISwExtension
        @brief Extensions
        */
        class ISwExtension {
        public:
            virtual ~ISwExtension() {}
            /** @brief Extension status */
            enum ExtensionStatus {Abstract,Concrete};

            /** @brief renvoie le status */
            virtual ExtensionStatus getStatus()=0;

            /** @brief renvoie le type */
            virtual QString getType()=0;

            /** @brief renvoie le nom */
            virtual QString getName()=0;

            /** @brief renvoie le nom du type de composant*/
            virtual QString getComponentType()=0;

            /** @brief renvoie le composant (le status doit etre concret)*/
            virtual SwComponent_Class * getComponent()=0;

            /** @brief renvoie une extension concrete (si elle est concrete, renvoie elle meme)*/
            virtual ISwExtension * concretise(SwComponent_Class * sourceComponent)=0;


        };
    
    }

}

#endif
