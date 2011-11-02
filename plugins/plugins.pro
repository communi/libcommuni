######################################################################
# Communi
######################################################################

TEMPLATE = subdirs
greaterThan(QT_MAJOR_VERSION, 4)|greaterThan(QT_MINOR_VERSION, 6):SUBDIRS += declarative
