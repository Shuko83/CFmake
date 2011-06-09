/**
@file _SwExtensionImpl.h
@brief Extension implementation
@author Big
 */

#ifndef _STREAMWORK_SWCORE__SWEXTENSIONIMPL_H
#define _STREAMWORK_SWCORE__SWEXTENSIONIMPL_H

#include "ISwExtension.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class _SwExtensionImpl
        @brief Extension implementation
        */
        class _SwExtensionImpl : public ISwExtension{
        public:
            /** @brief Constructor */
            _SwExtensionImpl(QString type,QString name,QString componentTypeName);
            /** @brief Constructor */
            _SwExtensionImpl(QString type,QString name,QString componentTypeName,SwComponent_Class * component);
            /** @brief Destructor */
            virtual ~_SwExtensionImpl();

            /** @brief renvoie le status */
            virtual ExtensionStatus getStatus();

            /** @brief renvoie le type */
            virtual QString getType();

            /** @brief renvoie le nom */
            virtual QString getName();

            /** @brief renvoie le nom du type de composant*/
            virtual QString getComponentType();

            /** @brief renvoie le composant (le status doit etre concret)*/
            virtual SwComponent_Class * getComponent();

            /** @brief renvoie une extension concrete (si elle est concrete, renvoie elle meme)*/
            virtual ISwExtension * concretise(SwComponent_Class * sourceComponent);
        private:
            ExtensionStatus _status;
            QString _type;
            QString _name;
            QString _componentTypeName;
            SwComponent_Class * _component;
        };
    
    }

}

#endif
