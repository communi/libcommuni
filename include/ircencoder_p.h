/*
* Copyright (C) 2008-2011 J-P Nurmi <jpnurmi@gmail.com>
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

#ifndef IRCENCODER_P_H
#define IRCENCODER_P_H

#include <QtCore/qbytearray.h>

class UCharsetDetector;

class IrcEncoder
{
    struct Data
    {
        QByteArray encoding;
        UCharsetDetector* detector;
    };

public:
    IrcEncoder();
    ~IrcEncoder();

    QByteArray encoding() const { return d.encoding; }
    void setEncoding(const QByteArray& encoding) { d.encoding = encoding; }

    QString encode(const QByteArray& data) const;

private:
    QByteArray detectEncoding(const QByteArray& data) const;

    Data d;
};

#endif // IRCENCODER_P_H
