/*!
\file _ISwConfigurationLaunchManager.h
\date 
\brief 
\author  
\version 
 */

#ifndef _ISwConfigurationLaunchManager_H
#define _ISwConfigurationLaunchManager_H
/*
  * INCLUDES GLOBAUX
  */
#include  <SwEnum.h>
#include  <SwFileDescriptor.h>
#include  <QString>
#include  <_ISwConfigurationLaunchEditor.h>
#include <QObject>
/*
* INCLUDES LOCAUX
*/




using namespace StreamWork::SwCore;
    /*!
    	\class _ISwConfigurationLaunchManager 
    	\brief  
    */
class ISwConfigurationLaunchEditor;

class _ISwConfigurationLaunchManager : public QObject
    {
    public:
               
        /*! \brief Destructeur */
        virtual ~_ISwConfigurationLaunchManager(){}
        
        virtual void setEditor(ISwConfigurationLaunchEditor * editor) = 0;
        
        virtual SwEnum getStartMode() = 0;   
        virtual void setStartMode(SwEnum value) = 0;    
                  
        virtual bool getAutoStart() = 0;            
        virtual void setAutoStart(bool value) = 0; 

        virtual bool getRecordActivated() = 0;            
        virtual void setRecordActivated(bool value) = 0; 
        
        virtual SwFileDescriptor getRecordDirectory() = 0;            
        virtual void setRecordDirectory(SwFileDescriptor value) = 0; 
        
        virtual SwFileDescriptor getReplayDirectory() = 0;            
        virtual void setReplayDirectory(SwFileDescriptor value) = 0; 
               
        virtual SwFileDescriptor getConfigurationFile() = 0;            
        virtual void setConfigurationFile(SwFileDescriptor value) = 0;       
     };


#endif
