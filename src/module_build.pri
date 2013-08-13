######################################################################
# Communi
######################################################################

isEmpty(IRC_MODULE):error(IRC_MODULE must be set)

TEMPLATE = lib
TARGET = $$qtLibraryTarget($$IRC_MODULE)
QT = core network
!verbose:CONFIG += silent
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all

include(../version.pri)
!win32:VERSION = $$COMMUNI_VERSION

isEmpty(COMMUNI_BUILDDIR):COMMUNI_BUILDDIR = $$OUT_PWD/../..

DESTDIR = $$COMMUNI_BUILDDIR/lib
DLLDESTDIR = $$COMMUNI_BUILDDIR/bin

CONFIG(debug, debug|release) {
    OBJECTS_DIR = debug
    MOC_DIR = debug
} else {
    OBJECTS_DIR = release
    MOC_DIR = release
}
