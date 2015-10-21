#pragma once

#include <QtCore/QtGlobal>

#ifdef SWSERVICEALIAS_LIB
#  define ALIAS_SERVICE_EXPORT Q_DECL_EXPORT
#else
#  define ALIAS_SERVICE_EXPORT Q_DECL_IMPORT
#endif