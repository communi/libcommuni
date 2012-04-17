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

#ifndef IRCDECODER_P_H
#define IRCDECODER_P_H

#include <IrcGlobal>
#include <QtCore/qbytearray.h>

class UCharsetDetector;

class COMMUNI_EXPORT IrcDecoder
{
    struct Data
    {
        QByteArray encoding;
        UCharsetDetector* detector;
    };

public:
    IrcDecoder();
    ~IrcDecoder();

    QByteArray encoding() const { return d.encoding; }
    void setEncoding(const QByteArray& encoding) { d.encoding = encoding; }

    QString decode(const QByteArray& data) const;

private:
    QByteArray detectEncoding(const QByteArray& data) const;

    Data d;
};

#endif // IRCDECODER_P_H
