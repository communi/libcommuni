######################################################################
# Communi
######################################################################

TARGET = communiplugin
QT += declarative
TARGETPATH = Communi
DESTDIR = ../../../imports/$$TARGETPATH

SOURCES += plugin.cpp
OTHER_FILES += qmldir

contains(MEEGO_EDITION,harmattan) {
    !no_rpath:QMAKE_RPATHDIR += /opt/communi/lib
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
    # TODO: TARGET.UID3 = 0xFFFFFFFF

    target.sources = $${TARGET}.dll
    target.path = $$QT_IMPORTS_BASE_DIR/$$TARGETPATH

    other_files.sources = $$OTHER_FILES
    other_files.path = $$QT_IMPORTS_BASE_DIR/$$TARGETPATH

    DEPLOYMENT += target other_files
} else {
    for(other_file, OTHER_FILES) {
        ARGUMENTS = $${PWD}$${QMAKE_DIR_SEP}$$other_file $$DESTDIR
        !isEmpty(QMAKE_POST_LINK):QMAKE_POST_LINK += &&
        QMAKE_POST_LINK += $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)
    }
}

include(../plugins.pri)
