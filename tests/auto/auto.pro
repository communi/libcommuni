######################################################################
# Communi
######################################################################

TEMPLATE = subdirs

# IrcCore
SUBDIRS += irc
SUBDIRS += ircconnection
SUBDIRS += irccommand
SUBDIRS += ircmessage
SUBDIRS += ircnetwork

# IrcModel
SUBDIRS += ircbuffer
SUBDIRS += ircbuffermodel
SUBDIRS += ircchannel
SUBDIRS += ircusermodel

# IrcUtil
SUBDIRS += irccommandparser

private_tests {
    SUBDIRS += ircmessagedecoder
}
