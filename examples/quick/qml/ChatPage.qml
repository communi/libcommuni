/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import Communi 3.0

SplitView {
    id: page

    property alias connection: connection
    property IrcBuffer currentBuffer: bufferModel.get(tableView.currentRow)

    IrcConnection {
        id: connection
    }

    IrcBufferModel {
        id: bufferModel
        dynamicSort: true
        connection: connection
        onMessageIgnored: serverBuffer.receiveMessage(message)
    }

    IrcBuffer {
        id: serverBuffer
        sticky: true
        name: connection.host
        Component.onCompleted: bufferModel.add(serverBuffer)
    }

    IrcCommandParser {
        id: parser

        channels: bufferModel.channels
        currentTarget: currentBuffer ? currentBuffer.title: ""

        Component.onCompleted: {
            parser.addCommand(IrcCommand.Join, "JOIN <#channel> (<key>)")
            parser.addCommand(IrcCommand.CtcpAction, "ME [target] <message...>")
            parser.addCommand(IrcCommand.Nick, "NICK <nick>")
            parser.addCommand(IrcCommand.Part, "PART (<#channel>) (<message...>)")
        }
    }

    handleDelegate: Rectangle { width: 1; color: Qt.darker(palette.window, 1.5) }

    Rectangle {
        id: frame

        color: Qt.darker(palette.base, 1.06)
        implicitWidth: tableView.implicitWidth
        implicitHeight: tableView.implicitHeight

        TableView {
            id: tableView

            anchors.fill: parent
            anchors.rightMargin: -1

            headerVisible: false
            backgroundVisible: false
            alternatingRowColors: false

            Connections {
                target: bufferModel
                onAdded: {
                    tableView.currentRow = bufferModel.count - 1
                }
            }

            model: bufferModel

            TableViewColumn {
                role: "display"
            }
        }
    }

    ColumnLayout {
        spacing: 0

        Item {
            id: stack

            width: 1
            height: 1
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true

            Repeater {
                model: bufferModel

                delegate: BufferView {
                    buffer: model.buffer
                    anchors.fill: parent
                    visible: index === tableView.currentRow

                    onQueried: {
                        var buffer = bufferModel.add(name)
                        tableView.currentRow = bufferModel.indexOf(buffer)
                    }
                }
            }
        }

        TextField {
            id: textField

            Layout.fillWidth: true
            placeholderText: "..."

            style: TextFieldStyle {
                background: Rectangle {
                    color: palette.base
                    Rectangle {
                        color: "transparent"
                        anchors.fill: parent
                        anchors.topMargin: -1
                        anchors.leftMargin: -1
                        border.color: Qt.darker(palette.window, 1.5)
                    }
                }
            }

            focus: true

            Connections {
                target: tableView
                onCurrentRowChanged: textField.forceActiveFocus()
            }

            onAccepted: {
                var cmd = parser.parse(text)
                if (cmd) {
                    connection.sendCommand(cmd)
                    if (cmd.type === IrcCommand.Message
                            || cmd.type === IrcCommand.CtcpAction
                            || cmd.type === IrcCommand.Notice) {
                        var msg = cmd.toMessage(connection.nickName, connection)
                        currentBuffer.receiveMessage(msg)
                    }
                    textField.text = ""
                }
            }
        }
    }
}
