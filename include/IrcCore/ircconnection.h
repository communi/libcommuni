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

#ifndef IRCCONNECTION_H
#define IRCCONNECTION_H

#include <IrcGlobal>
#include <IrcMessage>
#include <IrcNetwork>
#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qscopedpointer.h>
#include <QtNetwork/qabstractsocket.h>

IRC_BEGIN_NAMESPACE

class IrcCommand;
class IrcProtocol;
class IrcConnectionPrivate;

class IRC_CORE_EXPORT IrcConnection : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString nickName READ nickName WRITE setNickName NOTIFY nickNameChanged)
    Q_PROPERTY(QString realName READ realName WRITE setRealName NOTIFY realNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QVariant userData READ userData WRITE setUserData NOTIFY userDataChanged)
    Q_PROPERTY(QByteArray encoding READ encoding WRITE setEncoding)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool active READ isActive NOTIFY statusChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY statusChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(int reconnectDelay READ reconnectDelay WRITE setReconnectDelay NOTIFY reconnectDelayChanged)
    Q_PROPERTY(QAbstractSocket* socket READ socket WRITE setSocket)
    Q_PROPERTY(bool secure READ isSecure WRITE setSecure NOTIFY secureChanged)
    Q_PROPERTY(QString saslMechanism READ saslMechanism WRITE setSaslMechanism NOTIFY saslMechanismChanged)
    Q_PROPERTY(QStringList supportedSaslMechanisms READ supportedSaslMechanisms CONSTANT)
    Q_PROPERTY(IrcNetwork* network READ network CONSTANT)
    Q_ENUMS(Status)

public:
    explicit IrcConnection(QObject* parent = 0);
    explicit IrcConnection(const QString& host, QObject* parent = 0);
    virtual ~IrcConnection();

    QString host() const;
    void setHost(const QString& host);

    int port() const;
    void setPort(int port);

    QString userName() const;
    void setUserName(const QString& name);

    QString nickName() const;
    void setNickName(const QString& name);

    QString realName() const;
    void setRealName(const QString& name);

    QString password() const;
    void setPassword(const QString& password);

    QString displayName() const;
    void setDisplayName(const QString& name);

    QVariant userData() const;
    void setUserData(const QVariant& data);

    QByteArray encoding() const;
    void setEncoding(const QByteArray& encoding);

    enum Status {
        Inactive,
        Waiting,
        Connecting,
        Connected,
        Closing,
        Closed,
        Error
    };
    Status status() const;
    bool isActive() const;
    bool isConnected() const;
    bool isEnabled() const;

    int reconnectDelay() const;
    void setReconnectDelay(int seconds);

    QAbstractSocket* socket() const;
    void setSocket(QAbstractSocket* socket);

    bool isSecure() const;
    void setSecure(bool secure);

    QString saslMechanism() const;
    void setSaslMechanism(const QString& mechanism);

    static QStringList supportedSaslMechanisms();

    IrcNetwork* network() const;

    void installMessageFilter(QObject* filter);
    void removeMessageFilter(QObject* filter);

    void installCommandFilter(QObject* filter);
    void removeCommandFilter(QObject* filter);

    Q_INVOKABLE QByteArray saveState(int version = 0) const;
    Q_INVOKABLE bool restoreState(const QByteArray& state, int version = 0);

public Q_SLOTS:
    void open();
    void close();
    void quit(const QString& reason = QString());
    void setEnabled(bool enabled = true);
    void setDisabled(bool disabled = true);

    bool sendCommand(IrcCommand* command);
    bool sendData(const QByteArray& data);
    bool sendRaw(const QString& message);

Q_SIGNALS:
    void connecting();
    void nickNameReserved(QString* alternate);
    void connected();
    void disconnected();
    void statusChanged(IrcConnection::Status status);
    void socketError(QAbstractSocket::SocketError error);
    void socketStateChanged(QAbstractSocket::SocketState state);

    void messageReceived(IrcMessage* message);

    void capabilityMessageReceived(IrcCapabilityMessage* message);
    void errorMessageReceived(IrcErrorMessage* message);
    void inviteMessageReceived(IrcInviteMessage* message);
    void joinMessageReceived(IrcJoinMessage* message);
    void kickMessageReceived(IrcKickMessage* message);
    void modeMessageReceived(IrcModeMessage* message);
    void namesMessageReceived(IrcNamesMessage* message);
    void nickMessageReceived(IrcNickMessage* message);
    void noticeMessageReceived(IrcNoticeMessage* message);
    void numericMessageReceived(IrcNumericMessage* message);
    void motdMessageReceived(IrcMotdMessage* message);
    void partMessageReceived(IrcPartMessage* message);
    void pingMessageReceived(IrcPingMessage* message);
    void pongMessageReceived(IrcPongMessage* message);
    void privateMessageReceived(IrcPrivateMessage* message);
    void quitMessageReceived(IrcQuitMessage* message);
    void topicMessageReceived(IrcTopicMessage* message);
    void whoReplyMessageReceived(IrcWhoReplyMessage* message);

    void hostChanged(const QString& host);
    void portChanged(int port);
    void userNameChanged(const QString& name);
    void nickNameChanged(const QString& name);
    void realNameChanged(const QString& name);
    void passwordChanged(const QString& password);
    void displayNameChanged(const QString& name);
    void userDataChanged(const QVariant& data);

    void reconnectDelayChanged(int seconds);
    void enabledChanged(bool enabled);
    void secureChanged(bool secure);
    void saslMechanismChanged(const QString& mechanism);

    void destroyed(IrcConnection* connection);

protected Q_SLOTS:
    virtual IrcCommand* createCtcpReply(IrcPrivateMessage* request) const;

protected:
    IrcProtocol* protocol() const;
    void setProtocol(IrcProtocol* protocol);

private:
    friend class IrcProtocol;
    friend class IrcProtocolPrivate;
    QScopedPointer<IrcConnectionPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcConnection)
    Q_DISABLE_COPY(IrcConnection)

    Q_PRIVATE_SLOT(d_func(), void _irc_connected())
    Q_PRIVATE_SLOT(d_func(), void _irc_disconnected())
    Q_PRIVATE_SLOT(d_func(), void _irc_error(QAbstractSocket::SocketError))
    Q_PRIVATE_SLOT(d_func(), void _irc_state(QAbstractSocket::SocketState))
    Q_PRIVATE_SLOT(d_func(), void _irc_reconnect())
    Q_PRIVATE_SLOT(d_func(), void _irc_readData())
    Q_PRIVATE_SLOT(d_func(), void _irc_filterDestroyed(QObject*))
};

#ifndef QT_NO_DEBUG_STREAM
IRC_CORE_EXPORT QDebug operator<<(QDebug debug, IrcConnection::Status status);
IRC_CORE_EXPORT QDebug operator<<(QDebug debug, const IrcConnection* connection);
#endif // QT_NO_DEBUG_STREAM

IRC_END_NAMESPACE

Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcConnection*))
Q_DECLARE_METATYPE(IRC_PREPEND_NAMESPACE(IrcConnection::Status))

#endif // IRCCONNECTION_H
