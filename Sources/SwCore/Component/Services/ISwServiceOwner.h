/*!
\file ISwServiceOwner.h
\date 12/04/2006
\brief service que doit implementer le composant s'il veut avoir les fonctionnalites de persistence
\author  Big
\version 1.0
 */

#ifndef _ISwServiceOwner_H
#define _ISwServiceOwner_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "Main/Services/Management/ISwService.h"
#include "Component/Interfaces/ISwPersistent.h"

//Nom du service
#define CG_SW_SERVICE_OWNER "Owner"


namespace StreamWork 
{
    namespace SwCore
    {
        /*!
        \interface ISwServiceOwner
        \brief service que doit implementer le composant s'il veut avoir les fonctionnalites de persistence
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwServiceOwner : public ISwService, public ISwPersistent {
        public:
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_OWNER); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
            
        };
    }
}

#endif
