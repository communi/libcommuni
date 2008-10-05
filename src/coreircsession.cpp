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

#include "coreircsession.h"
#include <libircclient.h>
#include <QMetaObject>
#include <QMetaMethod>
#include <QHash>

class CoreIrcSessionPrivate
{
public:
    CoreIrcSessionPrivate(CoreIrcSession* session);
    ~CoreIrcSessionPrivate();

    static void event_connect(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_nick(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_quit(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_join(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_part(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_mode(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_umode(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_topic(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_kick(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_channel(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_privmsg(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_notice(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_invite(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_ctcp_req(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_ctcp_rep(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_ctcp_action(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_unknown(irc_session_t* session, const char* event, const char *origin, const char** params, uint count);
    static void event_numeric(irc_session_t* session, uint event, const char* origin, const char** params, uint count);
    static void event_dcc_chat_req(irc_session_t* session, const char* nick, const char* addr, irc_dcc_t dccid);    
    static void event_dcc_send_req(irc_session_t* session, const char* nick, const char* addr, const char* filename, ulong size, irc_dcc_t dccid);

    static QStringList listFromParams(const char** params, uint count);

    irc_session_t* _session;
    QStringList _channels;
    static int _count;
};

int CoreIrcSessionPrivate::_count = 0;

CoreIrcSessionPrivate::CoreIrcSessionPrivate(CoreIrcSession* session)
    : _session(0)
{
    irc_callbacks_t callbacks;
    callbacks.event_connect      = event_connect;
    callbacks.event_nick         = event_nick;
    callbacks.event_quit         = event_quit;
    callbacks.event_join         = event_join;
    callbacks.event_part         = event_part;
    callbacks.event_mode         = event_mode;
    callbacks.event_umode        = event_umode;
    callbacks.event_topic        = event_topic;
    callbacks.event_kick         = event_kick;
    callbacks.event_channel      = event_channel;
    callbacks.event_privmsg      = event_privmsg;
    callbacks.event_notice       = event_notice;
    callbacks.event_invite       = event_invite;
    callbacks.event_ctcp_req     = event_ctcp_req;
    callbacks.event_ctcp_rep     = event_ctcp_rep;
    callbacks.event_ctcp_action  = event_ctcp_action;
    callbacks.event_unknown      = event_unknown;
    callbacks.event_numeric      = event_numeric;
    callbacks.event_dcc_chat_req = event_dcc_chat_req;
    callbacks.event_dcc_send_req = event_dcc_send_req;
    _session = irc_create_session(&callbacks);

    irc_set_ctx(_session, session);

#ifdef Q_OS_WIN32
    if (!_count++)
    {
        WSAData data;
        WSAStartup(MAKEWORD(2, 2), &data);
    }
#endif // Q_OS_WIN32
}

CoreIrcSessionPrivate::~CoreIrcSessionPrivate()
{
    irc_destroy_session(_session);
#ifdef Q_OS_WIN32
    if (!--_count)
    {
        WSACleanup();
    }
#endif // Q_OS_WIN32
}

/*!
* The "on_connect" event is triggered when the client successfully
* connects to the server, and could send commands to the server.
* No extra params supplied; \a params is 0.
*/
void CoreIrcSessionPrivate::event_connect(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(count);
    Q_UNUSED(params);
    Q_UNUSED(event);
    Q_UNUSED(origin);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        //emit context->connected();
        QMetaObject::invokeMethod(context, "connected", Qt::DirectConnection);

        foreach (const QString& channel, context->autoJoinChannels())
        {
            context->cmdJoin(channel);
        }
    }
}

/*!
* The "nick" event is triggered when the client receives a NICK message,
* meaning that someone (including you) on a channel with the client has
* changed their nickname.
*
* \param origin the person, who changes the nick. Note that it can be you!
* \param params[0] mandatory, contains the new nick.
*/
void CoreIrcSessionPrivate::event_nick(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->nickChanged(origin, list.value(0));
        QMetaObject::invokeMethod(context, "nickChanged", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

/*!
* The "quit" event is triggered upon receipt of a QUIT message, which
* means that someone on a channel with the client has disconnected.
*
* \param origin the person, who is disconnected
* \param params[0] optional, contains the reason message (user-specified).
*/
void CoreIrcSessionPrivate::event_quit(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->quit(origin, list.value(0));
        QMetaObject::invokeMethod(context, "quit", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

/*!
* The "join" event is triggered upon receipt of a JOIN message, which
* means that someone has entered a channel that the client is on.
*
* \param origin the person, who joins the channel. By comparing it with
*               your own nickname, you can check whether your JOIN
*               command succeed.
* \param params[0] mandatory, contains the channel name.
*/
void CoreIrcSessionPrivate::event_join(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->joined(origin, list.value(0));
        QMetaObject::invokeMethod(context, "joined", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

/*!
* The "part" event is triggered upon receipt of a PART message, which
* means that someone has left a channel that the client is on.
*
* \param origin the person, who leaves the channel. By comparing it with
*               your own nickname, you can check whether your PART
*               command succeed.
* \param params[0] mandatory, contains the channel name.
* \param params[1] optional, contains the reason message (user-defined).
*/
void CoreIrcSessionPrivate::event_part(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->parted(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "parted", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

/*!
* The "mode" event is triggered upon receipt of a channel MODE message,
* which means that someone on a channel with the client has changed the
* channel's parameters.
*
* \param origin the person, who changed the channel mode.
* \param params[0] mandatory, contains the channel name.
* \param params[1] mandatory, contains the changed channel mode, like
*        '+t', '-i' and so on.
* \param params[2] optional, contains the mode argument (for example, a
*      key for +k mode, or user who got the channel operator status for
*      +o mode)
*/
void CoreIrcSessionPrivate::event_mode(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->channelModeChanged(origin, list.value(0), list.value(1), list.value(2));
        QMetaObject::invokeMethod(context, "channelModeChanged", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)), Q_ARG(QString, list.value(2)));
    }
}

/*!
* The "umode" event is triggered upon receipt of a user MODE message,
* which means that your user mode has been changed.
*
* \param origin the person, who changed the channel mode.
* \param params[0] mandatory, contains the user changed mode, like
*        '+t', '-i' and so on.
*/
void CoreIrcSessionPrivate::event_umode(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->userModeChanged(origin, list.value(0));
        QMetaObject::invokeMethod(context, "userModeChanged", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

/*!
* The "topic" event is triggered upon receipt of a TOPIC message, which
* means that someone on a channel with the client has changed the
* channel's topic.
*
* \param origin the person, who changes the channel topic.
* \param params[0] mandatory, contains the channel name.
* \param params[1] optional, contains the new topic.
*/
void CoreIrcSessionPrivate::event_topic(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->topicChanged(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "topicChanged", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

/*!
* The "kick" event is triggered upon receipt of a KICK message, which
* means that someone on a channel with the client (or possibly the
* client itself!) has been forcibly ejected.
*
* \param origin the person, who kicked the poor.
* \param params[0] mandatory, contains the channel name.
* \param params[0] optional, contains the nick of kicked person.
* \param params[1] optional, contains the kick text
*/
void CoreIrcSessionPrivate::event_kick(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->kicked(origin, list.value(0), list.value(1), list.value(2));
        QMetaObject::invokeMethod(context, "kicked", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)), Q_ARG(QString, list.value(2)));
    }
}

/*!
* The "channel" event is triggered upon receipt of a PRIVMSG message
* to an entire channel, which means that someone on a channel with
* the client has said something aloud. Your own messages don't trigger
* PRIVMSG event.
*
* \param origin the person, who generates the message.
* \param params[0] mandatory, contains the channel name.
* \param params[1] optional, contains the message text
*/
void CoreIrcSessionPrivate::event_channel(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->channelMessageReceived(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "channelMessageReceived", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

/*!
* The "privmsg" event is triggered upon receipt of a PRIVMSG message
* which is addressed to one or more clients, which means that someone
* is sending the client a private message.
*
* \param origin the person, who generates the message.
* \param params[0] mandatory, contains your nick.
* \param params[1] optional, contains the message text
*/
void CoreIrcSessionPrivate::event_privmsg(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->privateMessageReceived(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "privateMessageReceived", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

/*!
* The "notice" event is triggered upon receipt of a NOTICE message
* which means that someone has sent the client a public or private
* notice. According to RFC 1459, the only difference between NOTICE
* and PRIVMSG is that you should NEVER automatically reply to NOTICE
* messages. Unfortunately, this rule is frequently violated by IRC
* servers itself - for example, NICKSERV messages require reply, and
* are NOTICEs.
*
* \param origin the person, who generates the message.
* \param params[0] mandatory, contains the channel name.
* \param params[1] optional, contains the message text
*/
void CoreIrcSessionPrivate::event_notice(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->noticeReceived(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "noticeReceived", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

/*!
* The "invite" event is triggered upon receipt of an INVITE message,
* which means that someone is permitting the client's entry into a +i
* channel.
*
* \param origin the person, who INVITEs you.
* \param params[0] mandatory, contains your nick.
* \param params[1] mandatory, contains the channel name you're invited into.
*
* \sa irc_cmd_invite irc_cmd_chanmode_invite
*/
void CoreIrcSessionPrivate::event_invite(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->invited(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "invited", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

/*!
* The "ctcp" event is triggered when the client receives the CTCP
* request. By default, the built-in CTCP est handler is used. The
* build-in handler automatically replies on most CTCP messages, so you
* will rarely need to override it.
*
* \param origin the person, who generates the message.
* \param params[0] mandatory, the complete CTCP message, including its
*                  arguments.
*
* Mirc generates PING, FINGER, VERSION, TIME and ACTION messages,
* check the source code of \c libirc_event_ctcp_internal function to
* see how to write your own CTCP request handler. Also you may find
* useful this question in FAQ: \ref faq4
*/
void CoreIrcSessionPrivate::event_ctcp_req(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->ctcpRequestReceived(origin, list.value(0));
        QMetaObject::invokeMethod(context, "ctcpRequestReceived", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

/*!
* The "ctcp" event is triggered when the client receives the CTCP reply.
*
* \param origin the person, who generates the message.
* \param params[0] mandatory, the CTCP message itself with its arguments.
*/
void CoreIrcSessionPrivate::event_ctcp_rep(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->ctcpReplyReceived(origin, list.value(0));
        QMetaObject::invokeMethod(context, "ctcpReplyReceived", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

/*!
* The "action" event is triggered when the client receives the CTCP
* ACTION message. These messages usually looks like:\n
* \code
* [23:32:55] * Tim gonna sleep.
* \endcode
*
* \param origin the person, who generates the message.
* \param params[0] mandatory, the ACTION message.
*/
void CoreIrcSessionPrivate::event_ctcp_action(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->ctcpActionReceived(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "ctcpActionReceived", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

/*!
* The "unknown" event is triggered upon receipt of any number of
* unclassifiable miscellaneous messages, which aren't handled by the
* library.
*/
void CoreIrcSessionPrivate::event_unknown(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->unknownMessageReceived(origin, list);
        QMetaObject::invokeMethod(context, "unknownMessageReceived", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(QStringList, list));
    }
}

/*!
* The "numeric" event is triggered upon receipt of any numeric response
* from the server. There is a lot of such responses, see the full list
* here: \ref rfcnumbers.
*
* See the params in ::irc_eventcode_callback_t specification.
*/
void CoreIrcSessionPrivate::event_numeric(irc_session_t* session, uint event, const char* origin, const char** params, uint count)
{
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->numericMessageReceived(origin, event, list);
        QMetaObject::invokeMethod(context, "numericMessageReceived", Qt::DirectConnection, Q_ARG(QString, origin), Q_ARG(uint, event), Q_ARG(QStringList, list));
    }
}

/*!
* The "dcc chat" event is triggered when someone requests a DCC CHAT from
* you.
*
* See the params in ::irc_event_dcc_chat_t specification.
*/
void CoreIrcSessionPrivate::event_dcc_chat_req(irc_session_t* session, const char* nick, const char* addr, irc_dcc_t dccid)
{
    Q_UNUSED(nick);
    Q_UNUSED(addr);
    Q_UNUSED(dccid);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
    }
}

/*!
* The "dcc chat" event is triggered when someone wants to send a file
* to you via DCC SEND request.
*
* See the params in ::irc_event_dcc_send_t specification.
*/
void CoreIrcSessionPrivate::event_dcc_send_req(irc_session_t* session, const char* nick, const char* addr, const char* filename, ulong size, irc_dcc_t dccid)
{
    Q_UNUSED(nick);
    Q_UNUSED(addr);
    Q_UNUSED(filename);
    Q_UNUSED(size);
    Q_UNUSED(dccid);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
    }
}

QStringList CoreIrcSessionPrivate::listFromParams(const char** params, uint count)
{
    QStringList list;
    for (uint i = 0; i < count; ++i)
        list += QString::fromUtf8(params[i]);
    return list;
}

CoreIrcSession::CoreIrcSession(QObject* parent)
    : QObject(parent), d(new CoreIrcSessionPrivate(this))
{
}

CoreIrcSession::~CoreIrcSession()
{
    delete d;
}

QStringList CoreIrcSession::autoJoinChannels() const
{
    return d->_channels;
}

void CoreIrcSession::addAutoJoinChannel(const QString& channel)
{
    if (!d->_channels.contains(channel, Qt::CaseInsensitive))
    {
        d->_channels.append(channel);
    }
}

void CoreIrcSession::removeAutoJoinChannel(const QString& channel)
{
    int index = d->_channels.indexOf(QRegExp(channel, Qt::CaseInsensitive));
    if (index != -1)
    {
        d->_channels.removeAt(index);
    }
}

void CoreIrcSession::setAutoJoinChannels(const QStringList& channels)
{
    d->_channels = channels;
}

int CoreIrcSession::error() const
{
    return irc_errno(d->_session);
}

QString CoreIrcSession::errorString() const
{
    const char* str = irc_strerror(error());
    return QString::fromAscii(str);
}

void CoreIrcSession::setOption(uint option)
{
    irc_option_set(d->_session, option);
}

void CoreIrcSession::resetOption(uint option)
{
    irc_option_reset(d->_session, option);
}

void CoreIrcSession::connectSlotsByName(QObject* receiver)
{
    if (!receiver)
        return;
    const QMetaObject *thisMo = metaObject();
    const QMetaObject *thatMo = receiver->metaObject();
    Q_ASSERT(thisMo && thatMo);
    for (int j = 0; j < thatMo->methodCount(); ++j) {
        QMetaMethod slot = thatMo->method(j);
        const char* slotSignature = slot.signature();
        Q_ASSERT(slotSignature);
        if (qstrncmp(slotSignature, "on_", 3))
            continue;
        for (int i = 0; i < thisMo->methodCount(); ++i) {
            QMetaMethod signal = thisMo->method(i);
            if (signal.methodType() == QMetaMethod::Signal) {
                const char* signalSignature = signal.signature();
                Q_ASSERT(signalSignature);
                if (qstrcmp(slotSignature + 3, signalSignature))
                    continue;
                QMetaObject::connect(this, i, receiver, j);
            }
        }
    }
}

bool CoreIrcSession::isConnected() const
{
    return irc_is_connected(d->_session);
}

bool CoreIrcSession::connectToServer(const QString& host,
                                 quint16 port,
                                 const QString& nickName,
                                 const QString& userName,
                                 const QString& realName,
                                 const QString& password)
{
    return irc_connect(d->_session,
                       host.toUtf8(),
                       port,
                       password.toUtf8(),
                       nickName.toUtf8(),
                       userName.toUtf8(),
                       realName.toUtf8()) == 0;
}

void CoreIrcSession::disconnectFromServer()
{
    irc_disconnect(d->_session);
}

bool CoreIrcSession::exec()
{
    return irc_run(d->_session) == 0;
}

bool CoreIrcSession::sendRaw(const QString& message)
{
    int res = irc_send_raw(d->_session, message.toUtf8());
    return res == 0;
}

bool CoreIrcSession::cmdQuit(const QString& reason)
{
    return irc_cmd_quit(d->_session, reason.toUtf8()) == 0;
}

bool CoreIrcSession::cmdJoin(const QString& channel, const QString& key)
{
    return irc_cmd_join(d->_session, channel.toUtf8(), key.toUtf8()) == 0;
}

bool CoreIrcSession::cmdPart(const QString& channel)
{
    return irc_cmd_part(d->_session, channel.toUtf8()) == 0;
}

bool CoreIrcSession::cmdInvite(const QString& nick, const QString& channel)
{
    return irc_cmd_invite(d->_session, nick.toUtf8(), channel.toUtf8()) == 0;
}

bool CoreIrcSession::cmdNames(const QString& channel)
{
    return irc_cmd_names(d->_session, channel.toUtf8()) == 0;
}

bool CoreIrcSession::cmdList(const QString& channel)
{
    return irc_cmd_list(d->_session, channel.toUtf8()) == 0;
}

bool CoreIrcSession::cmdTopic(const QString& channel, const QString& topic)
{
    if (topic.isEmpty())
        return irc_cmd_topic(d->_session, channel.toUtf8(), 0);
    else
        return irc_cmd_topic(d->_session, channel.toUtf8(), topic.toUtf8());
}

bool CoreIrcSession::cmdChannelMode(const QString& channel, const QString& mode)
{
    return irc_cmd_channel_mode(d->_session, channel.toUtf8(), mode.toUtf8()) == 0;
}

bool CoreIrcSession::cmdUserMode(const QString& mode)
{
    return irc_cmd_user_mode(d->_session, mode.toUtf8()) == 0;
}

bool CoreIrcSession::cmdNick(const QString& nick)
{
    return irc_cmd_nick(d->_session, nick.toUtf8()) == 0;
}

bool CoreIrcSession::cmdWhois(const QString& nick)
{
    return irc_cmd_whois(d->_session, nick.toUtf8()) == 0;
}

bool CoreIrcSession::cmdMsg(const QString& nch, const QString& text)
{
    return irc_cmd_msg(d->_session, nch.toUtf8(), text.toUtf8()) == 0;
}

bool CoreIrcSession::cmdMe(const QString& nch, const QString& text)
{
    return irc_cmd_me(d->_session, nch.toUtf8(), text.toUtf8()) == 0;
}

bool CoreIrcSession::cmdNotice(const QString& nch, const QString& text)
{
    return irc_cmd_notice(d->_session, nch.toUtf8(), text.toUtf8()) == 0;
}

bool CoreIrcSession::cmdKick(const QString& nick, const QString& channel, const QString& reason)
{
    return irc_cmd_kick(d->_session, nick.toUtf8(), channel.toUtf8(), reason.toUtf8()) == 0;
}

bool CoreIrcSession::cmdCtcpRequest(const QString& nick, const QString& request)
{
    return irc_cmd_ctcp_request(d->_session, nick.toUtf8(), request.toUtf8()) == 0;
}

bool CoreIrcSession::cmdCtcpReply(const QString& nick, const QString& reply)
{
    return irc_cmd_ctcp_reply(d->_session, nick.toUtf8(), reply.toUtf8()) == 0;
}
