/*!
 \file _SwControllerActionStart.h
 \brief start sur le controller
 \version 
 \date
 \author 
*/

#ifndef _StreamWork_SwConfiguration_SwControllerActionStart_H
#define _StreamWork_SwConfiguration_SwControllerActionStart_H

/*
  * INCLUDES GLOBAUX
  */
#include <QObject>
#include <QAction>  
/*
  * INCLUDES LOCAUX
  */

#include "_SwControllerAction.h"
#include "ISwConfigurationController.h"


using namespace StreamWork::SwCore;
    
namespace StreamWork
{
    namespace SwConfiguration
    {    
        /*!
        	\class _SwControllerAction 
        	\brief Implementation of the Class _ISwControllerAction 
        */
        class _SwControllerActionStart : 
            public _SwControllerAction
            
        {

        public:
            
            /*! \brief Contructeur */
            _SwControllerActionStart( ) ;
                        
            /*! \brief Destructeur */
            virtual ~_SwControllerActionStart();
        
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
