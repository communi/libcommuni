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

ColumnLayout {
    id: view

    property IrcBuffer buffer

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
        Layout.fillWidth: true
        visible: buffer.channel
        color: palette.alternateBase
        implicitHeight: label.implicitHeight + 4

        Label {
            id: label

            anchors.margins: 2
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            wrapMode: Text.Wrap
            text: buffer && buffer.channel ? buffer.topic : ""
        }

        Rectangle {
            height: 1
            width: parent.width
            anchors.bottom: parent.bottom
            color: Qt.darker(palette.window, 1.5)
        }
    }

    SplitView {
        Layout.fillWidth: true
        Layout.fillHeight: true

        TextArea {
            id: textArea

            readOnly: true
            frameVisible: false
            Layout.fillWidth: true
            textFormat: Qt.RichText
        }

        TableView {
            id: tableView

            frameVisible: false
            headerVisible: false
            visible: buffer.channel
            alternatingRowColors: true

            model: IrcUserModel {
                id: userModel
                dynamicSort: true
                channel: buffer.toChannel()
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
