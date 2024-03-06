/*!
\file _IISwManagedConfigurationsList.h
\date 
\brief 
\author  
\version 
 */

#ifndef _ISwManagedConfigurationsList_H
#define _ISwManagedConfigurationsList_H
/*
  * INCLUDES GLOBAUX
  */


/*
  * INCLUDES LOCAUX
  */

#include "_SwManagedConfiguration.h"
#include "ISwConfigurationEditor.h"
#include "ISwConfigurationSaver.h"
#include <QList>


using namespace StreamWork::SwConfiguration;
using namespace StreamWork::SwCore;


class ISwConfigurationEditor;

/*!
	\class _ISwManagedConfigurationsList 
	\brief  
*/
class _ISwManagedConfigurationsList : public ISwConfigurationSaver
    
{

protected:


public:

      
    /*! \brief Destructeur */
    virtual ~_ISwManagedConfigurationsList(){};   
    
    virtual _SwManagedConfiguration * getSelectedConfiguration() = 0;

    virtual void setSelectedConfiguration(_SwManagedConfiguration * configuration) = 0;

    virtual void setEditor(ISwConfigurationEditor * editor) = 0;
    
    virtual QList<_SwManagedConfiguration *> getConfigurationsList() = 0;

    virtual bool LoadConfiguration(QString filename) = 0;

    virtual bool SaveConfiguration(QString filename) = 0;
 };

#endif
