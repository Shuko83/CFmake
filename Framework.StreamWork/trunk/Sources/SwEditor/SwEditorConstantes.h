/*!
\file SwEditorConstantes.h
\date 10/12/2014
\brief Constantes de SwCore
\author  QBN
\version 1.0
 */

#ifndef _SwEditorConstantes_H
#define _SwEditorConstantes_H

#include <QGlobal.h>
//Export via les DLL

#ifdef SWEDITOR_LIB
# define BUILD_SWEDITOR Q_DECL_EXPORT
#else
# define BUILD_SWEDITOR Q_DECL_IMPORT
#endif

#endif

