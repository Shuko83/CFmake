/*!
\file SwDocConstantes.h
\date 28/04/2006
\brief Constantes de SwWizard
\author  Big
\version 1.0
 */

#ifndef _SWDOCCONSTANTES_H
#define _SWDOCCONSTANTES_H

#include <QGlobal.h>
//Export via les DLL

#ifdef SWDOC_LIB
# define BUILD_SWDOC Q_DECL_EXPORT
#else
# define BUILD_SWDOC Q_DECL_IMPORT
#endif

#ifdef Q_CC_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4100)
#endif

#endif

