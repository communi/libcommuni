/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

import QtQuick 2.1
import Communi 3.0

QtObject {
    id: root

    function formatMessage(message) {
        var formatted
        switch (message.type) {
            case IrcMessage.Invite:  formatted = formatInviteMessage(message); break
            case IrcMessage.Join:    formatted = formatJoinMessage(message); break
            case IrcMessage.Kick:    formatted = formatKickMessage(message); break
            case IrcMessage.Mode:    formatted = formatModeMessage(message); break
            case IrcMessage.Names:   formatted = formatNamesMessage(message); break
            case IrcMessage.Nick:    formatted = formatNameMessage(message); break
            case IrcMessage.Notice:  formatted = formatNoticeMessage(message); break
            case IrcMessage.Numeric: formatted = formatNumericMessage(message); break
            case IrcMessage.Part:    formatted = formatPartMessage(message); break
            case IrcMessage.Private: formatted = formatPrivateMessage(message); break
            case IrcMessage.Quit:    formatted = formatQuitMessage(message); break
            case IrcMessage.Topic:   formatted = formatTopicMessage(message); break
        }
        return formatText(formatted, message.timeStamp)
    }

    function formatText(text, timeStamp) {
        if (text)
            return qsTr("[%1] %2").arg(Qt.formatTime(timeStamp, "hh:mm:ss")).arg(text)
    }

    function formatInviteMessage(message) {
        var sender = formatName(message.sender(Irc.SenderName))
        return qsTr("! %1 invited to %3").arg(sender).arg(message.channel)
    }

    function formatJoinMessage(message) {
        var sender = formatName(message.sender(Irc.SenderName))
        return qsTr("! %1 joined %2").arg(sender).arg(message.channel)
    }

    function formatKickMessage(message) {
        var sender = formatName(message.sender(Irc.SenderName))
        var user = formatName(message.user)
        if (message.reason.length)
            return qsTr("! %1 kicked %2 (%3)").arg(sender).arg(user).arg(message.reason)
        return qsTr("! %1 kicked %2").arg(sender).arg(user)
    }

    function formatModeMessage(message) {
        var sender = formatName(message.sender(Irc.SenderName))
        if (message.reply)
            return qsTr("! %1 mode is %2 %3").arg(message.target).arg(message.mode).arg(message.argument)
        return qsTr("! %1 sets mode %2 %3").arg(sender).arg(message.mode).arg(message.argument)
    }

    function formatNamesMessage(message) {
        return qsTr("! %1 has %2 users").arg(message.channel).arg(message.names.length)
    }

    function formatNameMessage(message) {
        var sender = formatName(message.sender(Irc.SenderName))
        var nick = formatName(message.nick)
        return qsTr("! %1 changed nick to %2").arg(sender).arg(nick)
    }

    function formatNoticeMessage(message) {
        var sender = formatName(message.sender(Irc.SenderName))
        var msg = formatHtml(message.message)
        return qsTr("[%1] %2").arg(sender).arg(msg)
    }

    function formatNumericMessage(message) {
        return qsTr("[%1] %2").arg(message.code).arg(message.parameters.slice(1).join(" "))
    }

    function formatPartMessage(message) {
        var sender = formatName(message.sender(Irc.SenderName))
        if (message.reason.length)
            return qsTr("! %1 parted %2 (%3)").arg(sender).arg(message.channel).arg(formatHtml(message.reason))
        return qsTr("! %1 parted %2").arg(sender).arg(message.channel)
    }

    function formatPrivateMessage(message) {
        var sender = formatName(message.sender(Irc.SenderName))
        var msg = formatHtml(message.message)
        if (message.action)
            return qsTr("* %1 %2").arg(sender).arg(msg)
        if (message.request)
            return qsTr("! %1 requested %2").arg(sender).arg(msg.split(" ")[0].toLowerCase())
        return qsTr("&lt;%1&gt; %2").arg(sender).arg(msg)
    }

    function formatQuitMessage(message) {
        var sender = formatName(message.sender(Irc.SenderName))
        if (message.reason.length)
            return qsTr("! %1 has quit (%2)").arg(sender).arg(formatHtml(message.reason))
        return qsTr("! %1 has quit").arg(sender)
    }

    function formatTopicMessage(message) {
        var sender = formatName(message.sender(Irc.SenderName))
        var topic = formatHtml(message.topic)
        var channel = message.channel
        if (message.reply)
            return qsTr("! %1 topic is \"%2\"").arg(channel).arg(topic)
        return qsTr("! %1 sets topic \"%2\" on %3").arg(sender).arg(topic).arg(channel)
    }

    function formatHtml(message) {
        return message
    }

    function formatName(name) {
        return name
    }
}
