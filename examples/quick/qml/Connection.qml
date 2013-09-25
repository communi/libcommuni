/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

import QtQuick 2.1
import Communi 3.0

IrcConnection {
    id: root

    property IrcBufferModel bufferModel: IrcBufferModel {
        id: bufferModel
        dynamicSort: true
        connection: root
        onMessageIgnored: {
            if (serverBuffer)
                serverBuffer.receiveMessage(message)
        }
    }

    property IrcBuffer serverBuffer: IrcBuffer {
        id: serverBuffer
        sticky: true
        name: root.displayName
        Component.onCompleted: bufferModel.add(serverBuffer)
    }
}
