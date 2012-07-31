######################################################################
# Communi
######################################################################

!verbose:!symbian:CONFIG += silent

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

!symbian {
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
}

macx:!qt_no_framework {
    LIBS += -F$$PWD/lib -framework Communi
    install_name {
        QMAKE_POST_LINK = install_name_tool -change \
            "$$[QT_INSTALL_LIBS]/Communi.framework/Versions/1/Communi" \
            "$$PWD/lib/Communi.framework/Versions/1/Communi" $$TARGET
    }
} else {
    REAL_TEMPLATE = $$TEMPLATE
    TEMPLATE = fakelib
    LIBS += -L$$PWD/lib -l$$qtLibraryTarget(Communi)
    TEMPLATE = $$REAL_TEMPLATE
    QMAKE_RPATHDIR += $$PWD/lib
}
