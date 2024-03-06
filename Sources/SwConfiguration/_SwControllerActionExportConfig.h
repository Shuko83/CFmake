/*!
 \file _SwControllerExportConfig.h
 \brief ExportConfig sur le controller
 \version 
 \date
 \author 
*/

#ifndef _StreamWork_SwConfiguration_SwControllerActionExportConfig_H
#define _StreamWork_SwConfiguration_SwControllerActionExportConfig_H

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
#include "ISwConfigurationSaver.h"
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
        class _SwControllerActionExportConfig : 
            public _SwControllerAction
            
        {

        public:
            
            /*! \brief Contructeur */
            _SwControllerActionExportConfig( ) ;
                        
            /*! \brief Destructeur */
            virtual ~_SwControllerActionExportConfig();
        
        	//---------------------------------------------------------------------
        	// Interface ISwConfigurationControllerListener
        	//---------------------------------------------------------------------
            /** @brief sur changement d etat*/
            virtual void OnStartedChanged(ISwConfigurationController *);

            /** @brief sur changement d etat*/
            virtual void OnConfigurationEditedChanged(ISwConfigurationController *);
        
            void setConfigSaver(ISwConfigurationSaver * configSaver);
          
            void setLaunchManager(_ISwConfigurationLaunchManager * launchManager);      	    
        protected:
            virtual void onActionActivated();
            
            QString _lastPath;
            ISwConfigurationSaver * _configSaver;
            _ISwConfigurationLaunchManager * _launchManager;
        };
        
    }
}
#endif 
