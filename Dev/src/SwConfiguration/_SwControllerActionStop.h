/*!
 \file _SwControllerActionStop.h
 \brief start sur le controller
 \version 
 \date
 \author 
*/

#ifndef _StreamWork_SwConfiguration_SwControllerActionStop_H
#define _StreamWork_SwConfiguration_SwControllerActionStop_H

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
        class _SwControllerActionStop : 
            public _SwControllerAction
            
        {

        public:
            
            /*! \brief Contructeur */
            _SwControllerActionStop( ) ;
                        
            /*! \brief Destructeur */
            virtual ~_SwControllerActionStop();
        
        	//---------------------------------------------------------------------
        	// Interface ISwConfigurationControllerListener
        	//---------------------------------------------------------------------
        	
            /** @brief sur changement d etat*/
            virtual void OnStartedChanged(ISwConfigurationController *);

            /** @brief sur changement d etat*/
            virtual void OnConfigurationEditedChanged(ISwConfigurationController *);

        	//---------------------------------------------------------------------
        	// Interface ISwAction
        	//---------------------------------------------------------------------
        	
            /*! \brief Renvoie le Action
            \return le Action */
			virtual QAction & GetAction() ;
        
        protected:
            virtual void onActionActivated();
        };
        
    }
}
#endif 
