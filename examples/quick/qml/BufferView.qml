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

SplitView {
    id: view

    property IrcBuffer buffer

    signal queried(string name)

    MessageFormatter {
        id: formatter
    }

    Connections {
        target: buffer
        onMessageReceived: {
            var line = formatter.formatMessage(message)
            if (line)
                textArea.append(line)
        }
    }

    TextArea {
        id: textArea

        readOnly: true
        frameVisible: false
        Layout.fillWidth: true
        Layout.fillHeight: true
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
