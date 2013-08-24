######################################################################
# Communi
######################################################################

TEMPLATE = subdirs
SUBDIRS += core model util imports

model.depends = core
util.depends = core
imports.depends = core model util
