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

#include "irc.h"

/*!
    \namespace Irc irc.h
    \ingroup LibIrcClient-Qt
    \brief Irc provides IRC related utility functions.

 */

/*!
    Returns the libircclient version number as string
    in form M.N.P (M = major, N = minor, P = patch).
 */
const char* Irc::version()
{
    return IRC_VERSION_STR;
}
