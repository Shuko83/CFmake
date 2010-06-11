/*!
\file ISwInterfaces_Service.h
\date 12/04/2006
\brief service de gestions des interfaces
\author  Big
\version 1.0
 */

#ifndef _ISwInterfaces_Service_H
#define _ISwInterfaces_Service_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "ISwHost.h"
#include "ISwService.h"
#include "ISwInterfaces_ServicesListener.h"
#include "SwComponent_Class.h"

namespace StreamWork 
{
    namespace SwCore
    { 
        /*!
        \class ISwInterfaces_Services
        \brief service de gestions des interfaces
        */
        class ISwInterfaces_Service: public ISwService, public ISwHost{
        public:
	        /*! \brief Ajout d'un listener */
	        virtual void AttachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener)=0;            
	        /*! \brief Retrait d'un listener */
	        virtual void DetachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener)=0;            
	        /*! \brief Permet de recuperer le type de l'interface (via typeid)*/
	        virtual QString GetInterfaceType(QString)=0;
        };
    }
}

#endif
