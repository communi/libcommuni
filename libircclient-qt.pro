######################################################################
# LibIrcClient-Qt
######################################################################

TEMPLATE = lib
DEFINES += BUILD_IRC
QT = core network
CONFIG += silent
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII

DESTDIR = lib
DEPENDPATH += include src
INCLUDEPATH += include
OBJECTS_DIR = .tmp
MOC_DIR = .tmp

DEFINES += HAVE_ICU
contains(DEFINES, HAVE_ICU) {
    win32:INCLUDEPATH += C:\ICU\include
    win32:LIBS += -LC:\ICU\lib
    win32:LIBS += -l$$qtLibraryTarget(icuin)
    unix:LIBS += -licui18n -licudata
    LIBS += -l$$qtLibraryTarget(icuuc)
}
TARGET = $$qtLibraryTarget(ircclient-qt)

CONV_HEADERS += include/Irc include/IrcDccSession include/IrcGlobal include/IrcSession include/IrcUtil
HEADERS += irc.h ircdccsession.h ircglobal.h ircsession.h ircutil.h
SOURCES += irc.cpp ircdccsession.cpp ircsession.cpp ircutil.cpp

mac {
    CONFIG += lib_bundle
    FRAMEWORK_HEADERS.version = Versions
    FRAMEWORK_HEADERS.files = $$HEADERS
    FRAMEWORK_HEADERS.path = Headers
    QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
}

mkspecs.files = libircclient-qt.prf
mkspecs.path = $$[QT_INSTALL_DATA]/mkspecs/features
INSTALLS += mkspecs

headers.files = $$HEADERS $$CONV_HEADERS
headers.path = $$[QT_INSTALL_HEADERS]/ircclient-qt
INSTALLS += headers

target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

dlltarget.path = $$[QT_INSTALL_BINS]
INSTALLS += dlltarget

include(coverage.pri)
