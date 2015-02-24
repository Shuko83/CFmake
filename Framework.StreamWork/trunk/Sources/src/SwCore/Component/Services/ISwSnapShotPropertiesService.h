/**
@file ISwSnapShotPropertiesService.h
@brief Service de backup de property
@author Big
 */

#ifndef _STREAMWORK_SWCORE_ISWSNAPSHOTPROPERTIESSERVICE_H
#define _STREAMWORK_SWCORE_ISWSNAPSHOTPROPERTIESSERVICE_H

#include <QtCore>
#include "ISwService.h"

//Nom du service
#define CG_SW_SNAPSHOPPROPERTY_SERVICE "SwSnapShotProperties"


namespace StreamWork {

    namespace SwCore {
    
        /**
        @class ISwSnapShotPropertiesService
        @brief Service de backup de property
        */
        class ISwSnapShotPropertiesService : public ISwService{
        public:
            //test if property exists
            virtual bool exist(QString property_name)=0;
            //remove property from snapshot
            virtual void removeFromSnapShot(QString property_name)=0;
            //get value
            virtual QVariant getValue(QString property_name)=0;
            //has changed
            virtual bool getHasChanged(QString property_name)=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SNAPSHOPPROPERTY_SERVICE); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()*/
            QString GetServiceRealName() {return QString(typeid(this).name());}    
        
        };
    
    }

}

#endif
