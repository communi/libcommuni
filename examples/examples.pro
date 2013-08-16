######################################################################
# Communi
######################################################################

TEMPLATE = subdirs
SUBDIRS += bot client

!lessThan(QT_MAJOR_VERSION, 5):!lessThan(QT_MINOR_VERSION, 1) {
    SUBDIRS += quick
}
