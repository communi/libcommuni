######################################################################
# Communi
######################################################################

TEMPLATE = subdirs
SUBDIRS += src
CONFIG += ordered

!no_tests {
    SUBDIRS += tests
} else {
    message(Tests disabled)
}

!no_examples {
    SUBDIRS += examples
} else {
    message(Examples disabled)
}

lessThan(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MAJOR_VERSION, 4) | lessThan(QT_MINOR_VERSION, 6) {
        error(Communi requires Qt 4.6 or newer but Qt $$[QT_VERSION] was detected.)
    }
}

QMAKE_CACHE = $${OUT_PWD}$${QMAKE_DIR_SEP}.qmake.cache
COMMUNI_CONFIG = $${OUT_PWD}$${QMAKE_DIR_SEP}communi-config.prf

static {
    system(echo DEFINES+=COMMUNI_STATIC > $$QMAKE_CACHE)
    system(echo DEFINES+=COMMUNI_STATIC > $$COMMUNI_CONFIG)
} else {
    system(echo DEFINES+=COMMUNI_SHARED > $$QMAKE_CACHE)
    system(echo DEFINES+=COMMUNI_SHARED > $$COMMUNI_CONFIG)
}

isEmpty(COMMUNI_INSTALL_LIBS):COMMUNI_INSTALL_LIBS = $$[QT_INSTALL_LIBS]
isEmpty(COMMUNI_INSTALL_BINS):COMMUNI_INSTALL_BINS = $$[QT_INSTALL_BINS]
isEmpty(COMMUNI_INSTALL_HEADERS):COMMUNI_INSTALL_HEADERS = $$[QT_INSTALL_HEADERS]/Communi

system(echo COMMUNI_INSTALL_LIBS=$$COMMUNI_INSTALL_LIBS >> $$QMAKE_CACHE)
system(echo COMMUNI_INSTALL_LIBS=$$COMMUNI_INSTALL_LIBS >> $$COMMUNI_CONFIG)
system(echo COMMUNI_INSTALL_BINS=$$COMMUNI_INSTALL_BINS >> $$QMAKE_CACHE)
system(echo COMMUNI_INSTALL_BINS=$$COMMUNI_INSTALL_BINS >> $$COMMUNI_CONFIG)
system(echo COMMUNI_INSTALL_HEADERS=$$COMMUNI_INSTALL_HEADERS >> $$QMAKE_CACHE)
system(echo COMMUNI_INSTALL_HEADERS=$$COMMUNI_INSTALL_HEADERS >> $$COMMUNI_CONFIG)

OTHER_FILES += astylerc
OTHER_FILES += AUTHORS
OTHER_FILES += CHANGELOG
OTHER_FILES += configure
OTHER_FILES += Doxyfile
OTHER_FILES += INSTALL
OTHER_FILES += LICENSE
OTHER_FILES += README
OTHER_FILES += features/communi.prf

mkspecs.files += features/communi.prf
mkspecs.files += $$OUT_PWD/communi-config.prf
mkspecs.path = $$[QT_INSTALL_DATA]/mkspecs/features
INSTALLS += mkspecs

include(version.pri)
!build_pass {
    macx {
        !qt_no_framework {
            message(Building Communi $$COMMUNI_VERSION (framework))
        } else {
            message(Building Communi $$COMMUNI_VERSION (dylib))
        }
    } else {
        message(Building Communi $$COMMUNI_VERSION)
    }
}

lessThan(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MAJOR_VERSION, 4) | lessThan(QT_MINOR_VERSION, 7) {
        message(Declarative support disabled. Use Qt 4.7 or later to enable declarative support.)
    }
}
