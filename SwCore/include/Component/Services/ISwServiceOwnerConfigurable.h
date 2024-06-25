/**
@file ISwServiceOwnerConfigurable.h
@brief service que doit implementer le composant s'il veut avoir les fonctionnalites de persistence en configuration
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWCORE_ISWSERVICEOWNERCONFIGURABLE_H
#define _STREAMWORK_SWCORE_ISWSERVICEOWNERCONFIGURABLE_H

/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "Main/Services/Management/ISwService.h"
#include "Component/Interfaces/ISwPersistentConfigurable.h"

//Nom du service
#define CG_SW_SERVICE_OWNER_CONFIGURABLE "OwnerConfigurable"


namespace StreamWork {

    namespace SwCore {
    
        /**
        @interface ISwServiceOwnerConfigurable
        @brief service que doit implementer le composant s'il veut avoir les fonctionnalites de persistence en configuration
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwServiceOwnerConfigurable : public ISwService, public ISwPersistentConfigurable {
        public:
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_OWNER_CONFIGURABLE); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    

        };

    }
    
}
#endif
