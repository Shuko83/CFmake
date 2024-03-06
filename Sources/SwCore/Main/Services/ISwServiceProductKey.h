/*!
\file ISwServiceProductKey.h
\date 28/09/2011
\brief interface service pour la vérification des options de licence
\author  OSI
\version 1.0
 */

#ifndef _ISwServiceProductKey_H
#define _ISwServiceProductKey_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "Main/Services/Management/ISwService.h"


//Nom du service
#define CG_SW_SERVICE_PRODUCT_KEY "ProductKeyService"


namespace StreamWork 
{
    namespace SwCore
    {
        /*!
        \interface ISwServiceProductKey
        \brief interface definissant un service pour la vérification des options de licence
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwServiceProductKey : public ISwService {
        public:
            /*! \brief verifie la licence*/
            virtual void GetOptionsList (QStringList & OptionList) = 0;
			virtual bool CheckOption (QString OptionName) = 0;

            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_PRODUCT_KEY); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
            
        };
    }
}

#endif
