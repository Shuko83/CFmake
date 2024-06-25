/*!
\file _SwManagedConfiguration.h
\date 
\brief 
\author  
\version 
 */

#ifndef _SwManagedConfiguration_H
#define _SwManagedConfiguration_H
/*
  * INCLUDES GLOBAUX
  */


/*
  * INCLUDES LOCAUX
  */
//#include "ISwConfigurationListener.h"
#include "Component/Interfaces/ISwObservable.h"
#include "Component/Interfaces/ISwObserver.h"
#include "ISwConfiguration.h"
#include "ISwConfigurationController.h"
#include "Properties/SwPropertiesModelImpl.h"
#include <QAbstractItemModel.h>

using namespace StreamWork::SwConfiguration;
using namespace StreamWork::SwCore;

/*!
	\class _SwManagedConfiguration 
	\brief  
*/
class _SwManagedConfiguration : 
    public ISwConfigurationListener,
    public StreamWork::SwCore::ISwObserver
{

protected:


public:
    /*! \brief Constructeur */
    _SwManagedConfiguration();
          
    /*! \brief Destructeur */
    virtual ~_SwManagedConfiguration();
            
    /** @brief sur changement des propriétés*/
    virtual void OnPropertiesChanged(ISwConfiguration *);

    /** @brief sur changement des propriétés*/
    virtual void OnActivate(ISwConfiguration *);

    /** @brief sur changement des propriétés*/
    virtual void OnDisable(ISwConfiguration *);

    /** @brief sur changement des propriétés*/
    //virtual void OnRecordAvailableChanged(ISwConfiguration *);

    /** @brief affectation de la configuration*/
    void setConfiguration(ISwConfiguration * configuration, QString name);
    
    /** @brief acces au nom de la configuration*/
	 QString getName();

    /** @brief affectation du nom de la configuration*/
	void    setName(QString name);

    /** @brief acces a la configuration*/
    ISwConfiguration * getConfiguration();

    /** @brief acces au model*/
    QAbstractItemModel * getModel();
   
    /*! \brief methode permettant de charger un fichier de propriétés*/
    bool Load(QDomElement & elt, QString * errorMessage = 0);

    /*! \brief methode permettant de charger un fichier de propriétés*/
    bool Save(QXmlStreamWriter &writer, QString * errorMessage = 0 );

    bool isEnabled();

    void setEnabled( bool value);

    void setRecordAvailable( bool value);

    bool isRecordAvailable();

    /*! \brief signale une modification de la configuration necessitant une sauvegarde*/
	void Update(StreamWork::SwCore::ISwObservable *);

    void setConfigurationController(ISwConfigurationController * controller);

    void unregisterOwnerConfigurableListener();

    void registerOwnerConfigurableListener();
protected: 
    

    ISwConfiguration *           _configuration;
    QString                      _name;
    SwPropertiesModelImpl *      _model;
    bool                         _loaded;
    bool                         _enabled;
    ISwConfigurationController * _controller;
    bool                         _recordAvailable;   
 };

#endif
