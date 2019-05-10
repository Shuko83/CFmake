/*!
\file ISwController.h
\date 30/05/2007
\brief service controller
\author  Big
\version 1.0
 */

#ifndef _ISwController_H
#define _ISwController_H
/*
  * INCLUDES GLOBAUX
 */
#include <QList>
#include <QString>
#include <QMetaType>
#include <QObject>
/*
  * INCLUDES LOCAUX
 */
#include "ISwService.h"
#include "ISwHost.h"
#include "ISwPersistent.h"
#include "ISwProperties.h"

//Nom du service
#define CG_SW_SERVICE_CONTROLLER "Controller"

namespace StreamWork 
{
    namespace SwCore
    {
        class ISwProperties;
        /*!
        @interface ISwController
        @brief  service controller
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwController: public ISwService, public ISwHost, public ISwPersistent {
        public:
	        /*! \brief Permet de crée une propriété */
	        virtual void InitializeControl(ISwProperties * properties,QString property_name)=0;		
	        /*! \brief le controle est supprimer au niveau de la propriété */
	        virtual void DetachControl()=0;		
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_CONTROLLER); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
        };
    }
}

#endif
