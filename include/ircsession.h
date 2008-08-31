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

#ifndef IRCSESSION_H
#define IRCSESSION_H

#include <QObject>
#include <QStringList>

class IrcSessionPrivate;

class IrcSession : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected)
    Q_PROPERTY(QStringList autoJoinChannels READ autoJoinChannels WRITE setAutoJoinChannels)

public:
    IrcSession(QObject* parent = 0);
    ~IrcSession();

    bool isConnected() const;
    bool connectToServer(const QString& server,
                         quint16 port,
                         const QString& password,
                         const QString& nick,
                         const QString& username,
                         const QString& realname);

public slots:
    void disconnectFromServer();
    bool exec();

public:
    QStringList autoJoinChannels() const;
    void addAutoJoinChannel(const QString& channel);
    void removeAutoJoinChannel(const QString& channel);
    void setAutoJoinChannels(const QStringList& channels);

    bool sendRaw(const char* format, ...);

public slots:
    bool cmdQuit(const QString& reason);
    bool cmdJoin(const QString& channel, const QString& key = QString());
    bool cmdPart(const QString& channel);
    bool cmdInvite(const QString& nick, const QString& channel);
    bool cmdNames(const QString& channel);
    bool cmdList(const QString& channel);
    bool cmdTopic(const QString& channel, const QString& topic);
    bool cmdChannelMode(const QString& channel, const QString& mode);
    bool cmdUserMode(const QString& mode);
    bool cmdNick(const QString& newnick);
    bool cmdWhois(const QString& nick);
    bool cmdMsg(const QString& nch, const QString& text);
    bool cmdMe(const QString& nch, const QString& text);
    bool cmdNotice(const QString& nch, const QString& text);
    bool cmdKick(const QString& nick, const QString& channel, const QString& reason = QString());
    bool cmdCtcpRequest(const QString& nick, const QString& request);
    bool cmdCtcpReply(const QString& nick, const QString& reply);

public:
/*
    int dcc_chat(void* ctx, const QString& nick, irc_dcc_callback_t callback, irc_dcc_t * dccid);
    int dcc_msg(irc_dcc_t dccid, const QString& text);
    int dcc_accept(irc_dcc_t dccid, void * ctx, irc_dcc_callback_t callback);
    int dcc_decline(irc_dcc_t dccid);
    int dcc_sendfile(void * ctx, const QString& nick, const QString& filename, irc_dcc_callback_t callback, irc_dcc_t * dccid);
    int dcc_destroy(irc_dcc_t dccid);
*/

    int error() const;
    QString errorString() const;

    void setOption(uint option);
    void resetOption(uint option);

/*
    int addSelectDescriptors(fd_set* in_set, fd_set* out_set, int* maxfd);
    int processSelectDescriptors(fd_set* in_set, fd_set* out_set);
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
    void ctcpRequestReceived(const QString& origin, const QString& message);
    void ctcpReplyReceived(const QString& origin, const QString& message);
    void ctcpActionReceived(const QString& origin, const QString& message);
    void unknownMessageReceived(const QString& origin, const QStringList& params);

private:
    IrcSessionPrivate* d;
    friend class IrcSessionPrivate;
};

#endif // IRCSESSION_H
