######################################################################
# Communi
######################################################################

TEMPLATE = lib
TARGET = $$qtLibraryTarget(communiplugin)
DEFINES += BUILD_PLUGIN
QT += declarative
CONFIG += plugin
!symbian:CONFIG += silent
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all

TARGETPATH = org/gitorious/communi
DESTDIR = ../../imports/$$TARGETPATH
DEPENDPATH += . ../../include
INCLUDEPATH += . ../../include
macx:CONFIG(qt_framework, qt_framework|qt_no_framework) {
    LIBS += -F../../lib -framework Communi
} else {
    LIBS += -L../../lib -l$$qtLibraryTarget(Communi)
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

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
INSTALLS += target

other_files.files = $$OTHER_FILES
other_files.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
INSTALLS += other_files

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = NetworkServices
    # TODO: TARGET.UID3 = 0x12345678

    target.sources = ircplugin.dll
    target.path = /resource/qt/imports/$$TARGETPATH

    other_files.sources = $$OTHER_FILES
    other_files.path = /resource/qt/imports/$$TARGETPATH

    DEPLOYMENT += target other_files
}

for(other_file, OTHER_FILES) {
    ARGUMENTS = $$other_file $$DESTDIR
    !isEmpty(QMAKE_POST_LINK):QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)
}
