/*!
\file ISwPin_Listener.h
\date 12/04/2006
\brief observateur d'un pin
\author  Big
\version 1.0
 */

#ifndef _ISwPin_Listener_H
#define _ISwPin_Listener_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "SwData_Class.h"

namespace StreamWork 
{
    namespace SwCore
    { 
        class SwPin;
        /*!
        \class ISwPin_Listener
        \brief observateur d'un pin
        */
        class ISwPin_Listener  {
        public:
	        /*! \brief Sur reception d'une donnée*/
	        virtual void OnReceiveData(SwPin *,SwData_Class *)=0;            
        };
    }
}

#endif
