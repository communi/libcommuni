######################################################################
# LibIrcClient-Qt
######################################################################

TEMPLATE = subdirs
SUBDIRS += auto # other

test.CONFIG += recursive
QMAKE_EXTRA_TARGETS += test
