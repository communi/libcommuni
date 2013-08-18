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

Item {
    id: page

    property string host: hostField.text || hostField.placeholderText
    property int port: portField.value
    property bool secure: secureField.checked
    property string nickName: nickNameField.text || nickNameField.placeholderText
    property string realName: realNameField.text || realNameField.placeholderText
    property string userName: userNameField.text || userNameField.placeholderText
    property string password: passwordField.text

    signal accepted()
    signal rejected()

    property alias okButton: okButton
    property alias cancelButton: cancelButton

    implicitWidth: grid.implicitWidth + row.implicitWidth
    implicitHeight: grid.implicitHeight + row.implicitHeight + 60

    GridLayout {
        id: grid

        width: page.width * 2 / 3
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -row.height

        columns: 2
        rowSpacing: 12
        columnSpacing: 12

        Label { text: "Host:" }
        TextField {
            id: hostField
            Layout.fillWidth: true
            placeholderText: "irc.freenode.net"
        }

        Label { text: "Port:" }
        RowLayout {
            SpinBox {
                id: portField
                value: 6667
                minimumValue: 1024
                maximumValue: 65535
                Layout.fillWidth: true
            }
            CheckBox {
                id: secureField
                text: "Secure"
            }
        }

        Label { text: "Nick name:" }
        TextField {
            id: nickNameField
            Layout.fillWidth: true
            placeholderText: "Communi" + Math.round(Math.random() * 9999)
        }

        Label { text: "Real name:" }
        TextField {
            id: realNameField
            Layout.fillWidth: true
            placeholderText: qsTr("Communi %1 QtQuick example").arg(irc.version())
        }

        Label { text: "User name:" }
        TextField {
            id: userNameField
            Layout.fillWidth: true
            placeholderText: "communi"
        }

        Label { text: "Password:" }
        TextField {
            id: passwordField
            echoMode: TextInput.Password
            Layout.fillWidth: true
        }
    }

    RowLayout {
        id: row

        anchors.margins: 12
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Item { Layout.fillWidth: true }

        Button {
            id: okButton
            text: "Ok"
            onClicked: page.accepted()
        }

        Button {
            id: cancelButton
            text: "Cancel"
            onClicked: page.rejected()
        }
    }
}
