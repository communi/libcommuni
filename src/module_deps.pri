######################################################################
# Communi
######################################################################

isEmpty(COMMUNI_MODULES):error(COMMUNI_MODULES must be set)

isEmpty(COMMUNI_BUILDDIR):COMMUNI_BUILDDIR = $$OUT_PWD/../..
COMMUNI_LIBDIR = $$COMMUNI_BUILDDIR/lib

isEmpty(COMMUNI_SOURCEDIR):COMMUNI_SOURCEDIR = $$PWD/..
COMMUNI_INCDIR = $$COMMUNI_SOURCEDIR/include

INCLUDEPATH += $$COMMUNI_INCDIR

for(COMMUNI_MODULE, COMMUNI_MODULES) {
    !contains(DEFINES, COMMUNI_STATIC):macx:!qt_no_framework {
        INCLUDEPATH += $$COMMUNI_LIBDIR/$${COMMUNI_MODULE}.framework/Headers
        QMAKE_LFLAGS += -F$$COMMUNI_LIBDIR # inject before system frameworks
        LIBS += -framework $$COMMUNI_MODULE
        install_name {
            !isEmpty(QMAKE_POST_LINK):QMAKE_POST_LINK += &&
            QMAKE_POST_LINK += install_name_tool -change \
                "$$[QT_INSTALL_LIBS]/$${COMMUNI_MODULE}.framework/Versions/3/$${COMMUNI_MODULE}" \
                "$$COMMUNI_LIBDIR/$${COMMUNI_MODULE}.framework/Versions/3/$${COMMUNI_MODULE}" $$TARGET
        }
    } else {
        INCLUDEPATH += $$COMMUNI_INCDIR/$$COMMUNI_MODULE
        DEPENDPATH += $$COMMUNI_INCDIR/$$COMMUNI_MODULE
        QMAKE_LIBDIR += $$COMMUNI_LIBDIR # injects before system libdirs
        REAL_TEMPLATE = $$TEMPLATE
        TEMPLATE = fakelib
        LIBS += -l$$qtLibraryTarget($$COMMUNI_MODULE)
        TEMPLATE = $$REAL_TEMPLATE
        !no_rpath:QMAKE_RPATHDIR += $$COMMUNI_LIBDIR
    }
}
