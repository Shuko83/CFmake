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
#include "Component/Interfaces/ISwHost.h"
#include "Main/Services/Management/ISwService.h"
#include "Main/Connexion/ISwInterfaces_ServicesListener.h"
#include "Component/Base/SwComponent_Class.h"

namespace StreamWork 
{
    namespace SwCore
    { 
        /*!
        \interface ISwInterfaces_Service
        \brief service de gestions des interfaces
        @ingroup SwCoreGrp
        @swinterface
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
