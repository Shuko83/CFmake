/**
 * @file TestPluginConstantes.h
 * @brief Declaration de constantes
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */

#ifndef _TestPluginConstantes_H
#define _TestPluginConstantes_H

/*
  * INCLUDES LOCAUX
  */

#include <Qt/qglobal.h>
//Export via les DLL
#include <SwMacros.h>

#ifdef Q_CC_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4100)  
#pragma warning(disable:4250)
#endif


#ifdef TestPlugin_LIB
#define TestPlugin_EXPORT Q_DECL_EXPORT
#define TestPlugin_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_EXPORT const StreamWork::SwCore::SwUUID name;
#else
#define TestPlugin_EXPORT Q_DECL_IMPORT
#define TestPlugin_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_IMPORT const StreamWork::SwCore::SwUUID name;
#endif


#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
