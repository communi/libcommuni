######################################################################
# LibIrcClient-Qt
######################################################################

TEMPLATE = subdirs
SUBDIRS += src # TODO: plugins tests examples
CONFIG += ordered

static {
    system(echo DEFINES+=IRC_STATIC > features$${QMAKE_DIR_SEP}libircclient-qt-config.prf)
} else {
    system(echo DEFINES+=IRC_SHARED > features$${QMAKE_DIR_SEP}libircclient-qt-config.prf)
}

mkspecs.files += features/libircclient-qt.prf
mkspecs.files += features/libircclient-qt-config.prf
mkspecs.path = $$[QMAKE_MKSPECS]/features
INSTALLS += mkspecs

symbian {
    mkspecs.path ~= s/^[A-Za-z]:/ # strip drive letter
    BLD_INF_RULES.prj_exports += "features/libircclient-qt.prf $$mkspecs.path/libircclient-qt.prf"
    BLD_INF_RULES.prj_exports += "features/libircclient-qt-config.prf $$mkspecs.path/libircclient-qt-config.prf"
}

!build_pass {
    macx {
        # See above for an explanation of this.
        !qt_no_framework {
            message(Building LibIrcClient-Qt $$IRC_VERSION (framework))
        } else {
            message(Building LibIrcClient-Qt $$IRC_VERSION (dylib))
        }
    } else {
        message(Building LibIrcClient-Qt $$IRC_VERSION)
    }

    !no_icu {
        message(ICU support enabled. Run \'qmake -config no_icu\' to disable ICU support.)
        include(icu.pri)
    } else {
        message(ICU support disabled.)
    }
}
