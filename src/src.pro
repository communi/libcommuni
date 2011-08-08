######################################################################
# Communi
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(Communi)
DEFINES += BUILD_COMMUNI
QT = core network
!symbian:CONFIG += silent
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all

COMMUNI_VERSION = 1.0.0
!win32:VERSION = $$COMMUNI_VERSION

DESTDIR = ../lib
DLLDESTDIR = ../bin
DEPENDPATH += . ../include
INCLUDEPATH += . ../include
!symbian {
    CONFIG(debug, debug|release) {
        OBJECTS_DIR = debug
        MOC_DIR = debug
    } else {
        OBJECTS_DIR = release
        MOC_DIR = release
    }
}

CONV_HEADERS += ../include/Irc
CONV_HEADERS += ../include/IrcCommand
CONV_HEADERS += ../include/IrcGlobal
CONV_HEADERS += ../include/IrcMessage
CONV_HEADERS += ../include/IrcMessageFilter
CONV_HEADERS += ../include/IrcPrefix
CONV_HEADERS += ../include/IrcReceiver
CONV_HEADERS += ../include/IrcSession
CONV_HEADERS += ../include/IrcUtil

PUB_HEADERS += ../include/irc.h
PUB_HEADERS += ../include/irccommand.h
PUB_HEADERS += ../include/ircglobal.h
PUB_HEADERS += ../include/ircmessage.h
PUB_HEADERS += ../include/ircmessagefilter.h
PUB_HEADERS += ../include/ircprefix.h
PUB_HEADERS += ../include/ircreceiver.h
PUB_HEADERS += ../include/ircsession.h
PUB_HEADERS += ../include/ircutil.h

PRIV_HEADERS += ../include/ircmessagebuilder_p.h
PRIV_HEADERS += ../include/ircparser_p.h
PRIV_HEADERS += ../include/ircsession_p.h

HEADERS += $$PUB_HEADERS
HEADERS += $$PRIV_HEADERS

SOURCES += irc.cpp
SOURCES += irccommand.cpp
SOURCES += ircmessage.cpp
SOURCES += ircmessagebuilder.cpp
SOURCES += ircmessagefilter.cpp
SOURCES += ircparser.cpp
SOURCES += ircprefix.cpp
SOURCES += ircreceiver.cpp
SOURCES += ircsession.cpp
SOURCES += ircutil.cpp

target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

dlltarget.path = $$[QT_INSTALL_BINS]
INSTALLS += dlltarget

macx:CONFIG(qt_framework, qt_framework|qt_no_framework) {
    CONFIG += lib_bundle debug_and_release
    CONFIG(debug, debug|release) {
        !build_pass:CONFIG += build_all
    } else { #release
        !debug_and_release|build_pass {
            FRAMEWORK_HEADERS.version = Versions
            FRAMEWORK_HEADERS.files = $$PUB_HEADERS $$CONV_HEADERS
            FRAMEWORK_HEADERS.path = Headers
        }
        QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
    }
    QMAKE_LFLAGS_SONAME = -Wl,-install_name,$$[QT_INSTALL_LIBS]/
} else {
    headers.files = $$PUB_HEADERS $$CONV_HEADERS
    headers.path = $$[QT_INSTALL_HEADERS]/Communi
    INSTALLS += headers
}

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = NetworkServices
    TARGET.UID3 = 0xEAF16DB1
    MMP_RULES += EXPORTUNFROZEN

    for(header, headers.files) {
        BLD_INF_RULES.prj_exports += "$$header $$MW_LAYER_PUBLIC_EXPORT_PATH($$basename(header))"
    }

    library.sources = Communi.dll
    library.path = !:/sys/bin
    DEPLOYMENT += library
}
