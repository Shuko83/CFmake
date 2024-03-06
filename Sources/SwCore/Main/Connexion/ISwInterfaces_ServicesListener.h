/*!
\file ISwInterfaces_ServicesListener.h
\date 12/04/2006
\brief observateur des services de gestions des interfaces
\author  Big
\version 1.0
 */

#ifndef _ISwInterfaces_ServicesListener_H
#define _ISwInterfaces_ServicesListener_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "Main/Services/Management/ISwService.h"
#include "Component/Base/SwComponent_Class.h"

namespace StreamWork 
{
    namespace SwCore
    { 
        class ISwInterfaces_Service;
        /*!
        \interface ISwInterfaces_ServicesListener
        \brief observateur des services de gestions des interfaces (voir StreamWork::SwCore::ISwInterfaces_Service)
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwInterfaces_ServicesListener  {
        public:
	        /*! \brief Sur ajout d'une nouvelle interface */
	        virtual void OnAddInterface(ISwInterfaces_Service * source,QString interface_name)=0;            
	        /*! \brief Sur suppression d'une  interface */
	        virtual void OnRemoveInterface(ISwInterfaces_Service * source,QString interface_name)=0;            
	        /*! \brief Sur connection d'une interface */
	        virtual void OnConnectInterface(ISwInterfaces_Service * source,QString interface_name,ISwInterfaces_Service * remote_source,QString remote_interface_name)=0;            
	        /*! \brief Sur deconnection d'une interface */
	        virtual void OnDisconnectInterface(ISwInterfaces_Service * source,QString interface_name,ISwInterfaces_Service * remote_source,QString remote_interface_name)=0;            
        };
    }
}

#endif
