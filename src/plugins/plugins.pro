######################################################################
# Communi
######################################################################

TEMPLATE = subdirs
greaterThan(QT_MAJOR_VERSION, 4) {
    SUBDIRS += quick2
} else {
    !lessThan(QT_MAJOR_VERSION, 4):!lessThan(QT_MINOR_VERSION, 7):SUBDIRS += declarative
}
