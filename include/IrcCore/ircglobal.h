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

#ifndef IRCGLOBAL_H
#define IRCGLOBAL_H

#include <QtCore/qglobal.h>

/*!
    \file ircglobal.h
    \brief #include &lt;IrcGlobal&gt;
 */

#if defined(COMMUNI_SHARED)
#
#  if defined(BUILD_IRC_CORE)
#    define IRC_CORE_EXPORT Q_DECL_EXPORT
#  else
#    define IRC_CORE_EXPORT Q_DECL_IMPORT
#  endif
#
#  if defined(BUILD_IRC_MODEL)
#    define IRC_MODEL_EXPORT Q_DECL_EXPORT
#  else
#    define IRC_MODEL_EXPORT Q_DECL_IMPORT
#  endif
#
#  if defined(BUILD_IRC_UTIL)
#    define IRC_UTIL_EXPORT Q_DECL_EXPORT
#  else
#    define IRC_UTIL_EXPORT Q_DECL_IMPORT
#  endif
#
#elif defined(COMMUNI_STATIC) || defined(BUILD_IRC_CORE) || defined(BUILD_IRC_MODEL) || defined(BUILD_IRC_UTIL)
#
#    define IRC_CORE_EXPORT
#    define IRC_MODEL_EXPORT
#    define IRC_UTIL_EXPORT
#
#else
#  error Installation problem: either COMMUNI_SHARED or COMMUNI_STATIC must be defined!
#endif

/*!
    \def IRC_VERSION

    This macro expands a numeric value of the form 0xMMNNPP (MM = major, NN = minor, PP = patch) that specifies Communi's version number.
    For example, if you compile your application against Communi 1.2.3, the IRC_VERSION macro will expand to 0x010203.

    You can use IRC_VERSION to use the latest Communi features where available. For example:
    \code
 #if IRC_VERSION >= 0x000300
     // SSL support since version 0.3.0
     connection->setSocket(new QSslSocket(connection));
 #endif
    \endcode

    \sa IRC_VERSION_STR and Irc::version().
 */
#define IRC_VERSION 0x030000

/*!
    \def IRC_VERSION_STR

    This macro expands to a string that specifies Communi's version number (for example, "1.2.3").
    This is the version against which the application is compiled.

    \sa Irc::version() and IRC_VERSION.
 */
#define IRC_VERSION_STR "3.0.0"

#endif // IRCGLOBAL_H
