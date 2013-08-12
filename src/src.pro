######################################################################
# Communi
######################################################################

TEMPLATE = subdirs
SUBDIRS += core model util

model.depends = core
util.depends = core
