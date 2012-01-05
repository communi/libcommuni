######################################################################
# Communi
######################################################################

!symbian {
    DEFINES += HAVE_ICU
    contains(MEEGO_EDITION,harmattan) {
        CONFIG += link_pkgconfig
        PKGCONFIG += icu
    } else {
        win32:INCLUDEPATH += C:/ICU/include
        win32:LIBS += -LC:/ICU/lib
        macx:INCLUDEPATH += /opt/local/include
        macx:LIBS += -L/opt/local/lib
        LIBS += -licui18n -licudata -licuuc
    }
}
