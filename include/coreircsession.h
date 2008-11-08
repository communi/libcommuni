/*
* Copyright (C) 2008 J-P Nurmi jpnurmi@gmail.com
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
*
* $Id$
*/

#ifndef COREIRCSESSION_H
#define COREIRCSESSION_H

#include "global.h"
#include <QObject>
#include <QStringList>

class CoreIrcSessionPrivate;

class LIBIRCCLIENT_QT_EXPORT CoreIrcSession : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QStringList autoJoinChannels READ autoJoinChannels WRITE setAutoJoinChannels)

public:
    explicit CoreIrcSession(QObject* parent = 0);
    virtual ~CoreIrcSession();

    int id() const;

    QStringList autoJoinChannels() const;
    void addAutoJoinChannel(const QString& channel);
    void removeAutoJoinChannel(const QString& channel);
    void setAutoJoinChannels(const QStringList& channels);

    int error() const;
    QString errorString() const;

    void setOption(uint option);
    void resetOption(uint option);

    void connectSlotsByName(QObject* receiver);

    QString host() const;
    quint16 port() const;
    QString nickName() const;
    QString userName() const;
    QString realName() const;
    QString password() const;

    bool isConnected() const;
    bool connectToServer(const QString& host, quint16 port, const QString& nickName, const QString& userName, const QString& realName, const QString& password = QString());

public slots:
    void disconnectFromServer();
    bool exec();

    bool sendRaw(const QString& message);

    bool cmdQuit(const QString& reason = QString());
    bool cmdJoin(const QString& channel, const QString& key = QString());
    bool cmdPart(const QString& channel);
    bool cmdInvite(const QString& nick, const QString& channel);
    bool cmdNames(const QString& channel);
    bool cmdList(const QString& channel);
    bool cmdTopic(const QString& channel, const QString& topic = QString());
    bool cmdChannelMode(const QString& channel, const QString& mode);
    bool cmdUserMode(const QString& mode);
    bool cmdNick(const QString& nick);
    bool cmdWhois(const QString& nick);
    bool cmdMsg(const QString& receiver, const QString& message);
    bool cmdMe(const QString& receiver, const QString& message);
    bool cmdNotice(const QString& receiver, const QString& message);
    bool cmdKick(const QString& nick, const QString& channel, const QString& reason = QString());
    bool cmdCtcpRequest(const QString& nick, const QString& request);
    bool cmdCtcpReply(const QString& nick, const QString& reply);
/*
    bool dccChat(void* ctx, const QString& nick, irc_dcc_callback_t callback, irc_dcc_t * dccid);
    bool dccMsg(irc_dcc_t dccid, const QString& text);
    bool dccAccept(irc_dcc_t dccid, void * ctx, irc_dcc_callback_t callback);
    bool dccDecline(irc_dcc_t dccid);
    bool dccSendFile(void * ctx, const QString& nick, const QString& filename, irc_dcc_callback_t callback, irc_dcc_t * dccid);
    bool dccDestroy(irc_dcc_t dccid);
*/

signals:
    void connected();
    void nickChanged(const QString& origin, const QString& nick);
    void quit(const QString& origin, const QString& message);
    void joined(const QString& origin, const QString& channel);
    void parted(const QString& origin, const QString& channel, const QString& message);
    void channelModeChanged(const QString& origin, const QString& channel, const QString& mode, const QString& args);
    void userModeChanged(const QString& origin, const QString& mode);
    void topicChanged(const QString& origin, const QString& channel, const QString& topic);
    void kicked(const QString& origin, const QString& channel, const QString& nick, const QString& message);
    void channelMessageReceived(const QString& origin, const QString& channel, const QString& message);
    void privateMessageReceived(const QString& origin, const QString& receiver, const QString& message);
    void noticeReceived(const QString& origin, const QString& receiver, const QString& message);
    void invited(const QString& origin, const QString& nick, const QString& channel);
    void ctcpRequestReceived(const QString& origin, const QString& request);
    void ctcpReplyReceived(const QString& origin, const QString& reply);
    void ctcpActionReceived(const QString& origin, const QString& receiver, const QString& action);
    void unknownMessageReceived(const QString& origin, const QStringList& params);
    void numericMessageReceived(const QString& origin, uint event, const QStringList& params);

private:
    CoreIrcSessionPrivate* d;
};

#endif // COREIRCSESSION_H
