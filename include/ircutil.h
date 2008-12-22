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

#ifndef IRC_UTIL_H
#define IRC_UTIL_H

#include <irc.h>

namespace Irc
{
    class LIBIRCCLIENT_QT_EXPORT Util
    {
    public:
        static QString nickFromTarget(const QString& target);
        static QString hostFromTarget(const QString& target);

        //static QString colorStripFromMirc(const QString& message);
        //static QString colorConvertFromMirc(const QString& message);
        //static QString colorConvertToMirc(const QString& message);
        //static QString colorConvertToHtml(const QString& message);
    };
}

#endif // IRC_UTIL_H
