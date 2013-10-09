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

#include "ircfilter.h"

IRC_BEGIN_NAMESPACE

/*!
    \file ircfilter.h
    \brief \#include &lt;%IrcFilter&gt;
 */

/*!
    \file ircmessagefilter.h
    \brief \#include &lt;%IrcMessageFilter&gt;
 */

/*!
    \file irccommandfilter.h
    \brief \#include &lt;%IrcCommandFilter&gt;
 */

/*!
    \class IrcMessageFilter ircfilter.h <IrcMessageFilter>
    \ingroup core
    \brief Provides an interface for filtering messages

    IrcMessageFilter may be used to intercept messages before
    IrcConnection::messageReceived() is emitted and the messages get delivered
    further. In order to use IrcMessageFilter, it must be installed via
    IrcConnection::installMessageFilter().

    Message filtering is mostly useful for handling specific replies before
    the rest of the application receives it. This way there is no need to
    for example ignore and hide such replies later in the application code.

    The following example sends a PING command after each PRIVMSG command.
    A consequent PONG reply from the server verifies that the PRIVMSG has
    been also processed.

    \code
    class CommandVerifier : public QObject,
                            public IrcCommandFilter,
                            public IrcMessageFilter
    {
        Q_OBJECT
        Q_INTERFACES(IrcCommandFilter IrcMessageFilter)

    public:
        CommandVerifier(IrcConnection* parent) :
            QObject(parent), identifier(0), connection(parent)
        {
            connection->installMessageFilter(this);
            connection->installCommandFilter(this);
        }

        virtual bool commandFilter(IrcCommand* cmd)
        {
            if (cmd->type() == IrcCommand::Message) {
                cmd->setParent(this); // take ownership
                connection->sendCommand(cmd);
                commands.insert(++identifier, cmd);
                connection->sendData("PING communi/" + QByteArray::number(identifier));
                return true;
            }
            return false;
        }

        virtual bool messageFilter(IrcMessage* msg)
        {
            if (msg->type() == IrcMessage::Pong) {
                QString arg = static_cast<IrcPongMessage*>(msg)->argument();
                if (arg.startsWith("communi/")) {
                    bool ok = false;
                    quint64 id = arg.mid(8).toULongLong(&ok);
                    if (ok) {
                        IrcCommand* command = commands.take(id);
                        if (command) {
                            emit verified(command);
                            command->deleteLater();
                            return true;
                        }
                    }
                }
            }
            return false;
        }

    signals:
        void verified(IrcCommand* cmd);

    private:
        quint64 identifier;
        IrcConnection* connection;
        QMap<quint64, IrcCommand*> commands;
    };
    \endcode

    \sa IrcConnection::installMessageFilter(), IrcCommandFilter
 */

/*!
    \fn IrcMessageFilter::~IrcMessageFilter()
    Destructs the message filter.

    The message filter is automatically removed from any connection(s)
    it is installed on.

    \sa IrcConnection::removeMessageFilter()
 */

/*!
    \fn virtual bool IrcMessageFilter::messageFilter(IrcMessage* message) = 0

    Reimplement this function to filter messages from installed connections.

    Return \c true to filter the message out, i.e. stop it being handled further;
    otherwise return \c false.

    \sa IrcConnection::installMessageFilter()
 */

/*!
    \class IrcCommandFilter ircfilter.h <IrcCommandFilter>
    \ingroup core
    \brief Provides an interface for filtering commands

    IrcCommandFilter may be used to intercept commands before they
    get sent further. In order to use IrcCommandFilter, it must be
    installed via IrcConnection::installCommandFilter().

    Command filtering can be useful doing extra tasks for specific
    type of commands. The following example sends a PING command
    after each PRIVMSG command. A consequent PONG reply from the
    server verifies that the PRIVMSG has been also processed.

    \code
    class CommandVerifier : public QObject,
                            public IrcCommandFilter,
                            public IrcMessageFilter
    {
        Q_OBJECT
        Q_INTERFACES(IrcCommandFilter IrcMessageFilter)

    public:
        CommandVerifier(IrcConnection* parent) :
            QObject(parent), identifier(0), connection(parent)
        {
            connection->installMessageFilter(this);
            connection->installCommandFilter(this);
        }

        virtual bool commandFilter(IrcCommand* cmd)
        {
            if (cmd->type() == IrcCommand::Message) {
                cmd->setParent(this); // take ownership
                connection->sendCommand(cmd);
                commands.insert(++identifier, cmd);
                connection->sendData("PING communi/" + QByteArray::number(identifier));
                return true;
            }
            return false;
        }

        virtual bool messageFilter(IrcMessage* msg)
        {
            if (msg->type() == IrcMessage::Pong) {
                QString arg = static_cast<IrcPongMessage*>(msg)->argument();
                if (arg.startsWith("communi/")) {
                    bool ok = false;
                    quint64 id = arg.mid(8).toULongLong(&ok);
                    if (ok) {
                        IrcCommand* command = commands.take(id);
                        if (command) {
                            emit verified(command);
                            command->deleteLater();
                            return true;
                        }
                    }
                }
            }
            return false;
        }

    signals:
        void verified(IrcCommand* cmd);

    private:
        quint64 identifier;
        IrcConnection* connection;
        QMap<quint64, IrcCommand*> commands;
    };
    \endcode

    \note Notice that it is safe to call IrcConnection::sendCommand()
    from IrcCommandFilter::commandFilter(). Such commands won't get
    delivered back to the \b same filter to avoid recursion.

    \sa IrcConnection::installCommandFilter(), IrcMessageFilter
 */

/*!
    \fn IrcCommandFilter::~IrcCommandFilter()
    Destructs the command filter.

    The command filter is automatically removed from any connection(s)
    it is installed on.

    \sa IrcConnection::removeCommandFilter()
 */

/*!
    \fn virtual bool IrcCommandFilter::commandFilter(IrcCommand* command) = 0

    Reimplement this function to filter commands to installed connections.

    Return \c true to filter the command out, i.e. stop it being handled further;
    otherwise return \c false.

    \sa IrcConnection::installCommandFilter()
 */

IRC_END_NAMESPACE
