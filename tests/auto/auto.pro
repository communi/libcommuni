######################################################################
# Communi
######################################################################

TEMPLATE = subdirs

SUBDIRS += irccommand
SUBDIRS += irccommandparser
SUBDIRS += ircmessage
SUBDIRS += ircsender
SUBDIRS += ircsession

private_tests {
    SUBDIRS += ircmessagedata
    SUBDIRS += ircmessagedecoder
}
