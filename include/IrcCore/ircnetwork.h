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

#ifndef IRCNETWORK_H
#define IRCNETWORK_H

#include <IrcGlobal>
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qscopedpointer.h>

class IrcSession;
class IrcNetworkPrivate;

class IRC_CORE_EXPORT IrcNetwork : public QObject
{
    Q_OBJECT

public:
    explicit IrcNetwork(IrcSession* session);
    virtual ~IrcNetwork();

    bool isValid() const;

    QString network() const;

    QStringList modes() const;
    QStringList prefixes() const;

    QString modeToPrefix(const QString& mode) const;
    QString prefixToMode(const QString& prefix) const;

    QStringList channelTypes() const;

    enum ModeType {
        TypeA    = 0x1,
        TypeB    = 0x2,
        TypeC    = 0x4,
        TypeD    = 0x8,
        AllTypes = 0xf
    };
    Q_DECLARE_FLAGS(ModeTypes, ModeType)

    QStringList channelModes(ModeTypes types) const;

    enum Limit {
        NickLength,
        ChannelLength,
        TopicLength,
        MessageLength,
        KickReasonLength,
        AwayReasonLength,
        ModeCount
    };

    int numericLimit(Limit limit) const;

    int modeLimit(const QString& mode = QString()) const;
    int channelLimit(const QString& type = QString()) const;
    int targetLimit(const QString& command = QString()) const;

    QStringList availableCapabilities() const;
    QStringList activeCapabilities() const;

private:
    QScopedPointer<IrcNetworkPrivate> d_ptr;
    Q_DECLARE_PRIVATE(IrcNetwork)
    Q_DISABLE_COPY(IrcNetwork)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(IrcNetwork::ModeTypes)

#endif // IRCNETWORK_H
