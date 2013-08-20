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
import Communi 3.0

Column {
    id: view

    property IrcBuffer buffer
    property IrcChannel channel: buffer ? buffer.toChannel() : null

    signal queried(string name)

    MessageFormatter {
        id: formatter
    }

    spacing: 0

    Connections {
        target: buffer
        onMessageReceived: {
            var line = formatter.formatMessage(message)
            if (line)
                textArea.append(line)
        }
    }

    Rectangle {
        id: rect

        visible: channel
        width: parent.width
        height: Math.max(20, label.implicitHeight + 4)
        color: Qt.darker(palette.base, 1.06)

        Label {
            id: label

            anchors.margins: 2
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            wrapMode: Text.Wrap
            text: channel && channel.topic ? channel.topic : "-"
        }

        Rectangle {
            color: "transparent"
            anchors.fill: parent
            anchors.leftMargin: -1
            anchors.bottomMargin: -1
            border.color: Qt.darker(palette.window, 1.5)
        }
    }

    SplitView {
        width: parent.width
        height: view.height - (channel ? rect.height : 0)

        handleDelegate: Item { } // Rectangle { width: 1; color: Qt.darker(palette.window, 1.5) }

        Item {
            id: textFrame

            Layout.fillWidth: true
            implicitWidth: textArea.implicitWidth
            implicitHeight: textArea.implicitHeight

            TextArea {
                id: textArea

                anchors.fill: parent
                anchors.leftMargin: -1

                readOnly: true
                textFormat: Qt.RichText

                onHeightChanged: alignToBottom()
                onWidthChanged: alignToBottom()

                function alignToBottom() {
                    flickableItem.contentY = flickableItem.contentHeight - flickableItem.height
                }
            }
        }

        Rectangle {
            id: tableFrame

            width: window.width / 6
            color: Qt.darker(palette.base, 1.06)
            implicitWidth: tableView.implicitWidth
            implicitHeight: tableView.implicitHeight

            visible: channel

            TableView {
                id: tableView

                anchors.fill: parent
                anchors.leftMargin: -1

                headerVisible: false
                backgroundVisible: false
                alternatingRowColors: false

                model: IrcUserModel {
                    id: userModel
                    dynamicSort: true
                    channel: view.channel
                }

                TableViewColumn {
                    role: "display"
                }

                onDoubleClicked: {
                    var user = userModel.get(row)
                    view.queried(user.name)
                }
            }
        }
    }
}
