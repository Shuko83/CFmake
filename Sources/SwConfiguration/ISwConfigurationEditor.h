/*!
\file ISwConfigurationEditor.h
\date 
\brief 
\author  
\version 
 */

#ifndef _ISwConfigurationEditor_H
#define _ISwConfigurationEditor_H
/*
  * INCLUDES GLOBAUX
  */


/*
  * INCLUDES LOCAUX
  */

#include <QList.h>
#include "_SwManagedConfiguration.h"
#include "_ISwManagedConfigurationsList.h"

using namespace StreamWork::SwConfiguration;
using namespace StreamWork::SwCore;


class _ISwManagedConfigurationsList;

/*!
	\class ISwConfigurationEditor 
	\brief  
*/
class ISwConfigurationEditor
    
{

protected:


public:
    
    /*! \brief Destructeur */
    virtual ~ISwConfigurationEditor() {};   

    virtual void setManagedConfigurationsModel(_ISwManagedConfigurationsList * model) = 0;
    
    virtual void onConfigurationListChanged() = 0 ;

    virtual void onCurrentConfigurationChanged() = 0 ;

    virtual void onCurrentConfigurationLoaded() = 0 ;

    virtual void onCurrentConfigurationSaved()= 0 ;

    virtual void setMonoConfiguration(bool value) = 0 ;
    //virtual void setConfigurationsList(QList<_SwManagedConfiguration *> & configList) = 0;    
            
protected: 
    

 };

#endif
