/*!
\file ISwExecution_Service.h
\date 12/04/2006
\brief service de pilotage de l'execution
\author  Big
\version 1.0
 */

#ifndef _ISwExecution_Service_H
#define _ISwExecution_Service_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "Component/Interfaces/ISwHost.h"
#include "Main/Services/Management/ISwService.h"
#include "Tools/Exception/SwException.h"
#include "Component/Interfaces/ISwPersistent.h"
#include "ISwClockProvider.h"
#include "Component/Interfaces/ISwHost.h"

//Nom du service
#define CG_SW_SERVICE_EXECUTION "Execution"

namespace StreamWork 
{
    namespace SwExecution
    { 
        /*!
        \interface ISwExecution_Service
        \brief service de pilotage de l'execution
        @ingroup SwExeGrp
        @swinterface
        */
        class ISwExecution_Service: public SwCore::ISwService, public SwCore::ISwPersistent, public SwCore::ISwHost{
        public:
		    /*! \brief Demarrage de l'execution */
	        virtual void StartExecution()=0;
		    /*! \brief demande d'arret de l'execution*/
	        virtual void AskForStopExecution()=0;
		    /*! \brief demande d'arret de l'execution*/
	        virtual bool IsExecutionStopped()=0;
		    /*! \brief Arret de l'execution */
	        virtual void StopExecution()=0;
		    /*! \brief definition du fournisseur de temps */
	        virtual void SetClockProvider(ISwClockProvider * provider)=0;
             //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_EXECUTION); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
       };
    }
}

#endif
