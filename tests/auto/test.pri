######################################################################
# LibIrcClient-Qt
######################################################################

CONFIG += qtestlib

INCLUDEPATH     += ../../../include/
DEPENDPATH      += ../../../include/
unix:!macx:LIBS += -Wl,-rpath,../../../lib
macx:LIBS       += -F../../../lib
LIBS            += -L../../../lib

qtAddLibrary(ircclient-qt)

test.depends = first
!isEmpty(DESTDIR):test.commands += cd $(DESTDIR) &&
unix {
    macx:test.commands += ./$(TARGET).app/Contents/MacOS/$(TARGET)
    else:test.commands += ./$(TARGET)
} else:win32 {
    DESTDIR = ./
    test.CONFIG += recursive
    build_pass:test.commands += $(TARGET)
}
QMAKE_EXTRA_TARGETS += test
