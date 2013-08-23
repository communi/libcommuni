######################################################################
# Communi
######################################################################

isEmpty(IRC_MODULE):error(IRC_MODULE must be set)

TEMPLATE = lib
TARGET = $$qtLibraryTarget($$IRC_MODULE)
QT = core network
!verbose:CONFIG += silent
contains(QT_CONFIG, debug_and_release) {
    win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
}

include(../version.pri)
!win32:VERSION = $$IRC_VERSION

isEmpty(IRC_BUILDDIR):IRC_BUILDDIR = $$OUT_PWD/../..

DESTDIR = $$IRC_BUILDDIR/lib
DLLDESTDIR = $$IRC_BUILDDIR/bin

CONFIG(debug, debug|release) {
    OBJECTS_DIR = debug
    MOC_DIR = debug
} else {
    OBJECTS_DIR = release
    MOC_DIR = release
}
