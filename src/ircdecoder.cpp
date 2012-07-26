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

#include "ircdecoder_p.h"
#include <QTextCodec>
#include "uchardet.h"

IrcDecoder::IrcDecoder()
{
    d.fallback = "UTF-8";
    d.detector = uchardet_new();
}

IrcDecoder::~IrcDecoder()
{
    uchardet_delete(d.detector);
}

QString IrcDecoder::decode(const QByteArray& data) const
{
    QByteArray enc = detectEncoding(data);
    if (enc.isEmpty())
        enc = d.fallback;
    QTextCodec *codec = QTextCodec::codecForName(enc);
    Q_ASSERT(codec);
    return codec->toUnicode(data);
}

QByteArray IrcDecoder::detectEncoding(const QByteArray& data) const
{
    uchardet_reset(d.detector);
    uchardet_handle_data(d.detector, data.constData(), data.length());
    uchardet_data_end(d.detector);
    return uchardet_get_charset(d.detector);
}
