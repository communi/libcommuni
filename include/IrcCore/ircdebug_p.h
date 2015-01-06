/*
  Copyright (C) 2008-2015 The Communi Project

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef IRCDEBUG_P_H
#define IRCDEBUG_P_H

#include <IrcGlobal>
#include <IrcConnection>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>

IRC_BEGIN_NAMESPACE

#ifndef IRC_DOXYGEN
static bool irc_debug_enabled()
{
    static bool enabled = qgetenv("IRC_DEBUG").toInt();
    return enabled;
}

class IrcDebug
{
public:
    IrcDebug(IrcConnection* c) : enabled(irc_debug_enabled())
#ifndef QT_NO_DEBUG_STREAM
      , debug(&str)
#endif // QT_NO_DEBUG_STREAM
    {
#ifndef QT_NO_DEBUG_STREAM
        if (enabled) {
            const QString desc = c->displayName();
            const QString stamp = QDateTime::currentDateTime().toString(Qt::ISODate);
            debug << qPrintable("[" + stamp + " " + desc + "]");
        }
#endif // QT_NO_DEBUG_STREAM
    }

    ~IrcDebug() {
#ifndef QT_NO_DEBUG_STREAM
        if (enabled)
            qDebug() << qPrintable(str);
#endif // QT_NO_DEBUG_STREAM
    }

    template<typename T>
    inline IrcDebug &operator<<(const T& t)
    {
#ifdef QT_NO_DEBUG_STREAM
        Q_UNUSED(t);
#else
        if (enabled) {
            debug << t;
        }
#endif // QT_NO_DEBUG_STREAM
        return *this;
    }

private:
    bool enabled;
    QString str;
#ifndef QT_NO_DEBUG_STREAM
    QDebug debug;
#endif // QT_NO_DEBUG_STREAM
};

#define ircDebug(Connection) IrcDebug(Connection)
#endif // IRC_DOXYGEN

IRC_END_NAMESPACE

#endif // IRCDEBUG_P_H
