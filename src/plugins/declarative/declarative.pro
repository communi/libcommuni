######################################################################
# Communi
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(communiplugin)
DEFINES += BUILD_PLUGIN
QT += declarative
CONFIG += plugin
!verbose:!symbian:CONFIG += silent
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all

TARGETPATH = Communi
DESTDIR = ../../../imports/$$TARGETPATH
DEPENDPATH += . ../../../include
INCLUDEPATH += . ../../../include
macx:CONFIG(qt_framework, qt_framework|qt_no_framework) {
    LIBS += -F../../../lib -framework Communi
} else {
    LIBS += -L../../../lib -l$$qtLibraryTarget(Communi)
}
!symbian {
    CONFIG(debug, debug|release) {
        OBJECTS_DIR = debug
        MOC_DIR = debug
    } else {
        OBJECTS_DIR = release
        MOC_DIR = release
    }
}

SOURCES += plugin.cpp
OTHER_FILES += qmldir

contains(MEEGO_EDITION,harmattan) {
    QMAKE_RPATHDIR += /opt/communi/lib
    COMMUNI_INSTALL_IMPORTS = /opt/communi/imports
} else {
    COMMUNI_INSTALL_IMPORTS = $$[QT_INSTALL_IMPORTS]
}

target.path = $$COMMUNI_INSTALL_IMPORTS/$$TARGETPATH
INSTALLS += target

other_files.files = $$OTHER_FILES
other_files.path = $$COMMUNI_INSTALL_IMPORTS/$$TARGETPATH
INSTALLS += other_files

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = NetworkServices

    target.sources = communiplugin.dll
    target.path = /resource/qt/imports/$$TARGETPATH

    other_files.sources = $$OTHER_FILES
    other_files.path = /resource/qt/imports/$$TARGETPATH

    DEPLOYMENT += target other_files
} else {
    for(other_file, OTHER_FILES) {
        ARGUMENTS = $${PWD}$${QMAKE_DIR_SEP}$$other_file $$DESTDIR
        !isEmpty(QMAKE_POST_LINK):QMAKE_POST_LINK += &&
        QMAKE_POST_LINK += $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)
    }
}
