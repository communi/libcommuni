######################################################################
# Communi
######################################################################

QT += network
DEFINES += BUILD_COMMUNI

DEPENDPATH += $$PWD $$PWD/../include
INCLUDEPATH += $$PWD $$PWD/../include

CONV_HEADERS += $$PWD/../include/Irc
CONV_HEADERS += $$PWD/../include/IrcChannel
CONV_HEADERS += $$PWD/../include/IrcChannelModel
CONV_HEADERS += $$PWD/../include/IrcCommand
CONV_HEADERS += $$PWD/../include/IrcGlobal
CONV_HEADERS += $$PWD/../include/IrcLagTimer
CONV_HEADERS += $$PWD/../include/IrcMessage
CONV_HEADERS += $$PWD/../include/IrcMessageFilter
CONV_HEADERS += $$PWD/../include/IrcPalette
CONV_HEADERS += $$PWD/../include/IrcSender
CONV_HEADERS += $$PWD/../include/IrcSession
CONV_HEADERS += $$PWD/../include/IrcSessionInfo
CONV_HEADERS += $$PWD/../include/IrcTextFormat
CONV_HEADERS += $$PWD/../include/IrcUser
CONV_HEADERS += $$PWD/../include/IrcUserModel

PUB_HEADERS += $$PWD/../include/irc.h
PUB_HEADERS += $$PWD/../include/ircchannel.h
PUB_HEADERS += $$PWD/../include/ircchannelmodel.h
PUB_HEADERS += $$PWD/../include/irccommand.h
PUB_HEADERS += $$PWD/../include/ircglobal.h
PUB_HEADERS += $$PWD/../include/irclagtimer.h
PUB_HEADERS += $$PWD/../include/ircmessage.h
PUB_HEADERS += $$PWD/../include/ircmessagefilter.h
PUB_HEADERS += $$PWD/../include/ircpalette.h
PUB_HEADERS += $$PWD/../include/ircprotocol.h
PUB_HEADERS += $$PWD/../include/ircsender.h
PUB_HEADERS += $$PWD/../include/ircsession.h
PUB_HEADERS += $$PWD/../include/ircsessioninfo.h
PUB_HEADERS += $$PWD/../include/irctextformat.h
PUB_HEADERS += $$PWD/../include/ircuser.h
PUB_HEADERS += $$PWD/../include/ircusermodel.h

PRIV_HEADERS += $$PWD/../include/ircchannel_p.h
PRIV_HEADERS += $$PWD/../include/ircmessage_p.h
PRIV_HEADERS += $$PWD/../include/ircmessagebuilder_p.h
PRIV_HEADERS += $$PWD/../include/ircmessagedecoder_p.h
PRIV_HEADERS += $$PWD/../include/ircmessagefilter_p.h
PRIV_HEADERS += $$PWD/../include/ircsession_p.h
PRIV_HEADERS += $$PWD/../include/ircuser_p.h

HEADERS += $$PUB_HEADERS
HEADERS += $$PRIV_HEADERS

SOURCES += $$PWD/irc.cpp
SOURCES += $$PWD/ircchannel.cpp
SOURCES += $$PWD/ircchannelmodel.cpp
SOURCES += $$PWD/irccommand.cpp
SOURCES += $$PWD/irclagtimer.cpp
SOURCES += $$PWD/ircmessage.cpp
SOURCES += $$PWD/ircmessage_p.cpp
SOURCES += $$PWD/ircmessagebuilder.cpp
SOURCES += $$PWD/ircmessagedecoder.cpp
SOURCES += $$PWD/ircmessagefilter.cpp
SOURCES += $$PWD/ircpalette.cpp
SOURCES += $$PWD/ircprotocol.cpp
SOURCES += $$PWD/ircsender.cpp
SOURCES += $$PWD/ircsession.cpp
SOURCES += $$PWD/ircsessioninfo.cpp
SOURCES += $$PWD/irctextformat.cpp
SOURCES += $$PWD/ircuser.cpp
SOURCES += $$PWD/ircusermodel.cpp

include(3rdparty/mozilla/mozilla.pri)

# respect the (no_)icu/(no_)uchardet config if specified,
# otherwise try to automatically detect using pkg-config
include(3rdparty/pkg.pri)
!icu:!no_icu:!uchardet:!no_uchardet {
    pkgExists(icu)|pkgExists(icu-i18n):CONFIG += icu
}

CONFIG(icu, icu|no_icu) {
    DEFINES += COMMUNI_ICU
    SOURCES += $$PWD/ircmessagedecoder_icu.cpp
    include(3rdparty/icu/icu.pri)
} else {
    DEFINES += COMMUNI_UCHARDET
    SOURCES += $$PWD/ircmessagedecoder_uchardet.cpp
    include(3rdparty/uchardet-0.0.1/uchardet.pri)
}
