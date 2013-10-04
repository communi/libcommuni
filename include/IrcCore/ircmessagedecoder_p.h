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

#ifndef IRCMESSAGEDECODER_P_H
#define IRCMESSAGEDECODER_P_H

#include <IrcGlobal>
#include <QtCore/qbytearray.h>
#include <QtCore/qtextcodec.h>

IRC_BEGIN_NAMESPACE

class IrcMessageDecoder
{
public:
    IrcMessageDecoder();
    ~IrcMessageDecoder();

    QString decode(const QByteArray& data, const QByteArray& encoding) const;

private:
    void initialize();
    void uninitialize();
    QByteArray codecForData(const QByteArray& data) const;

    struct Data {
        void* detector;
    } d;
};

IRC_END_NAMESPACE

#endif // IRCMESSAGEDECODER_P_H
