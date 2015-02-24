/**
 * @file {{PLUGIN_NAME}}Constantes.h
 * @brief Declaration de constantes
 * @version 1.0
 * @date {{DATE}}
 * @author {{AUTHOR}}
 */

#ifndef _{{PLUGIN_NAME_UPPER}}Constantes_H
#define _{{PLUGIN_NAME_UPPER}}Constantes_H

/**
  * INCLUDES LOCAUX
  */

#include <QGlobal.h>

//Export via les DLL
#include <SwMacros.h>


#ifdef {{PLUGIN_NAME}}_LIB
#define {{PLUGIN_NAME}}_EXPORT Q_DECL_EXPORT
#define {{PLUGIN_NAME}}_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_EXPORT const StreamWork::SwCore::SwUUID name;
#else
#define {{PLUGIN_NAME}}_EXPORT Q_DECL_IMPORT
#define {{PLUGIN_NAME}}_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_IMPORT const StreamWork::SwCore::SwUUID name;
#endif


#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
