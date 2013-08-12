######################################################################
# Communi
######################################################################

DEFINES += BUILD_COMMUNI_CORE

INCDIR = $$PWD/../../include/IrcCore

DEPENDPATH += $$PWD $$INCDIR
INCLUDEPATH += $$PWD $$INCDIR

CONV_HEADERS  = $$INCDIR/Irc
CONV_HEADERS += $$INCDIR/IrcCommand
CONV_HEADERS += $$INCDIR/IrcGlobal
CONV_HEADERS += $$INCDIR/IrcMessage
CONV_HEADERS += $$INCDIR/IrcMessageFilter
CONV_HEADERS += $$INCDIR/IrcSender
CONV_HEADERS += $$INCDIR/IrcSession
CONV_HEADERS += $$INCDIR/IrcSessionInfo

PUB_HEADERS  = $$INCDIR/irc.h
PUB_HEADERS += $$INCDIR/irccommand.h
PUB_HEADERS += $$INCDIR/ircglobal.h
PUB_HEADERS += $$INCDIR/ircmessage.h
PUB_HEADERS += $$INCDIR/ircmessagefilter.h
PUB_HEADERS += $$INCDIR/ircprotocol.h
PUB_HEADERS += $$INCDIR/ircsender.h
PUB_HEADERS += $$INCDIR/ircsession.h
PUB_HEADERS += $$INCDIR/ircsessioninfo.h

PRIV_HEADERS  = $$INCDIR/ircmessage_p.h
PRIV_HEADERS += $$INCDIR/ircmessagebuilder_p.h
PRIV_HEADERS += $$INCDIR/ircmessagedecoder_p.h
PRIV_HEADERS += $$INCDIR/ircmessagefilter_p.h
PRIV_HEADERS += $$INCDIR/ircsession_p.h

HEADERS += $$PUB_HEADERS
HEADERS += $$PRIV_HEADERS

SOURCES += $$PWD/irc.cpp
SOURCES += $$PWD/irccommand.cpp
SOURCES += $$PWD/ircmessage.cpp
SOURCES += $$PWD/ircmessage_p.cpp
SOURCES += $$PWD/ircmessagebuilder.cpp
SOURCES += $$PWD/ircmessagedecoder.cpp
SOURCES += $$PWD/ircmessagefilter.cpp
SOURCES += $$PWD/ircprotocol.cpp
SOURCES += $$PWD/ircsender.cpp
SOURCES += $$PWD/ircsession.cpp
SOURCES += $$PWD/ircsessioninfo.cpp

include(../3rdparty/mozilla/mozilla.pri)

# respect the (no_)icu/(no_)uchardet config if specified,
# otherwise try to automatically detect using pkg-config
include(../3rdparty/pkg.pri)
!icu:!no_icu:!uchardet:!no_uchardet {
    pkgExists(icu)|pkgExists(icu-i18n):CONFIG += icu
}

CONFIG(icu, icu|no_icu) {
    DEFINES += COMMUNI_ICU
    SOURCES += $$PWD/ircmessagedecoder_icu.cpp
    include(../3rdparty/icu/icu.pri)
} else {
    DEFINES += COMMUNI_UCHARDET
    SOURCES += $$PWD/ircmessagedecoder_uchardet.cpp
    include(../3rdparty/uchardet-0.0.1/uchardet.pri)
}
