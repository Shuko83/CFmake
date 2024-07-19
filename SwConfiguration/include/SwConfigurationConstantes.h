/*!
\file _SwConfigurationConstantes.h
\date 28/04/2006
\brief Constantes de configuration
\author  Gbs
\version 1.0
 */

#ifndef __SwConfigurationConstantes_H
#define __SwConfigurationConstantes_H

#include <QtGlobal>
//Export via les DLL

#ifdef SWCONFIGURATION_LIB
#define SWCONFIGURATION_EXPORT Q_DECL_EXPORT
#else
#define SWCONFIGURATION_EXPORT Q_DECL_IMPORT
#endif

#ifdef Q_CC_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4100)
#endif

#endif

