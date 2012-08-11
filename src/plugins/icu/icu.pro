######################################################################
# Communi
######################################################################

TARGET = $$qtLibraryTarget(icuplugin)
DESTDIR = ../../../plugins/communi

SOURCES += plugin.cpp
include(icu.pri)

contains(MEEGO_EDITION,harmattan) {
    QMAKE_RPATHDIR += /opt/communi/lib
    COMMUNI_INSTALL_PLUGINS = /opt/communi/plugins/communi
} else {
    COMMUNI_INSTALL_PLUGINS = $$[QT_INSTALL_PLUGINS]/communi
}

target.path = $$COMMUNI_INSTALL_PLUGINS
INSTALLS += target

symbian {
    TARGET.EPOCALLOWDLLDATA = 1

    target.sources = icuplugin.dll
    target.path = /resource/qt/plugins/communi

    DEPLOYMENT += target
}

include(../plugins.pri)
