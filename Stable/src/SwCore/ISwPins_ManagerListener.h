/*!
\file ISwPins_ManagerListener.h
\date 12/04/2006
\brief observateur du manager des pins
\author  Big
\version 1.0
 */

#ifndef _ISwPins_ManagerListener_H
#define _ISwPins_ManagerListener_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
namespace StreamWork 
{
    namespace SwCore
    { 
        class SwPin;
        /*!
        \class ISwPins_ManagerListener
        \brief observateur du manager des pins
        */
        class ISwPins_ManagerListener  {
        public:
	        /*! \brief Sur ajout d'un nouveau pin*/
	        virtual void OnAddPin(SwPin *)=0;            
	        /*! \brief Sur suppression d'un pin existant*/
	        virtual void OnRemovePin(SwPin *)=0;            
	        /*! \brief Sur connexion d'un pin*/
	        virtual void OnConnectPin(SwPin * local_pin,SwPin * remote_pin)=0;            
	        /*! \brief Sur deconnexion d'un pin*/
	        virtual void OnDisconnectPin(SwPin * local_pin,SwPin * remote_pin)=0;            
        };
    }
}

#endif
