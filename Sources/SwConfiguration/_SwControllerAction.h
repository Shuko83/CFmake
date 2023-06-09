/*!
 \file _SwControllerAction.h
 \brief action sur le controller
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _StreamWork_SwConfiguration_SwControllerAction_H
#define _StreamWork_SwConfiguration_SwControllerAction_H

/*
  * INCLUDES GLOBAUX
  */
#include <QAction>
#include <QObject>
/*
  * INCLUDES LOCAUX
  */

#include <ISwConfigurationController.h>
#include <ISwConfigurationControllerListener.h>

using namespace StreamWork::SwCore;
    
namespace StreamWork
{
    namespace SwConfiguration
    {    
        /*!
        	\class _SwControllerAction 
        	\brief Implementation of the Class _ISwControllerAction 
        */
        class _SwControllerAction : 
            public QObject,
            public ISwConfigurationControllerListener
        {
            Q_OBJECT
            public:
            
                /*! \brief Contructeur */
                _SwControllerAction();
                            
                /*! \brief Destructeur */
                virtual ~_SwControllerAction();
            
                void setController(ISwConfigurationController * controller);
        	    //---------------------------------------------------------------------
        	    // Interface ISwConfigurationControllerListener
        	    //---------------------------------------------------------------------

                /** @brief sur changement d etat*/
                virtual void OnStartedChanged(ISwConfigurationController *);

                /** @brief sur changement d etat*/
                virtual void OnConfigurationEditedChanged(ISwConfigurationController *);

                /*! \brief Renvoie le Action
                \return le Action */
			    QAction * getAction();
			
            protected:
                void setAction(QAction * action);
                virtual void onActionActivated();


                ISwConfigurationController           * _configurationController;
                QAction                              * _action;

            protected slots:
            
                void actionActivated();

        };
        
    }
}
#endif 
