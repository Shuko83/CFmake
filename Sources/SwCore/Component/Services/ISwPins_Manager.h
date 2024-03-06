/*!
\file ISwPins_Manager.h
\date 12/04/2006
\brief service  de gestion des pins
\author  Big
\version 1.0
 */

#ifndef _ISwPins_Manager_H
#define _ISwPins_Manager_H

/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QList>
/*
  * INCLUDES LOCAUX
 */
#include "Component/Interfaces/ISwHost.h"
#include "Component/Interfaces/ISwPersistent.h"
#include "Main/Services/Management/ISwService.h"
#include "Main/Connexion/ISwPins_ManagerListener.h"
#include "Component/Pin/SwPin.h"

//Nom du service
#define CG_SW_SERVICE_PINS_MANAGER "PinsManager"


namespace StreamWork 
{
    namespace SwCore
    {
        class SwPin;
		/*!
        \interface ISwPins_Manager
        \brief service  de gestion des pins
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwPins_Manager : public ISwService, public ISwHost, public ISwPersistent {
		public:
	        /*! \brief Enregistre un nouveau pin*/
	        virtual void RegisterPin(SwPin *)=0;
	        /*! \brief Desenregistre un pin existant*/
	        virtual void UnregisterPin(SwPin *)=0;
	        /*! \brief Connect un pin externe a un pin local*/
	        virtual void ConnectRemotePinToLocalPin(QString local_pin,QString remote_pin,ISwPins_Manager * remote_manager)=0;
	        /*! \brief Deconnecte un pin existant*/
	        virtual void DisconnectPin(QString local_pin)=0;
	        /*! \brief Enregistre un listener*/
	        virtual void RegisterListener(ISwPins_ManagerListener *)=0;
	        /*! \brief Desenregistre  un listener*/
	        virtual void UnregisterListener(ISwPins_ManagerListener *)=0;	
	        /*! \brief Acces a la liste des plugins*/
	        virtual QList<SwPin *> GetPinList()=0;	
	        /*! \brief Acces a la liste des pins par nom*/
	        virtual SwPin * GetPinByName(QString name)=0;	
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_PINS_MANAGER); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
            
        };
    }
}

#endif
