/**
@file _SwExtensionPointImpl.h
@brief Extension point implementation
@author Big
 */

#ifndef _STREAMWORK_SWCORE__SWEXTENSIONPOINTIMPL_H
#define _STREAMWORK_SWCORE__SWEXTENSIONPOINTIMPL_H

#include "Extensions/ISwExtensionPoint.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class _SwExtensionPointImpl
        @brief Extension point implementation
        */
        class _SwExtensionPointImpl : public ISwExtensionPoint{
        public:
            /** @brief Constructor */
            _SwExtensionPointImpl(QString type,QString name,SwComponent_Class * component);
            /** @brief Destructor */
            virtual ~_SwExtensionPointImpl();
            /** @brief renvoie le type */
            virtual QString getType();

            /** @brief renvoie le nom */
            virtual QString getName();

            /** @brief renvoie le composant (le status doit etre concret)*/
            virtual SwComponent_Class * getComponent();

            /** @brief renvoie l'extension derriere le point d'extension*/
            virtual ISwExtension * getExtension();

            /** @brief etends avec l'extension passé en parametre*/
            virtual void extends(ISwExtension * ext);

            /** @brief supprimer l'extension*/
            virtual ISwExtension * removeExtension();

        protected:
            QString _type;
            QString _name;
            SwComponent_Class * _component;

        };
    
    }

}

#endif
