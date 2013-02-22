/*
* Copyright (C) 2008-2013 The Communi Project
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
* License for more details.
*/

#include "ircprotocol.h"
#include "ircsession_p.h"
#include "ircsessioninfo.h"
#include "ircsession.h"
#include "ircmessage.h"
#include "irccommand.h"
#include "irc.h"
#include <QDebug>

class IrcProtocolPrivate
{
    Q_DECLARE_PUBLIC(IrcProtocol)

public:
    IrcProtocolPrivate(IrcProtocol* protocol);

    void readLines(const QByteArray& delimiter);
    void processLine(const QByteArray& line);

    IrcProtocol* q_ptr;
    IrcSession* session;
    QByteArray buffer;
};

IrcProtocolPrivate::IrcProtocolPrivate(IrcProtocol* protocol) :
    q_ptr(protocol)
{
}

void IrcProtocolPrivate::readLines(const QByteArray& delimiter)
{
    int i = -1;
    while ((i = buffer.indexOf(delimiter)) != -1) {
        QByteArray line = buffer.left(i).trimmed();
        buffer = buffer.mid(i + delimiter.length());
        if (!line.isEmpty())
            processLine(line);
    }
}

void IrcProtocolPrivate::processLine(const QByteArray& line)
{
    Q_Q(IrcProtocol);
    static bool dbg = qgetenv("COMMUNI_DEBUG").toInt();
    if (dbg) qDebug() << line;

    IrcMessage* msg = IrcMessage::fromData(line, session);
    if (msg) {
        msg->setEncoding(session->encoding());
        q->receiveMessage(msg);
    }
}

IrcProtocol::IrcProtocol(IrcSession* session) : QObject(session), d_ptr(new IrcProtocolPrivate(this))
{
    Q_D(IrcProtocol);
    d->session = session;
}

IrcProtocol::~IrcProtocol()
{
}

IrcSession* IrcProtocol::session() const
{
    Q_D(const IrcProtocol);
    return d->session;
}

QAbstractSocket* IrcProtocol::socket() const
{
    Q_D(const IrcProtocol);
    return d->session->socket();
}

void IrcProtocol::login(const QString& password)
{
    Q_D(IrcProtocol);

    // Send CAP LS first; if the server understands it this will
    // temporarily pause the handshake until CAP END is sent, so we
    // know whether the server supports the CAP extension.
    IrcSessionPrivate::get(d->session)->capabilities.clear();
    d->session->sendData("CAP LS");

    if (!password.isEmpty())
        d->session->sendRaw(QString("PASS %1").arg(password));

    d->session->sendCommand(IrcCommand::createNick(d->session->nickName()));
    d->session->sendRaw(QString("USER %1 hostname servername :%2").arg(d->session->userName(), d->session->realName()));
}

void IrcProtocol::receive()
{
    Q_D(IrcProtocol);
    d->buffer += socket()->readAll();
    // try reading RFC compliant message lines first
    d->readLines("\r\n");
    // fall back to RFC incompliant lines...
    d->readLines("\n");
}

bool IrcProtocol::send(const QByteArray& data)
{
    return socket()->write(data + QByteArray("\r\n")) != -1;
}

void IrcProtocol::receiveMessage(IrcMessage* message)
{
    Q_D(IrcProtocol);
    IrcSessionPrivate* priv = IrcSessionPrivate::get(d->session);
    priv->receiveMessage(message);
}
