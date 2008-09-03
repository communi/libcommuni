######################################################################
# LibIrcClient-Qt
######################################################################

TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS += 3rdparty src

mkspecs.files = libircclient-qt.prf
mkspecs.path = $$[QT_INSTALL_DATA]/mkspecs/features
INSTALLS += mkspecs
