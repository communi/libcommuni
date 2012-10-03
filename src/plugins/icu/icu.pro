######################################################################
# Communi
######################################################################

TARGET = icuplugin
DESTDIR = ../../../plugins/communi

SOURCES += plugin.cpp
include(icu.pri)

isEmpty(COMMUNI_INSTALL_PLUGINS) {
    contains(MEEGO_EDITION,harmattan) {
        !no_rpath:QMAKE_RPATHDIR += /opt/communi/lib
        COMMUNI_INSTALL_PLUGINS = /opt/communi/plugins
    } else {
        COMMUNI_INSTALL_PLUGINS = $$[QT_INSTALL_PLUGINS]
    }
}

target.path = $$COMMUNI_INSTALL_PLUGINS/communi
INSTALLS += target

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = NetworkServices
    # TODO: TARGET.UID3 = 0xFFFFFFFF

    target.sources = $${TARGET}.dll
    target.path = $$QT_PLUGINS_BASE_DIR/communi

    DEPLOYMENT += target
}

include(../plugins.pri)
