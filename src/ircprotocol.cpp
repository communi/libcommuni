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
#include "ircmessagebuilder_p.h"
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

    void handleNumericMessage(IrcNumericMessage* msg);
    void handlePrivateMessage(IrcPrivateMessage* msg);
    void handleCapabilityMessage(IrcCapabilityMessage* msg);

    IrcProtocol* q_ptr;
    IrcSession* session;
    IrcMessageBuilder* builder;
    QByteArray buffer;

    QSet<QString> activeCaps;
    QSet<QString> availableCaps;
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

    if (line.startsWith("AUTHENTICATE")) {
        const QList<QByteArray> args = line.split(' ');
        const bool auth = args.count() == 2 && args.at(1) == "+";
        q->authenticate(auth && session->isSecure());
        if (!session->isConnected())
            session->sendData("CAP END");
        return;
    }

    IrcMessage* msg = IrcMessage::fromData(line, session);
    if (msg) {
        msg->setEncoding(session->encoding());

        switch (msg->type()) {
        case IrcMessage::Capability:
            handleCapabilityMessage(static_cast<IrcCapabilityMessage*>(msg));
            break;
        case IrcMessage::Nick:
            if (msg->flags() & IrcMessage::Own)
                q->setNick(static_cast<IrcNickMessage*>(msg)->nick());
            break;
        case IrcMessage::Numeric:
            handleNumericMessage(static_cast<IrcNumericMessage*>(msg));
            break;
        case IrcMessage::Ping:
            session->sendRaw("PONG " + static_cast<IrcPingMessage*>(msg)->argument());
            break;
        case IrcMessage::Private:
            handlePrivateMessage(static_cast<IrcPrivateMessage*>(msg));
            break;
        default:
            break;
        }

        q->receiveMessage(msg);
        if (msg->type() == IrcMessage::Numeric)
            builder->processMessage(static_cast<IrcNumericMessage*>(msg));
    }
}

void IrcProtocolPrivate::handleNumericMessage(IrcNumericMessage* msg)
{
    Q_Q(IrcProtocol);
    switch (msg->code()) {
    case Irc::RPL_WELCOME:
        q->setNick(msg->parameters().value(0));
        q->setConnected(true);
        break;
    case Irc::RPL_ISUPPORT: {
        QHash<QString, QString> info;
        foreach (const QString& param, msg->parameters().mid(1)) {
            QStringList keyValue = param.split("=", QString::SkipEmptyParts);
            info.insert(keyValue.value(0), keyValue.value(1));
        }
        q->setInfo(info);
        break;
    }
    case Irc::ERR_NICKNAMEINUSE:
    case Irc::ERR_NICKCOLLISION: {
        QString alternate;
        emit session->nickNameReserved(&alternate);
        if (!alternate.isEmpty())
            session->setNickName(alternate);
        break;
    }
    default:
        break;
    }
}

void IrcProtocolPrivate::handlePrivateMessage(IrcPrivateMessage* msg)
{
    if (msg->isRequest()) {
        IrcCommand* reply = session->createCtcpReply(msg);
        if (reply)
            session->sendCommand(reply);
    }
}

static void handleCapability(QSet<QString>* caps, const QString& cap)
{
    Q_ASSERT(caps);
    if (cap.startsWith(QLatin1Char('-')) || cap.startsWith(QLatin1Char('=')))
        caps->remove(cap.mid(1));
    else if (cap.startsWith(QLatin1Char('~')))
        caps->insert(cap.mid(1));
    else
        caps->insert(cap);
}

void IrcProtocolPrivate::handleCapabilityMessage(IrcCapabilityMessage* msg)
{
    Q_Q(IrcProtocol);
    const bool connected = session->isConnected();
    const QString subCommand = msg->subCommand();
    if (subCommand == "LS") {
        foreach (const QString& cap, msg->capabilities())
            handleCapability(&availableCaps, cap);

        if (!connected) {
            const QStringList params = msg->parameters();
            if (params.value(params.count() - 1) != QLatin1String("*")) {
                QStringList request;
                emit session->capabilities(availableCaps.toList(), &request);
                if (!request.isEmpty()) {
                    session->sendCommand(IrcCommand::createCapability("REQ", request));
                } else {
                    // TODO: #14: SASL over non-SSL connection
                    if (session->isSecure())
                        q->authenticate(false);
                    session->sendData("CAP END");
                }
            }
        }
    } else if (subCommand == "ACK" || subCommand == "NAK") {
        bool auth = false;
        if (subCommand == "ACK") {
            foreach (const QString& cap, msg->capabilities()) {
                handleCapability(&activeCaps, cap);
                if (cap == "sasl")
                    auth = session->sendData("AUTHENTICATE PLAIN"); // TODO: methods
            }
        }

        if (!connected && !auth) {
            // TODO: #14: SASL over non-SSL connection
            if (session->isSecure())
                q->authenticate(false);
            session->sendData("CAP END");
        }
    }
}

IrcProtocol::IrcProtocol(IrcSession* session) : QObject(session), d_ptr(new IrcProtocolPrivate(this))
{
    Q_D(IrcProtocol);
    d->session = session;
    d->builder = new IrcMessageBuilder(session);
    connect(d->builder, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(receiveMessage(IrcMessage*)));
}

IrcProtocol::~IrcProtocol()
{
    Q_D(IrcProtocol);
    delete d->builder;
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

void IrcProtocol::open()
{
    Q_D(IrcProtocol);

    const bool secure = d->session->isSecure();
    if (secure)
        QMetaObject::invokeMethod(socket(), "startClientEncryption");

    d->activeCaps.clear();
    d->availableCaps.clear();

    // Send CAP LS first; if the server understands it this will
    // temporarily pause the handshake until CAP END is sent, so we
    // know whether the server supports the CAP extension.
    d->session->sendData("CAP LS");

    if (!d->session->isSecure())
        authenticate(false);

    d->session->sendCommand(IrcCommand::createNick(d->session->nickName()));
    d->session->sendRaw(QString("USER %1 hostname servername :%2").arg(d->session->userName(), d->session->realName()));
}

void IrcProtocol::authenticate(bool secure)
{
    Q_D(IrcProtocol);
    QString passwd;
    emit d->session->password(&passwd);
    if (!passwd.isEmpty()) {
        if (secure) {
            const QByteArray userName = d->session->userName().toUtf8();
            const QByteArray data = userName + '\0' + userName + '\0' + passwd.toUtf8();
            d->session->sendData("AUTHENTICATE " + data.toBase64());
        } else {
            d->session->sendRaw(QString("PASS %1").arg(passwd));
        }
    }
}

void IrcProtocol::read()
{
    Q_D(IrcProtocol);
    d->buffer += socket()->readAll();
    // try reading RFC compliant message lines first
    d->readLines("\r\n");
    // fall back to RFC incompliant lines...
    d->readLines("\n");
}

bool IrcProtocol::write(const QByteArray& data)
{
    return socket()->write(data + QByteArray("\r\n")) != -1;
}

QStringList IrcProtocol::availableCapabilities() const
{
    Q_D(const IrcProtocol);
    return d->availableCaps.toList();
}

QStringList IrcProtocol::activeCapabilities() const
{
    Q_D(const IrcProtocol);
    return d->activeCaps.toList();
}

void IrcProtocol::setActive(bool active)
{
    Q_D(IrcProtocol);
    IrcSessionPrivate* priv = IrcSessionPrivate::get(d->session);
    priv->setActive(active);
}

void IrcProtocol::setConnected(bool connected)
{
    Q_D(IrcProtocol);
    IrcSessionPrivate* priv = IrcSessionPrivate::get(d->session);
    priv->setConnected(connected);
}

void IrcProtocol::setNick(const QString& nick)
{
    Q_D(IrcProtocol);
    IrcSessionPrivate* priv = IrcSessionPrivate::get(d->session);
    priv->setNick(nick);
}

void IrcProtocol::setInfo(const QHash<QString, QString>& info)
{
    Q_D(IrcProtocol);
    IrcSessionPrivate* priv = IrcSessionPrivate::get(d->session);
    priv->setInfo(info);
}

void IrcProtocol::receiveMessage(IrcMessage* message)
{
    Q_D(IrcProtocol);
    IrcSessionPrivate* priv = IrcSessionPrivate::get(d->session);
    priv->receiveMessage(message);
}
