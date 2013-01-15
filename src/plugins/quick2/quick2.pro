######################################################################
# Communi
######################################################################

TARGET = communiplugin
QT += quick
TARGETPATH = Communi
DESTDIR = ../../../imports/$$TARGETPATH

SOURCES += plugin.cpp
OTHER_FILES += qmldir

isEmpty(COMMUNI_INSTALL_QML):COMMUNI_INSTALL_QML = $$[QT_INSTALL_QML]

target.path = $$COMMUNI_INSTALL_QML/$$TARGETPATH
INSTALLS += target

other_files.files = $$OTHER_FILES
other_files.path = $$COMMUNI_INSTALL_QML/$$TARGETPATH
INSTALLS += other_files

for(other_file, OTHER_FILES) {
    ARGUMENTS = $${PWD}$${QMAKE_DIR_SEP}$$other_file $$DESTDIR
    !isEmpty(QMAKE_POST_LINK):QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)
}

include(../plugins.pri)
