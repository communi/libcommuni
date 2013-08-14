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

#include "ircmessagedecoder_p.h"
#include <IrcGlobal>
#include <QSet>

extern "C" {
    int IsUTF8Text(const char* utf8, int len);
}

IRC_BEGIN_NAMESPACE

IRC_CORE_EXPORT bool irc_is_supported_encoding(const QByteArray& encoding)
{
    static QSet<QByteArray> codecs = QTextCodec::availableCodecs().toSet();
    return codecs.contains(encoding);
}

IrcMessageDecoder::IrcMessageDecoder()
{
    initialize();
    d.fallback = QTextCodec::codecForName("ISO-8859-15");
}

IrcMessageDecoder::~IrcMessageDecoder()
{
    uninitialize();
}

QByteArray IrcMessageDecoder::encoding() const
{
    return d.fallback->name();
}

void IrcMessageDecoder::setEncoding(const QByteArray& encoding)
{
    d.fallback = QTextCodec::codecForName(encoding);
}

QString IrcMessageDecoder::decode(const QByteArray& data) const
{
    QTextCodec* codec = 0;
    if (IsUTF8Text(data, data.length())) {
        codec = QTextCodec::codecForName("UTF-8");
    } else {
        QByteArray name = codecForData(data);
        codec = QTextCodec::codecForName(name);
    }

    if (!codec)
        codec = d.fallback;
    Q_ASSERT(codec);
    return codec->toUnicode(data);
}

IRC_END_NAMESPACE
