/*!
\file _SwManagedConfigurationsList.h
\date 
\brief 
\author  
\version 
 */

#ifndef _SwManagedConfigurationsList_H
#define _SwManagedConfigurationsList_H
/*
  * INCLUDES GLOBAUX
  */


/*
  * INCLUDES LOCAUX
  */
#include "ISwConfigurationListener.h"
#include "ISwConfiguration.h"
#include "Properties/ISwProperty.h"
#include "_SwManagedConfiguration.h"
#include "Component/Interfaces/ISwInterfaces_ConsumerObserver.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Consumer_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "ISwConfigurationEditor.h"
#include "_ISwManagedConfigurationsList.h"
#include "ISwConfigurationNameProvider.h"
#include "ISwConfigurationNameListener.h"
#include "ISwConfigurationController.h"

class ISwConfigurationEditor;
class QDomElement;
class QDir;

using namespace StreamWork::SwConfiguration;
using namespace StreamWork::SwCore;
/*!
	\class _SwManagedConfigurationsList 
	\brief  
*/
class _SwManagedConfigurationsList :
    public ISwInterfaces_ConsumerObserver,
    public _ISwManagedConfigurationsList,
    public ISwConfigurationNameProvider
    
{

protected:


public:
    /*! \brief Constructeur */
    _SwManagedConfigurationsList();
    /*! \brief Constructeur */
    _SwManagedConfigurationsList(
                SwInterfaces_Provider_Class * provider_service,
                SwInterfaces_Consumer_Class * consumer_service,
                SwProperties_Class * properties_service);
      
    /*! \brief Destructeur */
    virtual ~_SwManagedConfigurationsList();   
    
    virtual _SwManagedConfiguration * getSelectedConfiguration();

    virtual void setSelectedConfiguration(_SwManagedConfiguration * configuration);

    virtual void setEditor(ISwConfigurationEditor * editor);
    
    virtual QList<_SwManagedConfiguration *> getConfigurationsList();

    virtual bool LoadConfiguration(QString filename);

    virtual bool SaveConfiguration(QString filename);

    /** @brief  traitement de la configuration */
    void processSaveConfigurationNode(QDomElement & elt,const QDir & targetRepository);
    /** @brief  traitement de la configuration */
    void processLoadConfigurationNode(QDomElement & elt,const QDir & targetRepository);

    void setConfigurationCount(int value);
    
    /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
    /*! \brief Avant changement de la disponibilité de l'interface */
    virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);  
              
    /*! \brief Apres changement de la disponibilité de l'interface */
    virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
                      
    virtual void AddConfigurationNameListener (ISwConfigurationNameListener * listener) ;
    virtual void RemoveConfigurationNameListener (ISwConfigurationNameListener * listener) ;  
    virtual QString GetSelectedName();

    void fullReset();
    
    void setConfigurationController(ISwConfigurationController * controller);

protected: 
    
    /* map des configurations*/
	  QMap<int,ISwConfiguration *>            _configurations;
	  
	  /* noms des configuration */
	  QMap<int,QString>			                  _configurationsNames;
	  
	  /* propriétés de nommage des configuration */
	  QMap<int,ISwProperty *>		              _configurationsNamesProperties;
	  
	  /* activation de l'enregistrement dans une configuration */
	  QMap<int,bool>			                    _configurationsRecordable;
	  
	  /* propriétés d'activation de l'enregistrement dans une configuration */
	  QMap<int,ISwProperty *>		              _configurationsRecordableProperties;
	  
	  /* liste des configurations */
	  QMap<int,_SwManagedConfiguration *>		  _managedConfigurations; 
    ISwConfigurationEditor      *           _editor;
    _SwManagedConfiguration              *  _selectedConfiguration;
    ISwConfigurationController           *  _configurationController;


    uint _configurationsCount;
    /* propriété nombre de configurations */
    ISwProperty * _configurations_count_property;
    
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;     
       
	  void renameConfigurationConsumed( int index, QString NewName);
	  void addConfigurationConsumed( int index);
	  void removeConfigurationConsumed( int index);
    void notifyListNameChanged() ;	
    
	  QList<ISwConfigurationNameListener *>   _configurationNamesListeners;
 };

#endif
