/*!
 \file _SwControllerActionRestart.h
 \brief start sur le controller
 \version 
 \date
 \author 
*/

#ifndef _StreamWork_SwConfiguration_SwControllerActionRestart_H
#define _StreamWork_SwConfiguration_SwControllerActionRestart_H

/*
  * INCLUDES GLOBAUX
  */
#include <QObject>
#include <QAction>  
/*
  * INCLUDES LOCAUX
  */

#include <_SwControllerAction.h>
#include <ISwConfigurationController.h>


using namespace StreamWork::SwCore;
    
namespace StreamWork
{
    namespace SwConfiguration
    {    
        /*!
        	\class _SwControllerAction 
        	\brief Implementation of the Class _ISwControllerAction 
        */
        class _SwControllerActionRestart : 
            public _SwControllerAction
            
        {

        public:
            
            /*! \brief Contructeur */
            _SwControllerActionRestart( ) ;
                        
            /*! \brief Destructeur */
            virtual ~_SwControllerActionRestart();
        
        	//---------------------------------------------------------------------
        	// Interface ISwConfigurationControllerListener
        	//---------------------------------------------------------------------
        	
            /** @brief sur changement d etat*/
            virtual void OnStartedChanged(ISwConfigurationController *);

            /** @brief sur changement d etat*/
            virtual void OnConfigurationEditedChanged(ISwConfigurationController *);
        
        protected:
            virtual void onActionActivated();
        };
        
    }
}
#endif 
