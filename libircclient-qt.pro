######################################################################
# LibIrcClient-Qt
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(ircclient-qt)
DEFINES += BUILD_IRC
QT = core network
CONFIG += silent
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII

DESTDIR = lib
DEPENDPATH += include src
INCLUDEPATH += include
OBJECTS_DIR = .tmp
MOC_DIR = .tmp

macx {
    CONFIG += lib_bundle
    FRAMEWORK_HEADERS.version = Versions
    FRAMEWORK_HEADERS.files = $$HEADERS
    FRAMEWORK_HEADERS.path = Headers
    QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
}

CONV_HEADERS += include/Irc include/IrcDccSession include/IrcGlobal include/IrcSession include/IrcUtil
HEADERS += irc.h ircdccsession.h ircglobal.h ircsession.h ircutil.h
SOURCES += irc.cpp ircdccsession.cpp ircsession.cpp ircutil.cpp

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

include(icu.pri)
include(coverage.pri)
