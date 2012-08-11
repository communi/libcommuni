######################################################################
# Communi
######################################################################

TARGET = $$qtLibraryTarget(uchardetplugin)
DESTDIR = ../../../plugins/communi

SOURCES += plugin.cpp
include(../../3rdparty/uchardet-0.0.1/uchardet.pri)

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

    target.sources = uchardetplugin.dll
    target.path = /resource/qt/plugins/communi

    DEPLOYMENT += target
}

include(../plugins.pri)
