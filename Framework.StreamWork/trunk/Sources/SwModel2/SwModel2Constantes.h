/*!
\file SwModel2Constantes.h
\date 28/04/2006
\brief Constantes de SwGui
\author  Big
\version 1.0
 */

#ifndef _SwModel2Constantes_H
#define _SwModel2Constantes_H

#include <QGlobal.h>
//Export via les DLL

#ifdef SWMODEL2_LIB
#define SWMODEL2_EXPORT Q_DECL_EXPORT
#else
#define SWMODEL2_EXPORT Q_DECL_IMPORT
#endif

#ifdef Q_CC_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4100)
#endif

#endif

