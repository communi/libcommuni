######################################################################
# Communi
######################################################################

TARGET = tst_$$basename(_PRO_FILE_PWD_)

DEPENDPATH += $$_PRO_FILE_PWD_
INCLUDEPATH += $$_PRO_FILE_PWD_

CONFIG += install_name no_testcase_installs

!verbose:CONFIG += silent

CONFIG(debug, debug|release) {
    OBJECTS_DIR = debug
    MOC_DIR = debug
} else {
    OBJECTS_DIR = release
    MOC_DIR = release
}

greaterThan(QT_MAJOR_VERSION,5): QT += core5compat

IRC_BUILDDIR = $$OUT_PWD/../../..

IRC_MODULES = IrcCore IrcModel IrcUtil
include(../src/module_deps.pri)
