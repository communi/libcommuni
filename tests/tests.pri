######################################################################
# Communi
######################################################################

QT = core network
CONFIG += console no_keywords
CONFIG -= app_bundle

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

LIBS += -L$$PWD/../lib
QMAKE_RPATHDIR += $$PWD/../lib

TEMPLATE += fakelib
macx:!qt_no_framework {
    LIBS += -f$$qtLibraryTarget(Communi)
} else {
    LIBS += -l$$qtLibraryTarget(Communi)
}
TEMPLATE -= fakelib
