######################################################################
# Communi
######################################################################

TEMPLATE = app
TARGET = embed
QT += network

DEFINES += IRC_NAMESPACE=Communi
include(../../../src/communi.pri)

SOURCES += main.cpp
