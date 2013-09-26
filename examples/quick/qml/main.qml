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
import Communi 3.0

ApplicationWindow {
    id: window

    visible: true
    title: qsTr("Communi %1 QtQuick example").arg(irc.version())

    width: 800
    height: 480

    minimumWidth: connectPage.implicitWidth
    minimumHeight: connectPage.implicitHeight + toolBar.height

    color: Qt.darker(palette.base, 1.06)

    SystemPalette {
        id: palette
    }

    Irc {
        id: irc
    }

    toolBar: ToolBar {
        ToolButton {
            text: "+"
            enabled: !connectPage.visible
            onClicked: connectPage.visible = true
        }
    }

    Component {
        id: connection
        Connection { }
    }

    ConnectPage {
        id: connectPage

        anchors.fill: parent
        cancelButton.enabled: chatPage.connections.count

        onAccepted: {
            var conn = connection.createObject(window, {"host": host, "port": port, "secure": secure,
                                                        "nickName": nickName, "realName": realName,
                                                        "userName": userName, "password": password})
            conn.open()
            chatPage.addConnection(conn)
            connectPage.visible = false
        }
        onRejected: connectPage.visible = false
    }

    ChatPage {
        id: chatPage
        anchors.fill: parent
        visible: !connectPage.visible

        Connections {
            target: chatPage.connections
            onCountChanged: {
                console.log(chatPage.connections.count)
                if (chatPage.connections.count === 0)
                    connectPage.visible = true
            }
        }
    }

    Timer {
        id: quitTimer
        interval: 1000
        onTriggered: Qt.quit()
    }

    onClosing: {
        // let connections close gracefully
        // TODO: connection.quit(window.title)
        close.accepted = false
        window.visible = false
        quitTimer.start()
    }
}
