######################################################################
# Communi
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(uchardetplugin)
DEFINES += BUILD_PLUGIN
CONFIG += plugin
!verbose:!symbian:CONFIG += silent
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all

DESTDIR = ../../../plugins
DEPENDPATH += . ../../../include
INCLUDEPATH += . ../../../include
macx:CONFIG(qt_framework, qt_framework|qt_no_framework) {
    LIBS += -F../../../lib -framework Communi
} else {
    LIBS += -L../../../lib -l$$qtLibraryTarget(Communi)
}
!symbian {
    CONFIG(debug, debug|release) {
        OBJECTS_DIR = debug
        MOC_DIR = debug
    } else {
        OBJECTS_DIR = release
        MOC_DIR = release
    }
}

SOURCES += plugin.cpp
include(../../3rdparty/uchardet-0.0.1/uchardet.pri)

contains(MEEGO_EDITION,harmattan) {
    QMAKE_RPATHDIR += /opt/communi/lib
    COMMUNI_INSTALL_PLUGINS = /opt/communi/plugins
} else {
    COMMUNI_INSTALL_PLUGINS = $$[QT_INSTALL_PLUGINS]/communi
}

target.path = $$COMMUNI_INSTALL_PLUGINS
INSTALLS += target

symbian {
    TARGET.EPOCALLOWDLLDATA = 1

    target.sources = uchardetplugin.dll
    target.path = /resource/qt/plugins/communi

    DEPLOYMENT += target
}
