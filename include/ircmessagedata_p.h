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

#ifndef IRCMESSAGEDATA_P_H
#define IRCMESSAGEDATA_P_H

#include <IrcGlobal>
#include <QtCore/qlist.h>
#include <QtCore/qbytearray.h>

class COMMUNI_EXPORT IrcMessageData
{
public:
    IrcMessageData() : valid(false) { }

    static IrcMessageData fromData(const QByteArray& data);

    bool valid;
    QByteArray data;
    QByteArray prefix;
    QByteArray command;
    QList<QByteArray> params;
};

#endif // IRCMESSAGEDATA_P_H
