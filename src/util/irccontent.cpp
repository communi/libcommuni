/*
  Copyright (C) 2008-2014 The Communi Project

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

#include "irccontent.h"

IRC_BEGIN_NAMESPACE

/*!
    \file irccontent.h
    \brief \#include &lt;IrcContent&gt;
 */

/*!
    \since 3.2
    \class IrcContent irccontent.h <IrcContent>
    \ingroup util
    \brief Provides IRC content in plain text, HTML and the detected URLs.

    \sa IrcTextFormat::parse()
 */

class IrcContentPrivate
{
public:
    QString text;
    QString html;
    QList<QUrl> urls;
};

/*!
    \internal
 */
IrcContent::IrcContent(QObject* parent) : QObject(parent), d_ptr(new IrcContentPrivate)
{
}

/*!
    \internal
 */
IrcContent::~IrcContent()
{
}

/*!
    This property holds the plain text.

    \par Access functions:
    \li QString <b>text</b>() const
    \li void <b>setText</b>(const QString& text)
 */
QString IrcContent::text() const
{
    Q_D(const IrcContent);
    return d->text;
}

void IrcContent::setText(const QString& text)
{
    Q_D(IrcContent);
    d->text = text;
}

/*!
    This property holds the HTML content.

    \par Access functions:
    \li QString <b>html</b>() const
    \li void <b>setHtml</b>(const QString& html)
 */
QString IrcContent::html() const
{
    Q_D(const IrcContent);
    return d->html;
}

void IrcContent::setHtml(const QString& html)
{
    Q_D(IrcContent);
    d->html = html;
}

/*!
    This property holds the list of URLs.

    \par Access functions:
    \li QList<QUrl> <b>urls</b>() const
    \li void <b>setUrls</b>(const QList<QUrl>& urls)
 */
QList<QUrl> IrcContent::urls() const
{
    Q_D(const IrcContent);
    return d->urls;
}

void IrcContent::setUrls(const QList<QUrl>& urls)
{
    Q_D(IrcContent);
    d->urls = urls;
}

#include "moc_irccontent.cpp"

IRC_END_NAMESPACE
