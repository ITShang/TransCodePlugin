#ifndef RG_TC_PLUGIN_GLOBAL_H
#define RG_TC_PLUGIN_GLOBAL_H

#include <QtGlobal>

#if defined(RG_TC_PLUGIN_LIBRARY)
#  define RG_TC_PLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RG_TC_PLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RG_TC_PLUGIN_GLOBAL_H

