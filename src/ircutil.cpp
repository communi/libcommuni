/*
* Copyright (C) 2008 J-P Nurmi jpnurmi@gmail.com
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
*
* $Id$
*/

#include "ircutil.h"
#include <QRegExp>

static const int BUFLEN = 256;

static void irc_target_get_nick (const char * target, char *nick, size_t size)
{
	const char *p = strstr (target, "!");
	unsigned int len;

	if ( p )
		len = p - target;
	else
		len = strlen (target);

	if ( len > size-1 )
		len = size - 1;

	memcpy (nick, target, len);
	nick[len] = '\0';
}

static void irc_target_get_host (const char * target, char *host, size_t size)
{
	unsigned int len;
	const char *p = strstr (target, "!");

	if ( !p )
		p = target;

	len = strlen (p);

	if ( len > size-1 )
		len = size - 1;

	memcpy (host, p, len);
	host[len] = '\0';
}

namespace Irc
{
    /*!
        Parses and returns the nick part from target.
    */
    QString Irc::Util::nickFromTarget(const QString& target)
    {
        char buffer[BUFLEN-1];
        irc_target_get_nick(target.toUtf8(), buffer, BUFLEN);
        return QString::fromUtf8(buffer);
    }

    /*!
        Parses and returns the host part from target.
    */
    QString Irc::Util::hostFromTarget(const QString& target)
    {
        char buffer[BUFLEN-1];
        irc_target_get_host(target.toUtf8(), buffer, BUFLEN);
        return QString::fromUtf8(buffer);
    }

    /*
    static QString irc_colorStringHelper(char* message)
    {
        QString value = QString::fromUtf8(message);
        free(message);
        return value;
    }

    QString Irc::Util::colorConvertToHtml(const QString& message)
    {
        QString tmp = colorConvertToMirc(message);
        tmp.replace(QRegExp("\\[([^\\]]+)\\]([^\\[]*)\\(/[^\\]]+)\\]"), "<\\1>\\2<\\3>");
        return tmp;
    }
    */
}
