/**
@file ISwModelService.h
@brief Boite a outil pour la creation de model
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWMODEL_ISWMODELSERVICE_H
#define _STREAMWORK_SWMODEL_ISWMODELSERVICE_H

#include "Main/Services/Management/ISwService.h"

//Nom du service
#define CG_SW_SERVICE_MODEL "ServiceModel"


namespace StreamWork {

    namespace SwModel {
    
        /**
        @class ISwModelService
        @brief Boite a outil pour la creation de model
        */
        class ISwModelService : public SwCore::ISwService {
        public:
            /** @brief internal access (backup compatibility) */
            virtual QString getModelsDirectory()=0;
            /** @brief add model */
            virtual void addModel(QString hostComponent,QString modelName)=0;    
			/*! \brief check if the model name exist */
			virtual bool checkModelName(QString modelName) = 0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            virtual void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_MODEL); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
        
        };
    
    }

}

#endif
