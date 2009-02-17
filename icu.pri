######################################################################
# LibIrcClient-Qt
######################################################################

DEFINES += HAVE_ICU
contains(DEFINES, HAVE_ICU) {
    win32:INCLUDEPATH += C:\ICU\include
    win32:LIBS += -LC:\ICU\lib
    win32:LIBS += -l$$qtLibraryTarget(icuin)
    unix:LIBS += -licui18n -licudata
    LIBS += -l$$qtLibraryTarget(icuuc)
}
