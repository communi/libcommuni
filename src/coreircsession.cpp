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

/*!
    \class CoreIrcSession coreircsession.h
    \ingroup LibIrcClient-Qt
    \brief CoreIrcSession provides core IRC session functionality.

    \sa IrcSession
 */

/*!
    \fn void CoreIrcSession::connected()

    This signal is emitted when the session has been connected.
 */

/*!
    \fn void CoreIrcSession::nickChanged (const QString &origin, const QString &nick)

    This signal is emitted when \a origin has changed \a nick.
 */

/*!
    \fn void CoreIrcSession::quit(const QString &origin, const QString &message)

    This signal is emitted when \a origin has quit with \a message.
 */

/*!
    \fn void CoreIrcSession::joined(const QString &origin, const QString &channel)

    This signal is emitted when \a origin has joined \a channel.
 */

/*!
    \fn void CoreIrcSession::parted(const QString &origin, const QString &channel, const QString &message)

    This signal is emitted when \a origin has parted \a channel with \a message.
 */

/*!
    \fn void CoreIrcSession::channelModeChanged(const QString &origin, const QString &channel, const QString &mode, const QString &args)

    This signal is emitted when \a origin has changed \a channel \a mode with \a args.
 */

/*!
    \fn void CoreIrcSession::userModeChanged(const QString &origin, const QString &mode)

    This signal is emitted when \a origin has changed user \a mode.
 */

/*!
    \fn void CoreIrcSession::topicChanged(const QString &origin, const QString &channel, const QString &topic)

    This signal is emitted when \a origin has changed \a topic of \a channel.
 */

/*!
    \fn void CoreIrcSession::kicked(const QString &origin, const QString &channel, const QString &nick, const QString &message)

    This signal is emitted when \a origin has kicked \a nick from \a channel with \a message.
 */

/*!
    \fn void CoreIrcSession::channelMessageReceived(const QString &origin, const QString &channel, const QString &message)

    This signal is emitted when \a origin has sent a channel \a message to \a channel.
 */

/*!
    \fn void CoreIrcSession::privateMessageReceived(const QString &origin, const QString &receiver, const QString &message)

    This signal is emitted when \a origin has sent a private \a message to \a receiver.
 */

/*!
    \fn void CoreIrcSession::noticeReceived(const QString &origin, const QString &receiver, const QString &message)

    This signal is emitted when \a origin has sent a notice \a message to \a receiver.
 */

/*!
    \fn void CoreIrcSession::invited(const QString &origin, const QString &nick, const QString &channel)

    This signal is emitted when \a origin has invited \a nick to \a channel.
 */

/*!
    \fn void CoreIrcSession::ctcpRequestReceived(const QString &origin, const QString &request)

    This signal is emitted when \a origin has sent a CTCP \a request.
 */

/*!
    \fn void CoreIrcSession::ctcpReplyReceived(const QString &origin, const QString &reply)

    This signal is emitted when \a origin has sent a CTCP \a reply.
 */

/*!
    \fn void CoreIrcSession::ctcpActionReceived(const QString &origin, const QString &receiver, const QString &action)

    This signal is emitted when \a origin has sent a CTCP \a action to \a receiver.
 */

/*!
    \fn void CoreIrcSession::unknownMessageReceived(const QString &origin, const QStringList &params)

    This signal is emitted when \a origin has sent an unknown message with \a params.
 */

/*!
    \fn void CoreIrcSession::numericMessageReceived(const QString &origin, uint event, const QStringList &params)

    This signal is emitted when a numeric \a event has been receiver from \a origin with \a params.
 */

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
    static int _staticCount;
    static int _staticId;

    int id;
    QString host;
    quint16 port;
    QString nick;
    QString user;
    QString real;
    QString pass;
};

int CoreIrcSessionPrivate::_staticCount = 0;
int CoreIrcSessionPrivate::_staticId = 0;

CoreIrcSessionPrivate::CoreIrcSessionPrivate(CoreIrcSession* session)
    : _session(0), id(_staticId++)
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
    if (!_staticCount++)
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
    if (!--_staticCount)
    {
        WSACleanup();
    }
#endif // Q_OS_WIN32
}

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
        QMetaObject::invokeMethod(context, "connected");

        foreach (const QString& channel, context->autoJoinChannels())
        {
            context->cmdJoin(channel);
        }
    }
}

void CoreIrcSessionPrivate::event_nick(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->nickChanged(origin, list.value(0));
        QMetaObject::invokeMethod(context, "nickChanged", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

void CoreIrcSessionPrivate::event_quit(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->quit(origin, list.value(0));
        QMetaObject::invokeMethod(context, "quit", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

void CoreIrcSessionPrivate::event_join(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->joined(origin, list.value(0));
        QMetaObject::invokeMethod(context, "joined", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

void CoreIrcSessionPrivate::event_part(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->parted(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "parted", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

void CoreIrcSessionPrivate::event_mode(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->channelModeChanged(origin, list.value(0), list.value(1), list.value(2));
        QMetaObject::invokeMethod(context, "channelModeChanged", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)), Q_ARG(QString, list.value(2)));
    }
}

void CoreIrcSessionPrivate::event_umode(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->userModeChanged(origin, list.value(0));
        QMetaObject::invokeMethod(context, "userModeChanged", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

void CoreIrcSessionPrivate::event_topic(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->topicChanged(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "topicChanged", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

void CoreIrcSessionPrivate::event_kick(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->kicked(origin, list.value(0), list.value(1), list.value(2));
        QMetaObject::invokeMethod(context, "kicked", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)), Q_ARG(QString, list.value(2)));
    }
}

void CoreIrcSessionPrivate::event_channel(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->channelMessageReceived(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "channelMessageReceived", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

void CoreIrcSessionPrivate::event_privmsg(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->privateMessageReceived(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "privateMessageReceived", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

void CoreIrcSessionPrivate::event_notice(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->noticeReceived(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "noticeReceived", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

void CoreIrcSessionPrivate::event_invite(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->invited(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "invited", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

void CoreIrcSessionPrivate::event_ctcp_req(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->ctcpRequestReceived(origin, list.value(0));
        QMetaObject::invokeMethod(context, "ctcpRequestReceived", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

void CoreIrcSessionPrivate::event_ctcp_rep(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->ctcpReplyReceived(origin, list.value(0));
        QMetaObject::invokeMethod(context, "ctcpReplyReceived", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)));
    }
}

void CoreIrcSessionPrivate::event_ctcp_action(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->ctcpActionReceived(origin, list.value(0), list.value(1));
        QMetaObject::invokeMethod(context, "ctcpActionReceived", Q_ARG(QString, origin), Q_ARG(QString, list.value(0)), Q_ARG(QString, list.value(1)));
    }
}

void CoreIrcSessionPrivate::event_unknown(irc_session_t* session, const char* event, const char *origin, const char** params, uint count)
{
    Q_UNUSED(event);
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        QStringList list = listFromParams(params, count);
        //emit context->unknownMessageReceived(origin, list);
        QMetaObject::invokeMethod(context, "unknownMessageReceived", Q_ARG(QString, origin), Q_ARG(QStringList, list));
    }
}

void CoreIrcSessionPrivate::event_numeric(irc_session_t* session, uint event, const char* origin, const char** params, uint count)
{
    CoreIrcSession* context = (CoreIrcSession*) irc_get_ctx(session);
    if (context)
    {
        switch (event)
        {
            case LIBIRC_RFC_RPL_WELCOME:
                context->d->host = origin;
                break;

            default:
                break;
        }

        QStringList list = listFromParams(params, count);
        //emit context->numericMessageReceived(origin, event, list);
        QMetaObject::invokeMethod(context, "numericMessageReceived", Q_ARG(QString, origin), Q_ARG(uint, event), Q_ARG(QStringList, list));
    }
}

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

/*!
    Constructs a CoreIrcSession with \a parent.
 */
CoreIrcSession::CoreIrcSession(QObject* parent)
    : QObject(parent), d(new CoreIrcSessionPrivate(this))
{
}

/*!
    Destructs the IRC session.
 */
CoreIrcSession::~CoreIrcSession()
{
    delete d;
}

/*!
    Returns the session id.
 */
int CoreIrcSession::id() const
{
    return d->id;
}

/*!
    Returns the list of auto-join channels.
 */
QStringList CoreIrcSession::autoJoinChannels() const
{
    return d->_channels;
}

/*!
    Adds \a channel to the list of auto-join channels.
 */
void CoreIrcSession::addAutoJoinChannel(const QString& channel)
{
    if (!d->_channels.contains(channel, Qt::CaseInsensitive))
    {
        d->_channels.append(channel);
    }
}

/*!
    Removes \a channels from the list of auto-join channels.
 */
void CoreIrcSession::removeAutoJoinChannel(const QString& channel)
{
    int index = d->_channels.indexOf(QRegExp(channel, Qt::CaseInsensitive));
    if (index != -1)
    {
        d->_channels.removeAt(index);
    }
}

/*!
    Sets the list of auto-join \a channels.
 */
void CoreIrcSession::setAutoJoinChannels(const QStringList& channels)
{
    d->_channels = channels;
}

/*!
    Returns the error number.

    \sa errorString()
 */
int CoreIrcSession::error() const
{
    return irc_errno(d->_session);
}

/*!
    Returns the error string.

    \sa error()
 */
QString CoreIrcSession::errorString() const
{
    const char* str = irc_strerror(error());
    return QString::fromAscii(str);
}

/*!
    This functions sets the libircclient \a option.
 */
void CoreIrcSession::setOption(uint option)
{
    irc_option_set(d->_session, option);
}

/*!
    This functions resets the libircclient \a option.
 */
void CoreIrcSession::resetOption(uint option)
{
    irc_option_reset(d->_session, option);
}

/*!
    Connects CoreIrcSession signals to matching slots of the \a receiver.

    Receiver slots must follow the following syntax:
    \code
    void on_<signal name>(<signal parameters>);
    \endcode
 */
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

QString CoreIrcSession::host() const
{
    return d->host;
}

quint16 CoreIrcSession::port() const
{
    return d->port;
}

QString CoreIrcSession::nickName() const
{
    return d->nick;
}

QString CoreIrcSession::userName() const
{
    return d->user;
}

QString CoreIrcSession::realName() const
{
    return d->real;
}

QString CoreIrcSession::password() const
{
    return d->pass;
}

/*!
    Returns \c true if the session is connected to the server.
 */
bool CoreIrcSession::isConnected() const
{
    return irc_is_connected(d->_session);
}

/*!
    Connects to the server.

    \sa exec()
 */
bool CoreIrcSession::connectToServer(const QString& host,
                                 quint16 port,
                                 const QString& nickName,
                                 const QString& userName,
                                 const QString& realName,
                                 const QString& password)
{
    d->host = host;
    d->port = port;
    d->nick = nickName;
    d->user = userName;
    d->real = realName;
    d->pass = password;

    return irc_connect(d->_session,
                       host.toUtf8(),
                       port,
                       password.toUtf8(),
                       nickName.toUtf8(),
                       userName.toUtf8(),
                       realName.toUtf8()) == 0;
}

/*!
    Disconnects from the server.
 */
void CoreIrcSession::disconnectFromServer()
{
    irc_disconnect(d->_session);
}

/*!
    Starts an event loop, processing IRC events and emitting signals.
 */
bool CoreIrcSession::exec()
{
    return irc_run(d->_session) == 0;
}

/*!
    Sends a raw \a message to the server.
 */
bool CoreIrcSession::sendRaw(const QString& message)
{
    int res = irc_send_raw(d->_session, message.toUtf8());
    return res == 0;
}

/*!
    Quits the session with \a reason.
 */
bool CoreIrcSession::cmdQuit(const QString& reason)
{
    return irc_cmd_quit(d->_session, reason.toUtf8()) == 0;
}

/*!
    Joins a \a channel with \a key.
 */
bool CoreIrcSession::cmdJoin(const QString& channel, const QString& key)
{
    return irc_cmd_join(d->_session, channel.toUtf8(), key.toUtf8()) == 0;
}

/*!
    Parts the \a channel.
 */
bool CoreIrcSession::cmdPart(const QString& channel)
{
    return irc_cmd_part(d->_session, channel.toUtf8()) == 0;
}

/*!
    Invites \a nick to \a channel.
 */
bool CoreIrcSession::cmdInvite(const QString& nick, const QString& channel)
{
    return irc_cmd_invite(d->_session, nick.toUtf8(), channel.toUtf8()) == 0;
}

/*!
    Lists names on \a channel.
 */
bool CoreIrcSession::cmdNames(const QString& channel)
{
    return irc_cmd_names(d->_session, channel.toUtf8()) == 0;
}

/*!
    Lists channels.
 */
bool CoreIrcSession::cmdList(const QString& channel)
{
    return irc_cmd_list(d->_session, channel.toUtf8()) == 0;
}

/*!
    Obtains or sets \a topic on \a channel.
 */
bool CoreIrcSession::cmdTopic(const QString& channel, const QString& topic)
{
    if (topic.isEmpty())
        return irc_cmd_topic(d->_session, channel.toUtf8(), 0);
    else
        return irc_cmd_topic(d->_session, channel.toUtf8(), topic.toUtf8());
}

/*!
    Sets \a mode on \a channel.
 */
bool CoreIrcSession::cmdChannelMode(const QString& channel, const QString& mode)
{
    if (mode.isEmpty())
        return irc_cmd_channel_mode(d->_session, channel.toUtf8(), 0) == 0;
    else
        return irc_cmd_channel_mode(d->_session, channel.toUtf8(), mode.toUtf8()) == 0;
}

/*!
    Sets user \a mode.
 */
bool CoreIrcSession::cmdUserMode(const QString& mode)
{
    return irc_cmd_user_mode(d->_session, mode.toUtf8()) == 0;
}

/*!
    Sets \a nick.
 */
bool CoreIrcSession::cmdNick(const QString& nick)
{
    return irc_cmd_nick(d->_session, nick.toUtf8()) == 0;
}

/*!
    Who is \a nick.
 */
bool CoreIrcSession::cmdWhois(const QString& nick)
{
    return irc_cmd_whois(d->_session, nick.toUtf8()) == 0;
}

/*!
    Sends \a message to \a receiver.
 */
bool CoreIrcSession::cmdMessage(const QString& receiver, const QString& message)
{
    return irc_cmd_msg(d->_session, receiver.toUtf8(), message.toUtf8()) == 0;
}

/*!
    Sends a notice \a message to \a receiver.
 */
bool CoreIrcSession::cmdNotice(const QString& receiver, const QString& message)
{
    return irc_cmd_notice(d->_session, receiver.toUtf8(), message.toUtf8()) == 0;
}

/*!
    Kicks \a nick from \a channel with \a reason.
 */
bool CoreIrcSession::cmdKick(const QString& nick, const QString& channel, const QString& reason)
{
    return irc_cmd_kick(d->_session, nick.toUtf8(), channel.toUtf8(), reason.toUtf8()) == 0;
}

/*!
    Sends an action \a message to \a receiver.
 */
bool CoreIrcSession::cmdCtcpAction(const QString& receiver, const QString& message)
{
    return irc_cmd_me(d->_session, receiver.toUtf8(), message.toUtf8()) == 0;
}

/*!
    Sends a CTCP \a request to \a nick.
 */
bool CoreIrcSession::cmdCtcpRequest(const QString& nick, const QString& request)
{
    return irc_cmd_ctcp_request(d->_session, nick.toUtf8(), request.toUtf8()) == 0;
}

/*!
    Sends a CTCP \a reply to \a nick.
 */
bool CoreIrcSession::cmdCtcpReply(const QString& nick, const QString& reply)
{
    return irc_cmd_ctcp_reply(d->_session, nick.toUtf8(), reply.toUtf8()) == 0;
}
