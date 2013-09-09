######################################################################
# Communi
######################################################################

TEMPLATE = subdirs

SUBDIRS += irc
SUBDIRS += ircconnection
SUBDIRS += irccommand
SUBDIRS += irccommandparser
SUBDIRS += ircmessage
SUBDIRS += ircnetwork

private_tests {
    SUBDIRS += ircmessagedecoder
}
