/*!
\file SwModelConstantes.h
\date 28/04/2006
\brief Constantes de SwGui
\author  Big
\version 1.0
 */

#ifndef _SwModelConstantes_H
#define _SwModelConstantes_H

#include <QtGlobal>
//Export via les DLL

#ifdef SWMODEL_LIB
#define SWMODEL_EXPORT Q_DECL_EXPORT
#else
#define SWMODEL_EXPORT Q_DECL_IMPORT
#endif

#ifdef Q_CC_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4100)
#endif

#endif

