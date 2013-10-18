######################################################################
# Communi
######################################################################

TEMPLATE = subdirs
SUBDIRS += bot client minimal

!lessThan(QT_MAJOR_VERSION, 5):!lessThan(QT_MINOR_VERSION, 1) {
    qtHaveModule(qml):SUBDIRS += qmlbot
    qtHaveModule(quick):SUBDIRS += quick
}
