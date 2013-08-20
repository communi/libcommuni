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

Rectangle {
    id: frame

    property IrcChannel channel

    signal queried(string user)

    color: Qt.darker(palette.base, 1.06)
    implicitWidth: view.implicitWidth
    implicitHeight: view.implicitHeight

    TableView {
        id: view

        anchors.fill: parent
        anchors.leftMargin: -1

        headerVisible: false
        backgroundVisible: false
        alternatingRowColors: false

        model: IrcUserModel {
            id: userModel
            dynamicSort: true
            channel: frame.channel
        }

        TableViewColumn {
            role: "display"
        }

        onDoubleClicked: {
            var user = userModel.get(row)
            frame.queried(user.name)
        }
    }
}
