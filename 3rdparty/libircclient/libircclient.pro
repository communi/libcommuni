######################################################################
# LibIrcClient-Qt
######################################################################

TEMPLATE = lib
DESTDIR = lib
TARGET = ircclient
DEPENDPATH += include src
INCLUDEPATH += include
DEFINES += IN_BUILDING_LIBIRC
CONFIG += staticlib

# Input
HEADERS += include/libirc_dcc.h \
           include/libirc_doc.h \
           include/libirc_doc_faq.h \
           include/libirc_errors.h \
           include/libirc_events.h \
           include/libirc_options.h \
           include/libirc_params.h \
           include/libirc_rfcnumeric.h \
           include/libirc_session.h \
           include/libircclient.h

SOURCES += src/colors.c \
           src/dcc.c \
           src/errors.c \
           src/libircclient.c \
           src/portable.c \
           src/sockets.c \
           src/utils.c

DEF_FILE = src/libircclient.def

headers.files = $$HEADERS
headers.path = $$[QT_INSTALL_HEADERS]/ircclient-qt
INSTALLS += headers
