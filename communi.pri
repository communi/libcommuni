######################################################################
# Communi
######################################################################

!verbose:CONFIG += silent

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

CONFIG(debug, debug|release) {
    OBJECTS_DIR = debug
    MOC_DIR = debug
    RCC_DIR = debug
    UI_DIR = debug
} else {
    OBJECTS_DIR = release
    MOC_DIR = release
    RCC_DIR = release
    UI_DIR = release
}

isEmpty(COMMUNI_BUILDDIR):COMMUNI_BUILDDIR = $$OUT_PWD
COMMUNI_LIBDIR = $$COMMUNI_BUILDDIR/lib
COMMUNI_PLUGINSDIR = $$COMMUNI_BUILDDIR/plugins/communi

macx:!qt_no_framework {
    QMAKE_LFLAGS += -F$$COMMUNI_LIBDIR # inject before system frameworks
    LIBS += -framework Communi
    install_name {
        QMAKE_POST_LINK = install_name_tool -change \
            "$$[QT_INSTALL_LIBS]/Communi.framework/Versions/1/Communi" \
            "$$COMMUNI_LIBDIR/Communi.framework/Versions/1/Communi" $$TARGET
    }
} else {
    REAL_TEMPLATE = $$TEMPLATE
    TEMPLATE = fakelib
    QMAKE_LIBDIR += $$COMMUNI_LIBDIR # injects before system libdirs
    LIBS += -l$$qtLibraryTarget(Communi)
    TEMPLATE = $$REAL_TEMPLATE
    !no_rpath:QMAKE_RPATHDIR += $$COMMUNI_LIBDIR
}

static_icu|static_uchardet {
    static_icu:DEFINES += COMMUNI_STATIC_ICU_PLUGIN
    static_uchardet:DEFINES += COMMUNI_STATIC_UCHARDET_PLUGIN

    REAL_TEMPLATE = $$TEMPLATE
    TEMPLATE = fakelib
    contains(REAL_TEMPLATE, .*app) {
        static_icu:LIBS += -L$$COMMUNI_PLUGINSDIR -l$$qtLibraryTarget(icuplugin)
        static_uchardet:LIBS += -L$$COMMUNI_PLUGINSDIR -l$$qtLibraryTarget(uchardetplugin)
    }
    TEMPLATE = $$REAL_TEMPLATE

    static_icu:include(src/plugins/icu/icu.pri)
}
