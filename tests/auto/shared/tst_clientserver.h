/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This test is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#ifndef TST_CLIENTSERVER_H
#define TST_CLIENTSERVER_H

#include <IrcConnection>

#include <QtTest/QtTest>
#include <QtCore/QPointer>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#if QT_VERSION >= 0x050000
#define Q4SKIP(description) QSKIP(description)
#else
#define Q4SKIP(description) QSKIP(description, SkipAll)
#endif

class tst_ClientServer : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

protected:
    void waitForWritten(const QByteArray& data = QByteArray());

    QPointer<QTcpServer> server;
    QPointer<QTcpSocket> serverSocket;
    QPointer<IrcConnection> connection;
    QPointer<QAbstractSocket> clientSocket;
};

#endif // TST_CLIENTSERVER_H
