/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import Communi 3.0

TextField {
    id: textField

    property IrcBuffer buffer

    signal messageSent(IrcMessage message)

    placeholderText: "..."

    IrcCommandParser {
        id: parser

        channels: buffer ? buffer.model.channels : []
        currentTarget: buffer ? buffer.title : ""

        Component.onCompleted: {
            parser.addCommand(IrcCommand.Join, "JOIN <#channel> (<key>)")
            parser.addCommand(IrcCommand.CtcpAction, "ME [target] <message...>")
            parser.addCommand(IrcCommand.Nick, "NICK <nick>")
            parser.addCommand(IrcCommand.Part, "PART (<#channel>) (<message...>)")
        }
    }

    style: TextFieldStyle {
        background: Rectangle {
            color: palette.base
            Rectangle {
                color: "transparent"
                anchors.fill: parent
                anchors.leftMargin: -1
                border.color: "#aaa"
            }
        }
    }

    onAccepted: {
        var cmd = parser.parse(text)
        if (cmd) {
            buffer.connection.sendCommand(cmd)
            if (cmd.type === IrcCommand.Message
                    || cmd.type === IrcCommand.CtcpAction
                    || cmd.type === IrcCommand.Notice) {
                var msg = cmd.toMessage(buffer.connection.nickName, buffer.connection)
                textField.messageSent(msg)
            }
            textField.text = ""
        }
    }
}
