/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "tst_clientserver.h"

void tst_ClientServer::init()
{
    server = new QTcpServer(this);
    QVERIFY(server->listen());

    connection = new IrcConnection(this);
    connection->setUserName("user");
    connection->setNickName("nick");
    connection->setRealName("real");
    connection->setHost(server->serverAddress().toString());
    connection->setPort(server->serverPort());

    connection->open();
    waitForOpened();
}

void tst_ClientServer::cleanup()
{
    delete server;
    delete connection;
}

void tst_ClientServer::waitForOpened()
{
    if (!server->waitForNewConnection(200))
        QEXPECT_FAIL("", "The address is not available", Abort);
    serverSocket = server->nextPendingConnection();
    QVERIFY(serverSocket);

    clientSocket = connection->socket();
    QVERIFY(clientSocket);
    QVERIFY(clientSocket->waitForConnected());
}

void tst_ClientServer::waitForWritten(const QByteArray& data)
{
    if (!data.isNull())
        serverSocket->write(data);
    QVERIFY(serverSocket->waitForBytesWritten());
    QVERIFY(clientSocket->waitForReadyRead());
}
