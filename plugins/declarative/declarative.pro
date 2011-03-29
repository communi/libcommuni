######################################################################
# LibIrcClient-Qt
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(ircplugin)
DEFINES += BUILD_PLUGIN
QT += declarative
CONFIG += plugin
!symbian:CONFIG += silent
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all

DESTDIR = ../../imports/org/bitbucket/irc
DEPENDPATH += . ../../include
INCLUDEPATH += . ../../include
macx:CONFIG(qt_framework, qt_framework|qt_no_framework) {
    LIBS += -F../../lib -framework ircclient-qt
} else {
    LIBS += -L../../lib -l$$qtLibraryTarget(ircclient-qt)
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

SOURCES += ircplugin.cpp
OTHER_FILES += qmldir

target.path = $$[QT_INSTALL_IMPORTS]/org/bitbucket/irc
INSTALLS += target

other_files.files = $$OTHER_FILES
other_files.path = $$[QT_INSTALL_IMPORTS]/org/bitbucket/irc
INSTALLS += other_files

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = NetworkServices
    # TODO: TARGET.UID3 = 0x12345678

    target.sources = ircplugin.dll
    target.path = /resource/qt/imports/org/bitbucket/irc

    other_files.sources = $$OTHER_FILES
    other_files.path = /resource/qt/imports/org/bitbucket/irc

    DEPLOYMENT += target other_files
}

for(other_file, OTHER_FILES) {
    ARGUMENTS = $$other_file $$DESTDIR
    !isEmpty(QMAKE_POST_LINK):QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)
}
