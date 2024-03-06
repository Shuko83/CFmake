/**
@file SwExtensionImpl.h
@brief Extension implementation
@author Big
 */

#ifndef _STREAMWORK_SWCORE_SwExtensionImpl_H
#define _STREAMWORK_SWCORE_SwExtensionImpl_H

#include "Extensions/ISwExtension.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class SwExtensionImpl
        @brief Extension implementation
        */
        class BUILD_SWCORE SwExtensionImpl : public ISwExtension{
        public:
            /** @brief Constructor */
            SwExtensionImpl(QString type,QString name,QString pluginName,QString componentTypeName);
            /** @brief Constructor */
            SwExtensionImpl(QString type,QString name,QString pluginName, QString componentTypeName,SwComponent_Class * component);
            /** @brief Destructor */
            virtual ~SwExtensionImpl();

            /** @brief renvoie le status */
            virtual ExtensionStatus getStatus();

            /** @brief renvoie le type */
            virtual QString getType();

            /** @brief renvoie le nom */
            virtual QString getName();

			/** @brief renvoie le nom du plugin*/
			virtual QString getPluginName();

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
			QString _pluginName;
			QString _componentTypeName;
            SwComponent_Class * _component;
        };
    
    }

}

#endif
