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

#ifndef IRCMESSAGEDECODER_P_H
#define IRCMESSAGEDECODER_P_H

#include <IrcGlobal>
#include <QtCore/qbytearray.h>
#include <QtCore/qtextcodec.h>

class IrcCodecPlugin;

class COMMUNI_EXPORT IrcMessageDecoder
{
public:
    IrcMessageDecoder();
    ~IrcMessageDecoder();

    QByteArray encoding() const;
    void setEncoding(const QByteArray& encoding);

    QString decode(const QByteArray& data) const;

private:
    QByteArray initialize();
    bool loadPlugins();

    struct Data {
        QTextCodec* fallback;
    } d;
};

#endif // IRCMESSAGEDECODER_P_H
