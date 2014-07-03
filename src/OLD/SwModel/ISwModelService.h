/**
@file ISwModelService.h
@brief service model 
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWMODEL_ISWMODELSERVICE_H
#define _STREAMWORK_SWMODEL_ISWMODELSERVICE_H

#include "ISwService.h"

//Nom du service
#define CG_SW_SERVICE_MODEL "MODEL"

namespace StreamWork {

    namespace SwModel {
    
        /**
        @class ISwModelService
        @brief service model 
        */
        class ISwModelService {
        public:
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_MODEL); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()*/
            QString GetServiceRealName() {return QString(typeid(this).name());}
            /*! \brief Permet de rajouter un modele */


        };

    }
    
}
#endif
