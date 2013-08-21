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

Rectangle {
    id: background

    property IrcBuffer currentBuffer
    property alias model: listView.model

    color: "#edf3fe"

    Rectangle {
        id: frame
        anchors.fill: parent
        color: "transparent"
        border.color: "#bfbfbf"
    }

    ScrollView {
        id: scrollView

        anchors.fill: parent
        anchors.topMargin: -1

        ListView {
            id: listView

            delegate: Column {
                width: parent.width
                property Connection connection: modelData
                Repeater {
                    model: connection.bufferModel
                    delegate: Rectangle {
                        property bool first: index === 0
                        property bool current: model.buffer === currentBuffer
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 1
                        height: Math.max(21, label.implicitHeight + 5)
                        color: first ? "#ddd" : current ? "#b5d5ff" : "transparent"
                        Rectangle {
                            visible: first
                            width: parent.width
                            height: 1
                            anchors.bottom: parent.bottom
                            anchors.fill: parent
                            anchors.leftMargin: -1
                            anchors.rightMargin: -1
                            color: "transparent"
                            border.color: "#bfbfbf"
                        }
                        Label {
                            id: label
                            text: model.title
                            font.bold: current
                            anchors.margins: 2
                            anchors.leftMargin: 6
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                currentBuffer = model.buffer
                            }
                        }
                    }
                }
            }
        }
    }
}
