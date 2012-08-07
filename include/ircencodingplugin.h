/*
* Copyright (C) 2008-2012 J-P Nurmi <jpnurmi@gmail.com>
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

#ifndef IRCENCODINGPLUGIN_H
#define IRCENCODINGPLUGIN_H

#include <IrcGlobal>
#include <QtCore/qplugin.h>
#include <QtCore/qbytearray.h>

struct COMMUNI_EXPORT IrcEncodingInterface
{
    virtual QByteArray detectEncoding(const QByteArray& data) = 0;
};

Q_DECLARE_INTERFACE(IrcEncodingInterface, "Communi.IrcEncodingInterface")

class COMMUNI_EXPORT IrcEncodingPlugin : public QObject, public IrcEncodingInterface
{
    Q_OBJECT
    Q_INTERFACES(IrcEncodingInterface)

public:
    explicit IrcEncodingPlugin(QObject* parent = 0);
    virtual ~IrcEncodingPlugin();

    virtual QByteArray detectEncoding(const QByteArray& data) = 0;
};

#endif // IRCENCODINGPLUGIN_H
