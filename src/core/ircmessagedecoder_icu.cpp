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
#include <unicode/ucsdet.h>

IRC_BEGIN_NAMESPACE

#ifndef IRC_DOXYGEN
#define UCSD(x) reinterpret_cast<UCharsetDetector*>(x)

void IrcMessageDecoder::initialize()
{
    UErrorCode status = U_ZERO_ERROR;
    d.detector = ucsdet_open(&status);
    if (U_FAILURE(status))
        qWarning("IrcMessageDecoder: ICU initialization failed: %s", u_errorName(status));
}

void IrcMessageDecoder::uninitialize()
{
    ucsdet_close(UCSD(d.detector));
}

QByteArray IrcMessageDecoder::codecForData(const QByteArray &data) const
{
    QByteArray encoding;
    UErrorCode status = U_ZERO_ERROR;
    if (d.detector) {
        ucsdet_setText(UCSD(d.detector), data.constData(), data.length(), &status);
        if (!U_FAILURE(status)) {
            const UCharsetMatch* match = ucsdet_detect(UCSD(d.detector), &status);
            if (match && !U_FAILURE(status))
                encoding = ucsdet_getName(match, &status);
        }
    }
    if (U_FAILURE(status))
        qWarning("IrcMessageDecoder::codecForData() failed: %s", u_errorName(status));
    return encoding;
}
#endif // IRC_DOXYGEN

IRC_END_NAMESPACE
