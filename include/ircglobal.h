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

#ifndef IRC_GLOBAL_H
#define IRC_GLOBAL_H

#include <QtGlobal>

/*!
    \file ircglobal.h
 */

#if BUILD_IRC
#    define IRC_EXPORT Q_DECL_EXPORT
#else
#    define IRC_EXPORT Q_DECL_IMPORT
#endif // BUILD_IRC

/*!
    \def IRC_VERSION

    This macro expands a numeric value of the form 0xMMNNPP (MM = major, NN = minor, PP = patch) that specifies LibIrcClient-Qt's version number.
    For example, if you compile your application against LibIrcClient-Qt 1.2.3, the IRC_VERSION macro will expand to 0x010203.

    You can use IRC_VERSION to use the latest LibIrcClient-Qt features where available. For example:
    \code
#if IRC_VERSION >= 0x000200
    session->setNick(nick);
    session->setIdent(ident);
    session->setRealName(realName);
    session->setPassword(password);
    session->connectToServer(host, port);
#else
    session->connectToServer(host, port, nick, ident, realName, password);
#endif
    \endcode
    
    \sa IRC_VERSION_STR and Irc::version().
 */
#define IRC_VERSION 0x000200

/*!
    \def IRC_VERSION_STR

    This macro expands to a string that specifies LibIrcClient-Qt's version number (for example, "1.2.3").
    This is the version against which the application is compiled.

    \sa Irc::version() and IRC_VERSION.
 */
#define IRC_VERSION_STR "0.2.0"

#endif // IRC_GLOBAL_H
