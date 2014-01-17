/*
* Copyright (C) 2008-2014 The Communi Project
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
#include "uchardet.h"

IRC_BEGIN_NAMESPACE

#ifndef IRC_DOXYGEN
#define UCD(x) reinterpret_cast<uchardet_t>(x)

void IrcMessageDecoder::initialize()
{
    d.detector = uchardet_new();
}

void IrcMessageDecoder::uninitialize()
{
    uchardet_delete(UCD(d.detector));
}

QByteArray IrcMessageDecoder::codecForData(const QByteArray &data) const
{
    uchardet_reset(UCD(d.detector));
    uchardet_handle_data(UCD(d.detector), data.constData(), data.length());
    uchardet_data_end(UCD(d.detector));
    return uchardet_get_charset(UCD(d.detector));
}
#endif // IRC_DOXYGEN

IRC_END_NAMESPACE
