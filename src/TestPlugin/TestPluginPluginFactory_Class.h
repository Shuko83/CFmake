/**
 * @file TestPluginPluginFactory_Class.h
 * @brief Plugin Factory
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */

#ifndef _TestPluginPluginFactory_Class_H
#define _TestPluginPluginFactory_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QMap>
#include "TestPluginConstantes.h"

/*
  * INCLUDES LOCAUX
  */
#include <SwPluginFactory_Class.h>

using namespace StreamWork::SwCore;

/**
 *	@class TestPluginPluginFactory_Class
 *	@brief classe definissant un plugin usine
 */
class TestPluginPluginFactory_Class : public SwPluginFactory_Class
{
protected:
    /** @brief instanciation d'un composant */
    SwComponent_Class * CreateInstanceOf(QString name) ;
    /** @brief acces a l'icone d'un composant */
    QIcon CreateIconOf(QString name) const;
    /** @brief instanciation d'une data */
    SwData_Class * CreateInstanceOf(const SwUUID & type_id);
public:
    /** @brief Constructeur */
    TestPluginPluginFactory_Class();
    /** @brief Destructeur */
    virtual ~TestPluginPluginFactory_Class();
    /** @brief Initialisation */
    void Initialize();
    /** @brief Liberation */
    void Liberate();
    /** @brief Acces a la version du plugin */
    QString GetPluginVersion();
    //---------------------------------------------------------------------
    // Interface ISwServicesManager_Listener
    //---------------------------------------------------------------------
    /*! \brief sur ajout d'un service */
    virtual void OnRegisterService(ISwService * service);            
    /*! \brief sur suppression d'une  interface */
    virtual void OnUnregisterService(ISwService * service);            
};

#ifndef QT_NO_DEBUG
    extern "C" TestPlugin_EXPORT SwPluginFactory_Class *  GetPluginInterfaceD() {
        return new TestPluginPluginFactory_Class();
    }
#else 
    extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface() {
        return new TestPluginPluginFactory_Class();    
    }
#endif



#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
