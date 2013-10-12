/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

import QtQml 2.0
import Communi 3.0

QtObject {
    id: bot

    property string channel: "#jpnurmi"

    property Irc irc: Irc { id: irc }
    property IrcCommand cmd: IrcCommand { id: cmd }

    property IrcBufferModel model: IrcBufferModel {
        id: model

        connection: IrcConnection {
            id: connection

            host: "irc.freenode.net"
            userName: "communi"
            nickName: "QmlBot" + Math.round(Math.random() * 9999)
            realName: qsTr("Communi %1 QML bot example").arg(irc.version())

            onMessageReceived: {
                if (message.type === IrcMessage.Private) {
                    if (message.private) {
                        // private message: reply to the message sender
                        // => triggers: "!<cmd> <params>" and "<cmd> <params>"
                        parser.target = message.nick
                        parser.triggers = ["!", ""]
                    } else {
                        // channel message: reply to the target channel
                        // => triggers: "!<cmd> <params>" and "bot: <cmd> <params>"
                        console.log(message)
                        console.log(message.target)
                        parser.target = message.target
                        parser.triggers = ["!", nickName + ":"]
                    }

                    var cmd = parser.parse(message.message)
                    if (cmd) {
                        if (cmd.type === IrcCommand.Custom && cmd.parameters[0] === "HELP") {
                            help(cmd.parameters.slice(1))
                        } else {
                            sendCommand(cmd)
                            if (cmd.type === IrcCommand.Quit)
                                Qt.quit()
                        }
                    }
                }
            }

            function help(commands) {
                if (!commands.length)
                    commands = parser.commands

                var target = parser.target
                for (var i = 0; i < commands.length; ++i) {
                    var syntax = parser.syntax(commands[i])
                    sendCommand(cmd.createMessage(target, syntax))
                }
            }

            Component.onCompleted: {
                sendCommand(cmd.createJoin(channel))
                open()
            }

            Component.onDestruction: {
                quit(qsTr("Communi %1 QML bot example").arg(irc.version()))
                close()
            }
        }
    }

    property IrcCommandParser parser: IrcCommandParser {
        id: parser

        channels: model.channels

        Component.onCompleted: {
            parser.addCommand(IrcCommand.CtcpAction, "ACT [target] <message...>");
            parser.addCommand(IrcCommand.Custom, "HELP (<command...>)");
            parser.addCommand(IrcCommand.Nick, "NICK <nick>");
            parser.addCommand(IrcCommand.Join, "JOIN <#channel> (<key>)");
            parser.addCommand(IrcCommand.Part, "PART (<#channel>) (<message...>)");
            parser.addCommand(IrcCommand.Quit, "QUIT (<message...>)");
            parser.addCommand(IrcCommand.Message, "SAY [target] <message...>");
        }
    }
}
