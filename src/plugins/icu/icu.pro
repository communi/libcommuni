######################################################################
# Communi
######################################################################

TARGET = icuplugin
DESTDIR = ../../../plugins/communi
static_icu:CONFIG += static

SOURCES += plugin.cpp
include(icu.pri)

isEmpty(COMMUNI_INSTALL_PLUGINS):COMMUNI_INSTALL_PLUGINS = $$[QT_INSTALL_PLUGINS]

target.path = $$COMMUNI_INSTALL_PLUGINS/communi
INSTALLS += target

include(../plugins.pri)
