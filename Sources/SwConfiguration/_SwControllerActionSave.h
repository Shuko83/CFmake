/*!
 \file _SwControllerActionSave.h
 \brief Save sur le controller
 \version 
 \date
 \author 
*/

#ifndef _StreamWork_SwConfiguration_SwControllerActionSave_H
#define _StreamWork_SwConfiguration_SwControllerActionSave_H

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
#include <_ISwConfigurationLaunchManager.h>


using namespace StreamWork::SwCore;
    
namespace StreamWork
{
    namespace SwConfiguration
    {    
        /*!
        	\class _SwControllerAction 
        	\brief Implementation of the Class _ISwControllerAction 
        */
        class _SwControllerActionSave : 
            public _SwControllerAction
            
        {

        public:
            
            /*! \brief Contructeur */
            _SwControllerActionSave( ) ;
                        
            /*! \brief Destructeur */
            virtual ~_SwControllerActionSave();
        
        	//---------------------------------------------------------------------
        	// Interface ISwConfigurationControllerListener
        	//---------------------------------------------------------------------
            /** @brief sur changement d etat*/
            virtual void OnStartedChanged(ISwConfigurationController *);

            /** @brief sur changement d etat*/
            virtual void OnConfigurationEditedChanged(ISwConfigurationController *);
          
            void setLaunchManager(_ISwConfigurationLaunchManager * launchManager);      
        
        protected:
            virtual void onActionActivated();
            _ISwConfigurationLaunchManager * _launchManager;
        };
        
    }
}
#endif 
