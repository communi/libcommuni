######################################################################
# Communi
######################################################################

TEMPLATE = subdirs

SUBDIRS += irc
SUBDIRS += irccommand
SUBDIRS += irccommandparser
SUBDIRS += ircmessage
SUBDIRS += ircconnection

private_tests {
    SUBDIRS += ircmessagedecoder
}
