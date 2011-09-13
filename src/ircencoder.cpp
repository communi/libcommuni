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

#include "ircencoder_p.h"
#include <QTextCodec>

#ifdef HAVE_ICU
#include <unicode/ucsdet.h>
#endif // HAVE_ICU

IrcEncoder::IrcEncoder()
{
    d.detector = 0;
#ifdef HAVE_ICU
    UErrorCode status = U_ZERO_ERROR;
    d.detector = ucsdet_open(&status);
    if (U_FAILURE(status))
        qWarning("IrcEncoder: ICU initialization failed: %s", u_errorName(status));
#endif // HAVE_ICU
}

IrcEncoder::~IrcEncoder()
{
#ifdef HAVE_ICU
    ucsdet_close(d.detector);
#endif // HAVE_ICU
}

QString IrcEncoder::encode(const QByteArray& data) const
{
    QByteArray enc = d.encoding;
    if (enc.isNull())
        enc = detectEncoding(data);
    QTextCodec *codec = QTextCodec::codecForName(d.encoding);
    if (!codec)
        codec = QTextCodec::codecForLocale();
    return codec->toUnicode(data);
}

QByteArray IrcEncoder::detectEncoding(const QByteArray& data) const
{
    Q_UNUSED(data);
    QByteArray encoding;
#ifdef HAVE_ICU
    UErrorCode status = U_ZERO_ERROR;
    if (d.detector)
    {
        ucsdet_setText(d.detector, data.constData(), data.length(), &status);
        if (!U_FAILURE(status))
        {
            const UCharsetMatch* match = ucsdet_detect(d.detector, &status);
            if (match && !U_FAILURE(status))
                encoding = ucsdet_getName(match, &status);
        }
    }
    if (U_FAILURE(status))
        qWarning("IrcEncoder::detectEncoding() failed: %s", u_errorName(status));
#endif // HAVE_ICU
    return encoding;
}
