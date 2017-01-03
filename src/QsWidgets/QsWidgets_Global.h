#pragma once

#include <QtCore/qglobal.h>

#ifdef QSWIDGETS_LIB
# define QSWIDGETS_EXPORT Q_DECL_EXPORT
#else
# define QSWIDGETS_EXPORT Q_DECL_IMPORT
#endif
