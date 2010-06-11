/*!
\file SwWizardConstantes.h
\date 28/04/2006
\brief Constantes de SwWizard
\author  Big
\version 1.0
 */

#ifndef _SwWizardConstantes_H
#define _SwWizardConstantes_H

#include <Qt/qglobal.h>
//Export via les DLL

#ifdef WIZARDIDE_LIB
# define BUILD_SWWIZARD Q_DECL_EXPORT
#else
# define BUILD_SWWIZARD Q_DECL_IMPORT
#endif

#ifdef Q_CC_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4100)
#endif

#endif

