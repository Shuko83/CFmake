/*!
\file ISwConfigurationController.h
\date 
\brief 
\author  
\version 
 */

#ifndef ISwConfigurationController_H
#define ISwConfigurationController_H
/*
  * INCLUDES GLOBAUX
  */
#include  <SwEnum.h>
#include  <SwFileDescriptor.h>
#include  <QString>
#include  <ISwConfigurationControllerListener.h>
#include <QObject>
/*
* INCLUDES LOCAUX
*/




using namespace StreamWork::SwCore;
    /*!
    	\class ISwConfigurationController 
    	\brief  
    */

namespace StreamWork
{
    namespace SwConfiguration
    {
        class ISwConfigurationController
            {

            
            
            public:

                       
                /*! \brief Destructeur */
                virtual ~ISwConfigurationController(){}

                virtual void SetReplayConfiguration() = 0;

                virtual void SetNormalConfiguration() = 0;

                virtual void setConfigurationEdited(bool value) = 0;

                virtual bool getConfigurationEdited() = 0;

                virtual bool Start() = 0;    
         
                virtual bool Stop() = 0;    

                virtual bool Load() = 0;    

                virtual bool Save() = 0;    

                virtual bool isStarted() = 0;

                /** @brief ajout d un listener */
                virtual void addControllerListener(ISwConfigurationControllerListener *) =0;

                /** @brief suppression d un listener */
                virtual void removeControllerListener(ISwConfigurationControllerListener *) =0;
                             
             };
    }
}

#endif
