DEFINES += RG_TC_PLUGIN_LIBRARY

# RG_TC_Plugin files

SOURCES += rg_tc_pluginplugin.cpp \
    rgtranscode.cpp \
    prelookfile.cpp \
    mytablemodel.cpp \
    mypushdelegate.cpp \
    mainwindow.cpp \
    transcurrcode.cpp

HEADERS += rg_tc_pluginplugin.h \
        rg_tc_plugin_global.h \
        rg_tc_pluginconstants.h \
    rgtranscode.h \
    prelookfile.h \
    mytablemodel.h \
    mypushdelegate.h \
    myheader.h \
    mainwindow.h \
    transcurrcode.h

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=E:/Qt/Tools/qt-creator-2.8.0-src

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=E:/Qt/485Compile/VS2010/dcreator

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

PROVIDER = RGCompany

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

FORMS += \
    prelookfile.ui \
    mainwindow.ui

RESOURCES += \
    image.qrc

#DEFINES += QT_NO_CAST_FROM_ASCII \
#           QT_NO_CAST_TO_ASCII

