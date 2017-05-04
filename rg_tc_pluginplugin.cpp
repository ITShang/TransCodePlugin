#include "rg_tc_pluginplugin.h"
#include "rg_tc_pluginconstants.h"
#include "mainwindow.h"
#include "transcurrcode.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <QtPlugin>
#include <QApplication>

using namespace RG_TC_Plugin::Internal;

RG_TC_PluginPlugin::RG_TC_PluginPlugin()
{
    // Create your members
    widget = NULL;
    appTranslator.load(tr("rg_tc_zh_ZN.qm"),tr("://"));
    QApplication::installTranslator(&appTranslator);
}

RG_TC_PluginPlugin::~RG_TC_PluginPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
    if(widget){delete widget;widget =NULL;}
}

bool RG_TC_PluginPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize method, a plugin can be sure that the plugins it
    // depends on have initialized their members.
    
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)
    
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(code);
    QTextCodec::setCodecForCStrings(code);
    QTextCodec::setCodecForTr(code);

    QAction *action = new QAction(tr("TransView"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));

    //转换为UTF8
    QAction *action_utf8 = new QAction(tr("To UTF8"), this);
    Core::Command *tUTF8 = Core::ActionManager::registerAction(action_utf8, Constants::ACTION_ID_UTF8,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    connect(action_utf8, SIGNAL(triggered()), this, SLOT(transToUTF8()));

    //转换为GB2312
    QAction *action_gb2312 = new QAction(tr("To Gb2312"), this);
    Core::Command *tGB2312 = Core::ActionManager::registerAction(action_gb2312, Constants::ACTION_ID_GB2312,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    connect(action_gb2312, SIGNAL(triggered()), this, SLOT(transToGB2312()));

    //添加直接转换编码列表
    Core::ActionContainer *menuList = Core::ActionManager::createMenu(Constants::MENU_ID_CODE_LIST);
    menuList->menu()->setTitle(tr("Trans Curr File"));
    menuList->addAction(tUTF8);
    menuList->addAction(tGB2312);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle((tr("Trans Code")));
    menu->addAction(cmd);
    menu->addMenu(menuList);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);
    
    return true;
}

void RG_TC_PluginPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized method, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag RG_TC_PluginPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void RG_TC_PluginPlugin::triggerAction()
{
//    QMessageBox::information(Core::ICore::mainWindow(),
//                             tr("Action triggered"),
//                             tr("This is an action from RG_TC_Plugin."));
    if(!widget)
    {
        widget = new MainWindow();
    }
    widget->show();
}

void RG_TC_PluginPlugin::transToUTF8()
{
    TransCurrCode tcurr;
    tcurr.setTargetCode(tr("UTF-8"));
    tcurr.transCurrFileCode();
}

void RG_TC_PluginPlugin::transToGB2312()
{
    TransCurrCode tcurr;
    tcurr.setTargetCode(tr("GB2312"));
    tcurr.transCurrFileCode();
}

Q_EXPORT_PLUGIN2(RG_TC_Plugin, RG_TC_PluginPlugin)

