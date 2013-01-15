######################################################################
# Communi
######################################################################

TARGET = uchardetplugin
DESTDIR = ../../../plugins/communi
static_uchardet:CONFIG += static

SOURCES += plugin.cpp
include(../../3rdparty/uchardet-0.0.1/uchardet.pri)

isEmpty(COMMUNI_INSTALL_PLUGINS):COMMUNI_INSTALL_PLUGINS = $$[QT_INSTALL_PLUGINS]

target.path = $$COMMUNI_INSTALL_PLUGINS/communi
INSTALLS += target

include(../plugins.pri)
