/**
@file SwSnapShotPropertiesService.h
@brief Service de backup de property (implementation)
@author Big
 */

#ifndef _STREAMWORK_SWCORE_SWSNAPSHOTPROPERTIESSERVICE_H
#define _STREAMWORK_SWCORE_SWSNAPSHOTPROPERTIESSERVICE_H

#include "Component/Services/ISwSnapShotPropertiesService.h"
#include "SwCoreConstantes.h"
#include "Component/Services/ISwProperties.h"
#include "Component/Interfaces/ISwVisitor.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class SwSnapShotPropertiesService
        @brief Service de backup de property (implementation)
        */
        class BUILD_SWCORE SwSnapShotPropertiesService : public ISwSnapShotPropertiesService{
        public:
            /** @brief Constructor */
            SwSnapShotPropertiesService();
            /** @brief Destructor */
            virtual ~SwSnapShotPropertiesService();
            /** @brief doSnapShot */
            void doSnapShot(ISwProperties * properties);
            //test if property exists
            virtual bool exist(QString property_name);
            //remove property from snapshot
            virtual void removeFromSnapShot(QString property_name);
            //get value
            virtual QVariant getValue(QString property_name);
            //has changed
            virtual bool getHasChanged(QString property_name);
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate();             
        private:
            class PropDesc {
            public:
                QVariant value;
                bool hasChanged;
            };
            QMap<QString,PropDesc *> descProperties;
        };

        /**
        @class SwSnapShotPropertiesService
        @brief Service de backup de property (implementation)
        */
        class BUILD_SWCORE SwSnapShotPropertiesVisitor : public ISwVisitor{
        public:
            SwSnapShotPropertiesVisitor();
            /*! \brief methode de visite */
            virtual void Visit(SwComponent_Class *component);

        };

    
    }

}

#endif
