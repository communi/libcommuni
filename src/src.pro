######################################################################
# LibIrcClient-Qt
######################################################################

TEMPLATE = lib
DESTDIR = ../lib
TARGET = $$qtLibraryTarget(ircclient-qt)
DEPENDPATH += . ../include
INCLUDEPATH += . ../include ../3rdparty/libircclient/include
LIBS += -L../3rdparty/libircclient/lib -lircclient
DEFINES += BUILD_LIBIRCCLIENT_QT
win32:LIBS += -lWs2_32
QT = core

HEADERS += ../include/global.h ../include/irc.h ../include/ircsession.h
SOURCES += irc.cpp ircsession.cpp

headers.files = $$HEADERS
headers.path = $$[QT_INSTALL_HEADERS]/ircclient-qt
INSTALLS += headers

target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

dlltarget.path = $$[QT_INSTALL_BINS]
INSTALLS += dlltarget
