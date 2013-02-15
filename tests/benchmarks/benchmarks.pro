######################################################################
# Communi
######################################################################

TEMPLATE = subdirs

SUBDIRS += ircmessage
SUBDIRS += irctextformat

private_tests {
    SUBDIRS += ircmessagedata
    SUBDIRS += ircmessagedecoder
}
