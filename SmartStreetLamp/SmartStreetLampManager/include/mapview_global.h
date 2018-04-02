#ifndef MAPVIEW_GLOBAL_H
#define MAPVIEW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MAPVIEW_LIBRARY)
#  define MAPVIEWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAPVIEWSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAPVIEW_GLOBAL_H
