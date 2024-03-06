/*!
\file ISwConfigurationLaunchManager.h
\date 
\brief 
\author  
\version 
 */

#ifndef _ISwConfigurationLaunchEditor_H
#define _ISwConfigurationLaunchEditor_H
/*
  * INCLUDES GLOBAUX
  */


/*
  * INCLUDES LOCAUX
  */

#include "_ISwConfigurationLaunchManager.h"

using namespace StreamWork::SwCore;

class _ISwConfigurationLaunchManager;

/*!
	\class ISwConfigurationLaunchEditor 
	\brief  
*/
class ISwConfigurationLaunchEditor
    
{

protected:


public:
    
    /*! \brief Destructeur */
    virtual ~ISwConfigurationLaunchEditor() {};   

    virtual void setLaunchConfigurationModel(_ISwConfigurationLaunchManager * model) = 0;
    
    virtual void startModeUpdated() = 0;

    virtual void autoStartUpdated() = 0;

    virtual void configurationFileNameUpdated() = 0;

    virtual void recordActivatedUpdated() = 0;

    virtual void recordDirectoryUpdated() = 0;

    virtual void replayDirectoryUpdated() = 0;
 
protected: 
    

 };

#endif
