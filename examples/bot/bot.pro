######################################################################
# Communi
######################################################################

TEMPLATE = app
TARGET = bot
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += console
CONFIG -= app_bundle
QT = core network

greaterThan(QT_MAJOR_VERSION,5): QT += core5compat

# Input
HEADERS += ircbot.h
SOURCES += ircbot.cpp main.cpp

include(../examples.pri)
