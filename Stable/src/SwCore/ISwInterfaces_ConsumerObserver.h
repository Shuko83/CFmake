/*!
\file ISwInterfaces_ConsumerObserver.h
\date 12/04/2006
\brief observateur d'un interfaces_consumer
\author  Big
\version 1.0
 */

#ifndef _ISwInterfaces_ConsumerObserver_H
#define _ISwInterfaces_ConsumerObserver_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "SwComponent_Class.h"

namespace StreamWork 
{
    namespace SwCore
    { 
        /*!
        \interface ISwInterfaces_ConsumerObserver
        \brief observateur d'un interfaces_consumer (voir StreamWork::SwCore::ISwInterfaces_Consumer )
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwInterfaces_ConsumerObserver  {
        public:
	        /*! \brief Avant changement de la disponibilité de l'interface */
	        virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host)=0;            
	        /*! \brief Apres changement de la disponibilité de l'interface */
	        virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host)=0;            
        };
    }
}

#endif
