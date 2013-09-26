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

Item {
    id: page

    property alias currentBuffer: bufferListView.currentBuffer
    property ListModel connections: ListModel {
        id: connectionModel
    }

    function addConnection(connection) {
        connectionModel.append({connection: connection})
        if (!currentBuffer)
            currentBuffer = connection.serverBuffer
    }

    function removeConnection(connection) {
        for (var i = 0; i < connectionModel.count; ++i) {
            if (connectionModel.get(i).connection === connection) {
                connectionModel.remove(i)
                break
            }
        }
    }

    SplitView {
        anchors.fill: parent

        handleDelegate: Item { }

        BufferListView {
            id: bufferListView
            width: window.width / 6
            model: connectionModel
        }

        Column {
            Layout.fillWidth: true

            TopicLabel {
                id: topicLabel
                width: parent.width
                visible: channel
                channel: currentBuffer ? currentBuffer.toChannel() : null
            }

            SplitView {
                width: parent.width
                height: parent.height - (topicLabel.visible ? topicLabel.height : 0) - textEntry.height

                handleDelegate: Item { }

                Item {
                    id: stack

                    width: 1; height: 1
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Repeater {
                        anchors.fill: parent
                        model: connectionModel

                        delegate: Repeater {
                            anchors.fill: parent
                            model: connection.bufferModel

                            delegate: TextBrowser {
                                buffer: model.buffer
                                anchors.fill: parent
                                visible: buffer == currentBuffer
                            }
                        }
                    }
                }

                UserListView {
                    width: window.width / 6
                    visible: currentBuffer && currentBuffer.channel
                    channel: currentBuffer ? currentBuffer.toChannel() : null
                    onQueried: currentBuffer = currentBuffer.model.add(user)
                }
            }

            TextEntry {
                id: textEntry
                width: parent.width
                buffer: currentBuffer
                enabled: currentBuffer
                onMessageSent: currentBuffer.receiveMessage(message)
            }
        }
    }
}
