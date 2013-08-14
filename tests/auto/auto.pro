######################################################################
# Communi
######################################################################

TEMPLATE = subdirs

SUBDIRS += irccommand
SUBDIRS += irccommandparser
SUBDIRS += ircmessage
SUBDIRS += ircsender
SUBDIRS += ircconnection

private_tests {
    SUBDIRS += ircmessagedecoder
}
