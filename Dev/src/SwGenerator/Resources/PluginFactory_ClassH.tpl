/**
 * @file {{PLUGIN_NAME}}PluginFactory_Class.h
 * @brief Plugin Factory
 * @version 1.0
 * @date {{DATE}}
 * @author {{AUTHOR}}
 */

#ifndef _{{PLUGIN_NAME_UPPER}}_PLUGINFACTORY_CLASS_H
#define _{{PLUGIN_NAME_UPPER}}_PLUGINFACTORY_CLASS_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QMap>
#include "{{PLUGIN_NAME}}Constantes.h"

/*
  * INCLUDES LOCAUX
  */
#include <SwPluginFactory_Class.h>

using namespace StreamWork::SwCore;

/**
 *    @class {{PLUGIN_NAME}}PluginFactory_Class
 *    @brief classe definissant un plugin usine
 *  @ingroup SwPluginGrp
 */
class {{PLUGIN_NAME}}PluginFactory_Class : public SwPluginFactory_Class
{
protected:
    /** 
      * @brief instanciation d'un composant 
      */
    SwComponent_Class * CreateInstanceOf(QString name) ;
    
	/** 
	 * @brief acces a l'icone d'un composant
	 * @param[in] name nom du composant
	 * @return l'incone ou icone vide si non trouvé
	 */
    QIcon CreateIconOf(QString name) const;
   
   /** 
     *@brief instanciation d'une data 
     */
    SwData_Class * CreateInstanceOf(const SwUUID & type_id);
public:
    /** 
      * @brief Constructeur 
      */
    {{PLUGIN_NAME}}PluginFactory_Class();
    
	/** 
	  * @brief Destructeur 
	  */
    virtual ~{{PLUGIN_NAME}}PluginFactory_Class();
    
	/** 
	  * @brief Initialisation 
	  */
    void Initialize();
    
	/** 
	  * @brief Liberation 
	  */
    void Liberate();
    
	/** 
	 * @brief Acces a la version du plugin
	 * @return version
	 */
    QString GetPluginVersion();
};

#ifndef QT_NO_DEBUG
    extern "C" {{PLUGIN_NAME}}_EXPORT SwPluginFactory_Class *  GetPluginInterfaceD() {
        return new {{PLUGIN_NAME}}PluginFactory_Class();
    }
#else 
    extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface() {
        return new {{PLUGIN_NAME}}PluginFactory_Class();    
    }
#endif

#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
