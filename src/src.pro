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

DEFINES += HAVE_ICU
contains(DEFINES, HAVE_ICU) {
    win32:INCLUDEPATH += C:\ICU\include
    win32:LIBS += -LC:\ICU\lib
    win32:LIBS += -l$$qtLibraryTarget(icuin)
    unix:LIBS += -licui18n
    LIBS += -l$$qtLibraryTarget(icuuc)
}

HEADERS += ../include/coreircsession.h ../include/global.h ../include/irc.h ../include/ircsession.h
SOURCES += coreircsession.cpp irc.cpp ircsession.cpp

headers.files = $$HEADERS
headers.path = $$[QT_INSTALL_HEADERS]/ircclient-qt
INSTALLS += headers

target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

dlltarget.path = $$[QT_INSTALL_BINS]
INSTALLS += dlltarget
