######################################################################
# Communi
######################################################################

DEFINES += BUILD_COMMUNI_UTIL

INCDIR = $$PWD/../../include/IrcUtil

DEPENDPATH += $$PWD $$INCDIR
INCLUDEPATH += $$PWD $$INCDIR

CONV_HEADERS  = $$INCDIR/IrcCommandParser
CONV_HEADERS += $$INCDIR/IrcLagTimer
CONV_HEADERS += $$INCDIR/IrcPalette
CONV_HEADERS += $$INCDIR/IrcTextFormat

PUB_HEADERS  = $$INCDIR/irccommandparser.h
PUB_HEADERS += $$INCDIR/irclagtimer.h
PUB_HEADERS += $$INCDIR/ircpalette.h
PUB_HEADERS += $$INCDIR/irctextformat.h

PRIV_HEADERS  =

HEADERS += $$PUB_HEADERS
HEADERS += $$PRIV_HEADERS

SOURCES += $$PWD/irccommandparser.cpp
SOURCES += $$PWD/irclagtimer.cpp
SOURCES += $$PWD/ircpalette.cpp
SOURCES += $$PWD/irctextformat.cpp
