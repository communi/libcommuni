######################################################################
# Communi
######################################################################

TEMPLATE = subdirs
SUBDIRS += src plugins tests # TODO: examples
CONFIG += ordered

static {
    system(echo DEFINES+=COMMUNI_STATIC > .qmake.cache)
    system(echo DEFINES+=COMMUNI_STATIC > features$${QMAKE_DIR_SEP}communi-config.prf)
} else {
    system(echo DEFINES+=COMMUNI_STATIC > .qmake.cache)
    system(echo DEFINES+=COMMUNI_SHARED > features$${QMAKE_DIR_SEP}communi-config.prf)
}

OTHER_FILES += CHANGELOG
OTHER_FILES += COPYING
OTHER_FILES += Doxyfile
OTHER_FILES += README
OTHER_FILES += TODO
OTHER_FILES += VERSION

mkspecs.files += features/communi.prf
mkspecs.files += features/communi-config.prf
mkspecs.path = $$[QMAKE_MKSPECS]/features
INSTALLS += mkspecs

symbian {
    mkspecs.path ~= s/^[A-Za-z]:/ # strip drive letter
    BLD_INF_RULES.prj_exports += "features/communi.prf $$mkspecs.path/communi.prf"
    BLD_INF_RULES.prj_exports += "features/communi-config.prf $$mkspecs.path/communi-config.prf"
}

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
        include(icu.pri)
    } else {
        message(ICU support disabled.)
    }
}
