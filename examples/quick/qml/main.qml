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

    width: 800
    height: 480

    minimumWidth: stack.initialItem.implicitWidth
    minimumHeight: stack.initialItem.implicitHeight

    color: Qt.darker(palette.base, 1.06)

    SystemPalette {
        id: palette
    }

    Irc {
        id: irc
    }

    Component {
        id: chatPage
        ChatPage {
            Connections {
                target: window
                onClosing: {
                    connection.quit(qsTr("Communi %1 QtQuick example").arg(irc.version()))
                }
            }
        }
    }

    StackView {
        id: stack
        anchors.fill: parent

        initialItem: ConnectPage {
            cancelButton.enabled: false
            onAccepted: {
                var page = chatPage.createObject(window, {"connection.host": host,
                                                          "connection.port": port,
                                                          "connection.secure": secure,
                                                          "connection.nickName": nickName,
                                                          "connection.realName": realName,
                                                          "connection.userName": userName,
                                                          "connection.password": password})
                page.connection.open()
                stack.replace(page)
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
        close.accepted = false
        window.visible = false
        quitTimer.start()
    }
}
