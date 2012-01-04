######################################################################
# Communi
######################################################################

TEMPLATE = subdirs
SUBDIRS += src plugins examples
!symbian:!contains(MEEGO_EDITION,harmattan):SUBDIRS += tests
CONFIG += ordered

lessThan(QT_MAJOR_VERSION, 4) | lessThan(QT_MINOR_VERSION, 6) {
    error(Communi requires Qt 4.6 or newer but Qt $$[QT_VERSION] was detected.)
}

!isEqual(PWD, $$OUT_PWD) {
    error(Communi does not support shadow builds.)
}

static {
    system(echo DEFINES+=COMMUNI_STATIC > .qmake.cache)
    system(echo DEFINES+=COMMUNI_STATIC > features$${QMAKE_DIR_SEP}communi-config.prf)
} else {
    system(echo DEFINES+=COMMUNI_SHARED > .qmake.cache)
    system(echo DEFINES+=COMMUNI_SHARED > features$${QMAKE_DIR_SEP}communi-config.prf)
}

OTHER_FILES += CHANGELOG
OTHER_FILES += COPYING
OTHER_FILES += Doxyfile
OTHER_FILES += README
OTHER_FILES += TODO
OTHER_FILES += VERSION

contains(MEEGO_EDITION,harmattan) {
    OTHER_FILES += qtc_packaging/debian_harmattan/rules
    OTHER_FILES += qtc_packaging/debian_harmattan/README
    OTHER_FILES += qtc_packaging/debian_harmattan/manifest.aegis
    OTHER_FILES += qtc_packaging/debian_harmattan/copyright
    OTHER_FILES += qtc_packaging/debian_harmattan/control
    OTHER_FILES += qtc_packaging/debian_harmattan/compat
    OTHER_FILES += qtc_packaging/debian_harmattan/changelog
}

contains(MEEGO_EDITION,harmattan) {
    COMMUNI_INSTALL_FEATURES = /usr/share/qt4/mkspecs
} else {
    COMMMUNI_INSTALL_FEATURES = $$[QMAKE_MKSPECS]
}

mkspecs.files += features/communi.prf
mkspecs.files += features/communi-config.prf
mkspecs.path = $$COMMMUNI_INSTALL_FEATURES/features
INSTALLS += mkspecs

symbian {
    mkspecs.path ~= s/^[A-Za-z]:/ # strip drive letter
    BLD_INF_RULES.prj_exports += "features/communi.prf $$mkspecs.path/communi.prf"
    BLD_INF_RULES.prj_exports += "features/communi-config.prf $$mkspecs.path/communi-config.prf"
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

    !no_icu {
        message(ICU support enabled. Run \'qmake -config no_icu\' to disable ICU support.)
    } else {
        message(ICU support disabled.)
        system(echo CONFIG+=no_icu >> .qmake.cache)
    }
}

lessThan(QT_MAJOR_VERSION, 4) | lessThan(QT_MINOR_VERSION, 7) {
    message(Declarative support disabled. Use Qt 4.7 or later to enable declarative support.)
}
