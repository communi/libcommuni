/*
* Copyright (C) 2008-2009 J-P Nurmi jpnurmi@gmail.com
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

/*!
    \class Irc::Util ircutil.h
    \brief The Irc::Util class provides IRC related utility functions.
 */

namespace Irc
{
    /*!
        Parses and returns the nick part from \a target.
    */
    QString Irc::Util::nickFromTarget(const QString& target)
    {
        int index = target.indexOf(QLatin1Char('!'));
        return target.left(index);
    }

    /*!
        Parses and returns the host part from \a target.
    */
    QString Irc::Util::hostFromTarget(const QString& target)
    {
        int index = target.indexOf(QLatin1Char('!'));
        return target.mid(index + 1);
    }
}
