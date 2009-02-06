/*
 * Copyright (C) 2008-2009 J-P Nurmi jpnurmi@gmail.com
 *
 * This example is free, and not covered by LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially. By using it you may give me some credits in your
 * program, but you don't have to.
 *
 * $Id$
 */

#include "session.h"
#include <QtCore>

MyIrcSession::MyIrcSession(QObject* parent) : Irc::Session(parent)
{
    connectSlotsByName(this);
}

void MyIrcSession::on_connected()
{
    qDebug() << "connected:";
}

void MyIrcSession::on_disconnected()
{
    qDebug() << "disconnected:";
}

void MyIrcSession::on_msgJoined(const QString& origin, const QString& channel)
{
    qDebug() << "join:" << origin << channel;
}

void MyIrcSession::on_msgParted(const QString& origin, const QString& channel, const QString& message)
{
    qDebug() << "part:" << origin << channel << message;
}

void MyIrcSession::on_msgQuit(const QString& origin, const QString& message)
{
    qDebug() << "quit:" << origin << message;
}

void MyIrcSession::on_msgNickChanged(const QString& origin, const QString& nick)
{
    qDebug() << "nick:" << origin << nick;
}

void MyIrcSession::on_msgModeChanged(const QString& origin, const QString& receiver, const QString& mode, const QString& args)
{
    qDebug() << "mode:" << origin << receiver << mode << args;
}

void MyIrcSession::on_msgTopicChanged(const QString& origin, const QString& channel, const QString& topic)
{
    qDebug() << "topic:" << origin << channel << topic;
}

void MyIrcSession::on_msgInvited(const QString& origin, const QString& receiver, const QString& channel)
{
    qDebug() << "invite:" << origin << receiver << channel;
}

void MyIrcSession::on_msgKicked(const QString& origin, const QString& channel, const QString& nick, const QString& message)
{
    qDebug() << "kick:" << origin << channel << nick << message;
}

void MyIrcSession::on_msgMessageReceived(const QString& origin, const QString& receiver, const QString& message)
{
    qDebug() << "message:" << origin << receiver << message;
}

void MyIrcSession::on_msgNoticeReceived(const QString& origin, const QString& receiver, const QString& notice)
{
    qDebug() << "notice:" << origin << receiver << notice;
}

void MyIrcSession::on_msgCtcpRequestReceived(const QString& origin, const QString& request)
{
    qDebug() << "ctcp request:" << origin << request;
}

void MyIrcSession::on_msgCtcpReplyReceived(const QString& origin, const QString& reply)
{
    qDebug() << "ctcp reply:" << origin << reply;
}

void MyIrcSession::on_msgCtcpActionReceived(const QString& origin, const QString& receiver, const QString& action)
{
    qDebug() << "ctcp action:" << origin << receiver << action;
}

void MyIrcSession::on_msgNumericMessageReceived(const QString& origin, uint code, const QStringList& params)
{
    qDebug() << "numeric:" << origin << code << params;
}

void MyIrcSession::on_msgUnknownMessageReceived(const QString& origin, const QStringList& params)
{
    qDebug() << "unknown:" << origin << params;
}
