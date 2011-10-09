######################################################################
# Communi
######################################################################

!verbose:!symbian:CONFIG += silent

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

macx:!qt_no_framework {
    LIBS += -F$$PWD/../lib -framework Communi
    QMAKE_POST_LINK = install_name_tool -change \
        "$$[QT_INSTALL_LIBS]/Communi.framework/Versions/1/Communi" \
        "$$PWD/../lib/Communi.framework/Versions/1/Communi" $$TARGET
} else {
    TEMPLATE += fakelib
    LIBS += -L$$PWD/../lib -l$$qtLibraryTarget(Communi)
    TEMPLATE -= fakelib
    QMAKE_RPATHDIR += $$PWD/../lib
}

!no_icu:include(../icu.pri)
