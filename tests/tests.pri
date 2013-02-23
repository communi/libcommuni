######################################################################
# Communi
######################################################################

TARGET = tst_$$basename(_PRO_FILE_PWD_)
CONFIG += install_name no_testcase_installs
COMMUNI_BUILDDIR = $$OUT_PWD/../../..
include(../communi.pri)
