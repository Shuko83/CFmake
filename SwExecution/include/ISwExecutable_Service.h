/*!
\file ISwExecutable_Service.h
\date 12/04/2006
\brief service de gestion de l'execution
\author  Big
\version 1.0
 */

#ifndef _ISwExecutable_Service_H
#define _ISwExecutable_Service_H
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
#include "ISwExecution_Service.h"
#include "Component/Interfaces/ISwActivable.h"

//Nom du service
#define CG_SW_SERVICE_EXECUTABLE "Executable"


namespace StreamWork 
{
    namespace SwExecution
    { 
        /*!
        \interface ISwExecutable_Service
        \brief service de gestion de l'execution
        @ingroup SwExeGrp
        @swinterface
        Sequence d'appel
        Initialize(temps courant)
        Start(temps courant)
        Execute(temps courant,true)
        Execute(temps courant,false)*
        Stop(temps courant)
        */
        class ISwExecutable_Service: 
            public SwCore::ISwService, 
            public SwCore::ISwHost, 
            virtual public SwCore::ISwActivable {

        private:
            //Status
            bool _isRunning;

        protected:
            virtual void InitializeService(){_isRunning=false;};   
        public:
	        /*! \brief Initialisation */
	        virtual void Initialize(double start_time,ISwExecution_Service * executor) throw (SwCore::SwException)=0;            
	        /*! \brief Demarrage */
	        virtual void Start(double current_time) throw (SwCore::SwException)=0;            
	        /*! \brief Execution */
	        virtual void Execute(double current_time,bool is_first_call) throw (SwCore::SwException)=0;            
	        /*! \brief Execution */
	        virtual void Stop(double current_time)=0;
	        /*! \brief status */
            void setRunning(bool running_state) { _isRunning=running_state;}
            bool isRunning() {return _isRunning;}

             //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_EXECUTABLE); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
       };
    }
}

#endif
