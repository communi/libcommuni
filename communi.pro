######################################################################
# Communi
######################################################################

TEMPLATE = subdirs
SUBDIRS += src
CONFIG += ordered

!no_plugins {
    SUBDIRS += src/plugins
} else {
    message(Plugins disabled)
}

!no_tests {
    contains(MEEGO_EDITION,harmattan) {
        message(Tests not supported on Meego Harmattan)
    } else:symbian {
        message(Tests not supported on Symbian)
    } else {
        SUBDIRS += tests
    }
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

static {
    system(echo DEFINES+=COMMUNI_STATIC > $${OUT_PWD}$${QMAKE_DIR_SEP}.qmake.cache)
    system(echo DEFINES+=COMMUNI_STATIC > $${OUT_PWD}$${QMAKE_DIR_SEP}communi-config.prf)
} else {
    system(echo DEFINES+=COMMUNI_SHARED > $${OUT_PWD}$${QMAKE_DIR_SEP}.qmake.cache)
    system(echo DEFINES+=COMMUNI_SHARED > $${OUT_PWD}$${QMAKE_DIR_SEP}communi-config.prf)
}

OTHER_FILES += AUTHORS
OTHER_FILES += CHANGELOG
OTHER_FILES += configure
OTHER_FILES += COPYING
OTHER_FILES += Doxyfile
OTHER_FILES += README
OTHER_FILES += TODO
OTHER_FILES += VERSION
OTHER_FILES += features/communi.prf

contains(MEEGO_EDITION,harmattan) {
    OTHER_FILES += qtc_packaging/debian_harmattan/rules
    OTHER_FILES += qtc_packaging/debian_harmattan/README
    OTHER_FILES += qtc_packaging/debian_harmattan/manifest.aegis
    OTHER_FILES += qtc_packaging/debian_harmattan/copyright
    OTHER_FILES += qtc_packaging/debian_harmattan/control
    OTHER_FILES += qtc_packaging/debian_harmattan/compat
    OTHER_FILES += qtc_packaging/debian_harmattan/changelog
}

!contains(MEEGO_EDITION,harmattan) {
    mkspecs.files += features/communi.prf
    mkspecs.files += $$OUT_PWD/communi-config.prf
    mkspecs.path = $$[QMAKE_MKSPECS]/features
    INSTALLS += mkspecs
}

symbian {
    mkspecs.path ~= s/^[A-Za-z]:/ # strip drive letter
    BLD_INF_RULES.prj_exports += "features/communi.prf $$mkspecs.path/communi.prf"
    BLD_INF_RULES.prj_exports += "communi-config.prf $$mkspecs.path/communi-config.prf"
}

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
