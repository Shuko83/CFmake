/*!
\file SwGuiConstantes.h
\date 28/04/2006
\brief Constantes de SwGui
\author  Big
\version 1.0
 */

#ifndef _SwGuiConstantes_H
#define _SwGuiConstantes_H

#include <Qt/qglobal.h>
//Export via les DLL

#ifdef SW_EDITOR
#define SWGUI_EXPORT
#else
#ifdef SWGUI_LIB
# define SWGUI_EXPORT Q_DECL_EXPORT
#else
# define SWGUI_EXPORT Q_DECL_IMPORT
#endif
#endif

#ifdef Q_CC_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4100)
#pragma warning(disable:4250)
#endif

#endif

