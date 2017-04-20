#ifndef RG_TC_PLUGIN_H
#define RG_TC_PLUGIN_H

#include "rg_tc_plugin_global.h"

#include <extensionsystem/iplugin.h>

class MainWindow;

namespace RG_TC_Plugin {
namespace Internal {

class RG_TC_PluginPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "RG_TC_Plugin.json")
    
public:
    enum CodeType{
        UTF8,
        GB2312
    };
    RG_TC_PluginPlugin();
    ~RG_TC_PluginPlugin();
    
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();
    
private slots:
    void triggerAction();
    void transToUTF8();
    void transToGB2312();

private:
    MainWindow * widget;
};

} // namespace Internal
} // namespace RG_TC_Plugin

#endif // RG_TC_PLUGIN_H

