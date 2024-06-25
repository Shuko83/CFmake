/*!
 \file _SwControllerLoad.h
 \brief Load sur le controller
 \version 
 \date
 \author 
*/

#ifndef _StreamWork_SwConfiguration_SwControllerActionLoad_H
#define _StreamWork_SwConfiguration_SwControllerActionLoad_H

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
#include "_ISwManagedConfigurationsList.h"
#include "_ISwConfigurationLaunchManager.h"

using namespace StreamWork::SwCore;
    
namespace StreamWork
{
    namespace SwConfiguration
    {    
        /*!
        	\class _SwControllerAction 
        	\brief Implementation of the Class _ISwControllerAction 
        */
        class _SwControllerActionLoad : 
            public _SwControllerAction
            
        {

        public:
            
            /*! \brief Contructeur */
            _SwControllerActionLoad( ) ;
                        
            /*! \brief Destructeur */
            virtual ~_SwControllerActionLoad();
        
        	//---------------------------------------------------------------------
        	// Interface ISwConfigurationControllerListener
        	//---------------------------------------------------------------------
            /** @brief sur changement d etat*/
            virtual void OnStartedChanged(ISwConfigurationController *);

            /** @brief sur changement d etat*/
            virtual void OnConfigurationEditedChanged(ISwConfigurationController *);
        
            void setConfigurationsList(_ISwManagedConfigurationsList * configList);
          
            void setLaunchManager(_ISwConfigurationLaunchManager * launchManager);      	    
        protected:
            virtual void onActionActivated();
            QString _lastPath;
            _ISwManagedConfigurationsList * _configList;
            _ISwConfigurationLaunchManager * _launchManager;
        };
        
    }
}
#endif 
