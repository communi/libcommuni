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

static bool isUtf8(const QByteArray& utf8);

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
    QTextCodec *codec = QTextCodec::codecForName(enc);
    if (!codec)
        codec = QTextCodec::codecForLocale();
    return codec->toUnicode(data);
}

QByteArray IrcEncoder::detectEncoding(const QByteArray& data) const
{
    QByteArray encoding("ISO 8859-1");
    if (isUtf8(data))
        return "UTF-8";
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

/*
  The Original Code is mozilla.org code.
  See http://lxr.mozilla.org/mozilla/source/modules/rdf/src/utils.c

  Copyright (C) 1998 Netscape Communications Corporation
*/

#define kLeft1BitMask  0x80
#define kLeft2BitsMask 0xC0
#define kLeft3BitsMask 0xE0
#define kLeft4BitsMask 0xF0
#define kLeft5BitsMask 0xF8
#define kLeft6BitsMask 0xFC
#define kLeft7BitsMask 0xFE

#define k2BytesLeadByte kLeft2BitsMask
#define k3BytesLeadByte kLeft3BitsMask
#define k4BytesLeadByte kLeft4BitsMask
#define k5BytesLeadByte kLeft5BitsMask
#define k6BytesLeadByte kLeft6BitsMask
#define kTrialByte      kLeft1BitMask

#define UTF8_1Byte(c) ( 0 == ((c) & kLeft1BitMask))
#define UTF8_2Bytes(c) ( k2BytesLeadByte == ((c) & kLeft3BitsMask))
#define UTF8_3Bytes(c) ( k3BytesLeadByte == ((c) & kLeft4BitsMask))
#define UTF8_4Bytes(c) ( k4BytesLeadByte == ((c) & kLeft5BitsMask))
#define UTF8_5Bytes(c) ( k5BytesLeadByte == ((c) & kLeft6BitsMask))
#define UTF8_6Bytes(c) ( k6BytesLeadByte == ((c) & kLeft7BitsMask))
#define UTF8_ValidTrialByte(c) ( kTrialByte == ((c) & kLeft2BitsMask))

bool isUtf8(const QByteArray& utf8)
{
    int clen = 0;
    for (int i = 0; i < utf8.length(); i += clen)
    {
        if (UTF8_1Byte(utf8[i]))
        {
            clen = 1;
        }
        else if (UTF8_2Bytes(utf8[i]))
        {
            clen = 2;
            // No enough trail bytes
            if ((i + clen) > utf8.length())
                return false;
            // 0000 0000 - 0000 007F : should encode in less bytes
            if (0 ==  (utf8[i] & 0x1E))
                return false;
        }
        else if (UTF8_3Bytes(utf8[i]))
        {
            clen = 3;
            // No enough trail bytes
            if ((i + clen) > utf8.length())
                return false;
            // a single Surrogate should not show in 3 bytes UTF8, instead,
            // the pair should be intepreted as one single UCS4 char and
            // encoded UTF8 in 4 bytes
            if ((0xED == utf8[i]) && (0xA0 == (utf8[i+1] & 0xA0)))
                return false;
            // 0000 0000 - 0000 07FF : should encode in less bytes
            if ((0 == (utf8[i] & 0x0F)) && (0 == (utf8[i+1] & 0x20)))
                return false;
        }
        else if (UTF8_4Bytes(utf8[i]))
        {
            clen = 4;
            // No enough trail bytes
            if ((i + clen) > utf8.length())
                return false;
            // 0000 0000 - 0000 FFFF : should encode in less bytes
            if ((0 == (utf8[i] & 0x07 )) && (0 == (utf8[i+1] & 0x30)))
                return false;
        }
        else if (UTF8_5Bytes(utf8[i]))
        {
            clen = 5;
            // No enough trail bytes
            if ((i + clen) > utf8.length())
                return false;
            // 0000 0000 - 001F FFFF : should encode in less bytes
            if ((0 == (utf8[i] & 0x03 )) && (0 == (utf8[i+1] & 0x38)))
                return false;
        }
        else if (UTF8_6Bytes(utf8[i]))
        {
            clen = 6;
            // No enough trail bytes
            if ((i + clen) > utf8.length())
                return false;
            // 0000 0000 - 03FF FFFF : should encode in less bytes
            if ((0 == (utf8[i] & 0x01)) && (0 == (utf8[i+1] & 0x3E)))
                return false;
        }
        else
        {
            return false;
        }
        for (int j = 1; j < clen; ++j)
        {
            if (!UTF8_ValidTrialByte(utf8[i+j])) // Trail bytes invalid
                return false;
        }
    }
    return true;
}
