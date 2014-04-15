/**
@file _SwServiceExtensionsImpl.h
@brief Extensions service implementation
@author Big
 */

#ifndef _STREAMWORK_SWCORE__SWSERVICEEXTENSIONSIMPL_H
#define _STREAMWORK_SWCORE__SWSERVICEEXTENSIONSIMPL_H

#include "ISwServiceExtensions.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class _SwServiceExtensionsImpl
        @brief Extensions service implementation
        */
        class _SwServiceExtensionsImpl : public ISwServiceExtensions {
        public:
            /** @brief Constructor */
            _SwServiceExtensionsImpl();
            /** @brief Destructor */
            virtual ~_SwServiceExtensionsImpl();
            /*! \brief Permet d'avoir une liste d'extension pourt un type donne */
            virtual QList<ISwExtension *> getExtensionWithType(QString extension_type_name);
        protected:
            /*! \brief Permet d'enregistrer une extension avec le type en nom */
            virtual void registerExtensionWithType(QString extension_type_name,QString extension_name,QString component_name);

            
        private:
            QMap<QString,QList<ISwExtension *>> _extensions;        
        };
    
    }

}

#endif
