######################################################################
# LibIrcClient-Qt
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(ircclient-qt)
DEFINES += BUILD_IRC
QT = core network
!symbian:CONFIG += silent
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII

IRC_VERSION = 0.6.0
!win32:VERSION = $$IRC_VERSION

DESTDIR = lib
DLLDESTDIR = bin
DEPENDPATH += include src
INCLUDEPATH += include
!symbian {
    CONFIG(debug, debug|release) {
        OBJECTS_DIR = debug
        MOC_DIR = debug
    } else {
        OBJECTS_DIR = release
        MOC_DIR = release
    }
}

static {
    DEFINES += IRC_STATIC
    system(echo DEFINES+=IRC_STATIC > libircclient-qt-config.prf)
} else {
    DEFINES += IRC_SHARED
    system(echo DEFINES+=IRC_SHARED > libircclient-qt-config.prf)
}

CONV_HEADERS += include/Irc
CONV_HEADERS += include/IrcBuffer
CONV_HEADERS += include/IrcDccSession
CONV_HEADERS += include/IrcGlobal
CONV_HEADERS += include/IrcSession
CONV_HEADERS += include/IrcUtil

PUB_HEADERS += include/irc.h
PUB_HEADERS += include/ircbuffer.h
PUB_HEADERS += include/ircdccsession.h
PUB_HEADERS += include/ircglobal.h
PUB_HEADERS += include/ircsession.h
PUB_HEADERS += include/ircutil.h

PRIV_HEADERS += include/ircbuffer_p.h
PRIV_HEADERS += include/ircsession_p.h

HEADERS += $$PUB_HEADERS
HEADERS += $$PRIV_HEADERS

SOURCES += irc.cpp
SOURCES += ircbuffer.cpp
SOURCES += ircdccsession.cpp
SOURCES += ircsession.cpp
SOURCES += ircutil.cpp

mkspecs.files = libircclient-qt.prf libircclient-qt-config.prf
mkspecs.path = $$[QMAKE_MKSPECS]/features
INSTALLS += mkspecs

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
    headers.path = $$[QT_INSTALL_HEADERS]/ircclient-qt
    INSTALLS += headers
}

!build_pass {
    macx {
        # See above for an explanation of this.
        !qt_no_framework {
            message(Building LibIrcClient-Qt $$IRC_VERSION (framework))
        } else {
            message(Building LibIrcClient-Qt $$IRC_VERSION (dylib))
        }
    } else {
        message(Building LibIrcClient-Qt $$IRC_VERSION)
    }

    !no_icu {
        message(ICU support enabled. Run \'qmake -config no_icu\' to disable ICU support.)
        include(icu.pri)
    } else {
        message(ICU support disabled.)
    }

    !no_deprecated {
        message(Deprecated functionality enabled. Run \'qmake -config no_deprecated\' to disable deprecated functionality.)
    } else {
        message(Deprecated functionality disabled.)
        DEFINES += IRC_NO_DEPRECATED
    }
}

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = NetworkServices
    TARGET.UID3 = 0xEAF16DB1
    MMP_RULES += EXPORTUNFROZEN

    mkspecs.path ~= s/^[A-Za-z]:/ # strip drive letter
    BLD_INF_RULES.prj_exports += "libircclient-qt.prf $$mkspecs.path/libircclient-qt.prf"
    BLD_INF_RULES.prj_exports += "libircclient-qt-config.prf $$mkspecs.path/libircclient-qt-config.prf"

    for(header, headers.files) {
        BLD_INF_RULES.prj_exports += "$$header $$MW_LAYER_PUBLIC_EXPORT_PATH($$basename(header))"
    }

    library.sources = ircclient-qt.dll
    library.path = !:/sys/bin
    DEPLOYMENT += library
}
