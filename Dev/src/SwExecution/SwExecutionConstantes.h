/*!
\file SwExecutionConstantes.h
\date 28/04/2006
\brief Constantes de SwGui
\author  Big
\version 1.0
 */

#ifndef _SwExecutionConstantes_H
#define _SwExecutionConstantes_H

#include <Qt/qglobal.h>
//Export via les DLL

#ifdef SWEXECUTION_LIB
#define SWEXECUTION_EXPORT Q_DECL_EXPORT
#else
#define SWEXECUTION_EXPORT Q_DECL_IMPORT
#endif

#ifdef Q_CC_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4100)  
#pragma warning(disable:4250)
#endif

#endif

