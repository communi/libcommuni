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

CONFIG_VARS = $${OUT_PWD}$${QMAKE_DIR_SEP}.config.vars
QMAKE_CACHE = $${OUT_PWD}$${QMAKE_DIR_SEP}.qmake.cache
IRC_CONFIG = $${OUT_PWD}$${QMAKE_DIR_SEP}communi-config.prf

static {
    system(echo DEFINES+=IRC_STATIC > $$QMAKE_CACHE)
    system(echo DEFINES+=IRC_STATIC > $$IRC_CONFIG)
} else {
    system(echo DEFINES+=IRC_SHARED > $$QMAKE_CACHE)
    system(echo DEFINES+=IRC_SHARED > $$IRC_CONFIG)
}

exists($$CONFIG_VARS) {
    system(echo include\\\($$CONFIG_VARS\\\) >> $$QMAKE_CACHE)
}

isEmpty(IRC_INSTALL_LIBS):IRC_INSTALL_LIBS = $$[QT_INSTALL_LIBS]
isEmpty(IRC_INSTALL_BINS):IRC_INSTALL_BINS = $$[QT_INSTALL_BINS]
isEmpty(IRC_INSTALL_HEADERS):IRC_INSTALL_HEADERS = $$[QT_INSTALL_HEADERS]/Communi
isEmpty(IRC_INSTALL_FEATURES):IRC_INSTALL_FEATURES = $$[QT_INSTALL_DATA]/mkspecs/features

# qt4/win: WARNING: Unescaped backslashes are deprecated
!win32|greaterThan(QT_MAJOR_VERSION, 4) {
    system(echo IRC_INSTALL_LIBS=$$IRC_INSTALL_LIBS >> $$QMAKE_CACHE)
    system(echo IRC_INSTALL_LIBS=$$IRC_INSTALL_LIBS >> $$IRC_CONFIG)
    system(echo IRC_INSTALL_BINS=$$IRC_INSTALL_BINS >> $$QMAKE_CACHE)
    system(echo IRC_INSTALL_BINS=$$IRC_INSTALL_BINS >> $$IRC_CONFIG)
    system(echo IRC_INSTALL_HEADERS=$$IRC_INSTALL_HEADERS >> $$QMAKE_CACHE)
    system(echo IRC_INSTALL_HEADERS=$$IRC_INSTALL_HEADERS >> $$IRC_CONFIG)
    system(echo IRC_INSTALL_FEATURES=$$IRC_INSTALL_FEATURES >> $$QMAKE_CACHE)
    system(echo IRC_INSTALL_FEATURES=$$IRC_INSTALL_FEATURES >> $$IRC_CONFIG)
}

OTHER_FILES += astylerc
OTHER_FILES += AUTHORS
OTHER_FILES += CHANGELOG
OTHER_FILES += configure
OTHER_FILES += Doxyfile
OTHER_FILES += INSTALL
OTHER_FILES += LICENSE
OTHER_FILES += README
OTHER_FILES += features/communi.prf

features.files += features/communi.prf
features.files += $$OUT_PWD/communi-config.prf
features.path = $$IRC_INSTALL_FEATURES
INSTALLS += features

include(version.pri)
!build_pass {
    macx {
        !qt_no_framework {
            message(Building Communi $$IRC_VERSION (framework))
        } else {
            message(Building Communi $$IRC_VERSION (dylib))
        }
    } else {
        message(Building Communi $$IRC_VERSION)
    }
}
