######################################################################
# LibIrcClient-Qt
######################################################################

TEMPLATE = lib
DESTDIR = lib
TARGET = $$qtLibraryTarget(ircclient-qt)
DEPENDPATH += include src
INCLUDEPATH += include
DEFINES += BUILD_LIBIRCCLIENT_QT
QT = core network

DEFINES += HAVE_ICU
contains(DEFINES, HAVE_ICU) {
    win32:INCLUDEPATH += C:\ICU\include
    win32:LIBS += -LC:\ICU\lib
    win32:LIBS += -l$$qtLibraryTarget(icuin)
    unix:LIBS += -licui18n
    LIBS += -l$$qtLibraryTarget(icuuc)
}

HEADERS += irc.h ircsession.h ircutil.h
SOURCES += irc.cpp ircsession.cpp ircutil.cpp

headers.files = $$HEADERS
headers.path = $$[QT_INSTALL_HEADERS]/ircclient-qt
INSTALLS += headers

target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

dlltarget.path = $$[QT_INSTALL_BINS]
INSTALLS += dlltarget
