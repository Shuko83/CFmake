/*!
\file SwDatasBaseConstantes.h
\date 28/04/2006
\brief Constantes de SwGui
\author  Big
\version 1.0
 */

#ifndef _SwDatasBaseConstantes_H
#define _SwDatasBaseConstantes_H

#include <QGlobal.h>
//Export via les DLL
#include <SwMacros.h>

#ifdef SWDATABASE_LIB
    #define SWDATASBASE_EXPORT Q_DECL_EXPORT
	#define SWDATASBASE_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_EXPORT const StreamWork::SwCore::SwUUID name;
#else
    #define SWDATASBASE_EXPORT Q_DECL_IMPORT
	#define SWDATASBASE_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_IMPORT const StreamWork::SwCore::SwUUID name;
#endif

#ifdef Q_CC_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4100)
#endif

#endif

