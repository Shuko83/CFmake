/**
@file ISwEditionService.h
@brief Si present, alors en mode edition
@author Big
 */

#ifndef _STREAMWORK_SWCORE_ISWEDITIONSERVICE_H
#define _STREAMWORK_SWCORE_ISWEDITIONSERVICE_H

#include "Main/Services/Management/ISwService.h"

//Nom du service
#define CG_SW_EDITION_SERVICE "SwEditionService"


namespace StreamWork {

    namespace SwCore {
    
        /**
        @class ISwEditionService
        @brief Si present, alors en mode edition
        */
        class ISwEditionService : public ISwService {
        public:
            /*! \brief verifie la licence*/
            virtual bool editorSavingInProgress()=0;
            /*! \brief verifie la licence*/
            virtual void setEditorSavingInProgress(bool)=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_EDITION_SERVICE); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
        
        };
    
    }

}

#define GET_SW_EDITION_SERVICE dynamic_cast<ISwEditionService *>(SwApplication::GetInstance()->QueryService("SwEditionService"))

#endif
