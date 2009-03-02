######################################################################
# LibIrcClient-Qt
######################################################################

TEMPLATE = subdirs
SUBDIRS += ircsession

test.CONFIG += recursive
QMAKE_EXTRA_TARGETS += test
