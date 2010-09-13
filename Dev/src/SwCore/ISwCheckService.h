/*!
\file ISwCheckService.h
\date 12/04/2006
\brief interface service pour la v&eacute;rification de clef logicielle
\author  Big
\version 1.0
 */

#ifndef _ISwCheckService_H
#define _ISwCheckService_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "ISwService.h"


//Nom du service
#define CG_SW_CHECK_SERVICE "CheckLicense"


namespace StreamWork 
{
    namespace SwCore
    {
        /*!
        \class ISwCheckService
        \brief interface service pour la v&eacute;rification de clef logicielle
        */
        class ISwCheckService : public ISwService {
        public:
            /*! \brief verifie la licence*/
            virtual void check()=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_CHECK_SERVICE); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()*/
            QString GetServiceRealName() {return QString(typeid(this).name());}    
            
        };
    }
}

#endif
