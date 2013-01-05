/*
* Copyright (C) 2008-2013 J-P Nurmi <jpnurmi@gmail.com>
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

#ifndef IRCSESSIONINFO_H
#define IRCSESSIONINFO_H

#include <IrcGlobal>
#include <QtCore/qstringlist.h>
#include <QtCore/qshareddata.h>

class IrcSession;
class IrcSessionInfoPrivate;

class COMMUNI_EXPORT IrcSessionInfo
{
public:
    explicit IrcSessionInfo(const IrcSession* session);
    IrcSessionInfo(const IrcSessionInfo& other);
    IrcSessionInfo& operator=(const IrcSessionInfo& other);
    ~IrcSessionInfo();

    bool isValid() const;

    QString network() const;

    QStringList modes() const;
    QStringList prefixes() const;
    QStringList channelTypes() const;

    QString modeToPrefix(const QString& mode) const;
    QString prefixToMode(const QString& prefix) const;

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

private:
    mutable QSharedDataPointer<IrcSessionInfoPrivate> d;
};

#endif // IRCSESSIONINFO_H
