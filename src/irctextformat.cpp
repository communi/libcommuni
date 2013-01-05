/*
* Copyright (C) 2008-2013 J-P Nurmi <jpnurmi@gmail.com>
* Copyright (C) 2010-2011 SmokeX <smokexjc@gmail.com>
* Copyright (C) 2012 Mark Johnson <marknotgeorge@googlemail.com>
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

/*
  Parts of this code come from Konversation and are copyrighted to:
  Copyright (C) 2002 Dario Abatianni <eisfuchs@tigress.com>
  Copyright (C) 2004 Peter Simonsson <psn@linux.se>
  Copyright (C) 2006-2008 Eike Hein <hein@kde.org>
  Copyright (C) 2004-2009 Eli Mackenzie <argonel@gmail.com>
*/

#include "irctextformat.h"
#include <QStringList>

static QRegExp URL_PATTERN(QLatin1String("((www\\.(?!\\.)|(ssh|fish|irc|amarok|(f|sf|ht)tp(|s))://)(\\.?[\\d\\w/,\\':~\\^\\?=;#@\\-\\+\\%\\*\\{\\}\\!\\(\\)\\[\\]]|&)+)|""([-.\\d\\w]+@[-.\\d\\w]{2,}\\.[\\w]{2,})"), Qt::CaseInsensitive);

/*!
    \file irctextformat.h
    \brief #include &lt;IrcTextFormat&gt;
 */

/*!
    \class IrcTextFormat irctextformat.h <IrcTextFormat>
    \ingroup utility
    \brief The IrcUtil class provides methods for text formatting.
 */

class IrcTextFormatPrivate : public QSharedData
{
public:
    QRegExp rx;
    IrcPalette palette;
};

/*!
    Constructs a new text format.
 */
IrcTextFormat::IrcTextFormat() : d(new IrcTextFormatPrivate)
{
    d->rx = URL_PATTERN;
}

/*!
    Constructs a copy of \a other text format.
 */
IrcTextFormat::IrcTextFormat(const IrcTextFormat& other) : d(other.d)
{
}

/*!
    Assigns an \a other text format to this.
 */
IrcTextFormat& IrcTextFormat::operator=(const IrcTextFormat& other)
{
    if (this != &other)
        d = other.d;
    return *this;
}

/*!
    Destructs the text format.
 */
IrcTextFormat::~IrcTextFormat()
{
}

/*!
    Returns the palette used for color formatting.
 */
IrcPalette IrcTextFormat::palette() const
{
    return d->palette;
}

/*!
    Sets the \a palette used for color formatting.
 */
void IrcTextFormat::setPalette(const IrcPalette& palette)
{
    d->palette = palette;
}

/*!
    Returns the regular expression used for matching URLs.
 */
QRegExp IrcTextFormat::urlRegExp() const
{
    return d->rx;
}

/*!
    Sets the regular expression used for matching URLs.
 */
void IrcTextFormat::setUrlRegExp(const QRegExp& rx)
{
    d->rx = rx;
}

static bool parseColors(const QString& message, int pos, int* len, int *fg, int *bg)
{
    // fg(,bg)
    *len = 0;
    *fg = -1;
    *bg = -1;
    QRegExp rx(QLatin1String("(\\d{1,2})(?:,(\\d{1,2}))?"));
    int idx = rx.indexIn(message, pos);
    if (idx == pos) {
        *len = rx.matchedLength();
        *fg = rx.cap(1).toInt();
        bool ok = false;
        int tmp = rx.cap(2).toInt(&ok);
        if (ok)
            *bg = tmp;
    }
    return *len > 0;
}

/*!
    Converts \a message to HTML using. This function parses the message
    and replaces IRC-style formatting like colors, bold and underline to
    the corresponding HTML formatting. Furthermore, this function detects
    URLs and replaces them with appropriate HTML hyperlinks.

    \sa palette, urlRegExp
*/
QString IrcTextFormat::messageToHtml(const QString& message) const
{
    QString processed = message;
    processed.replace(QLatin1Char('&'), QLatin1String("&amp;"));
    processed.replace(QLatin1Char('<'), QLatin1String("&lt;"));
    processed.replace(QLatin1Char('>'), QLatin1String("&gt;"));
    processed.replace(QLatin1Char('"'), QLatin1String("&quot;"));
    processed.replace(QLatin1Char('\''), QLatin1String("&apos;"));
    processed.replace(QLatin1Char('\t'), QLatin1String("&nbsp;"));

    enum {
        None            = 0x0,
        Bold            = 0x1,
        Italic          = 0x4,
        StrikeThrough   = 0x8,
        Underline       = 0x10,
        Inverse         = 0x20
    };
    int state = None;

    int pos = 0;
    int len = 0;
    int fg = -1;
    int bg = -1;
    int depth = 0;
    while (pos < processed.size()) {
        QString replacement;
        switch (processed.at(pos).unicode()) {
            case '\x02': // bold
                if (state & Bold) {
                    depth--;
                    replacement = QLatin1String("</span>");
                } else {
                    depth++;
                    replacement = QLatin1String("<span style='font-weight: bold'>");
                }
                state ^= Bold;
                break;

            case '\x03': // color
                if (parseColors(processed, pos + 1, &len, &fg, &bg)) {
                    depth++;
                    QStringList styles;
                    styles += QString(QLatin1String("color:%1")).arg(d->palette.colorName(fg, QLatin1String("black")));
                    if (bg != -1)
                        styles += QString(QLatin1String("background-color:%1")).arg(d->palette.colorName(bg, QLatin1String("transparent")));
                    replacement = QString(QLatin1String("<span style='%1'>")).arg(styles.join(QLatin1String(";")));
                    // \x03FF(,BB)
                    processed.replace(pos, len + 1, replacement);
                    pos += replacement.length();
                    continue;
                } else {
                    depth--;
                    replacement = QLatin1String("</span>");
                }
                break;

                //case '\x09': // italic
            case '\x1d': // italic
                if (state & Italic) {
                    depth--;
                    replacement = QLatin1String("</span>");
                } else {
                    depth++;
                    replacement = QLatin1String("<span style='font-style: italic'>");
                }
                state ^= Italic;
                break;

            case '\x13': // strike-through
                if (state & StrikeThrough) {
                    depth--;
                    replacement = QLatin1String("</span>");
                } else {
                    depth++;
                    replacement = QLatin1String("<span style='text-decoration: line-through'>");
                }
                state ^= StrikeThrough;
                break;

            case '\x15': // underline
            case '\x1f': // underline
                if (state & Underline) {
                    depth--;
                    replacement = QLatin1String("</span>");
                } else {
                    depth++;
                    replacement = QLatin1String("<span style='text-decoration: underline'>");
                }
                state ^= Underline;
                break;

            case '\x16': // inverse
                if (state & Inverse) {
                    depth--;
                    replacement = QLatin1String("</span>");
                } else {
                    depth++;
                    replacement = QLatin1String("<span style='font-weight: bold'>");
                }
                state ^= Inverse;
                break;

            case '\x0f': // none
                if (depth > 0)
                    replacement = QString(QLatin1String("</span>")).repeated(depth);
                else
                    processed.remove(pos--, 1); // must rewind back for ++pos below...
                state = None;
                depth = 0;
                break;

            default:
                break;
        }

        if (!replacement.isEmpty()) {
            processed.replace(pos, 1, replacement);
            pos += replacement.length();
        } else {
            ++pos;
        }
    }

    pos = 0;
    while ((pos = d->rx.indexIn(processed, pos)) >= 0) {
        int len = d->rx.matchedLength();
        QString href = processed.mid(pos, len);

        // Don't consider trailing &gt; as part of the link.
        QString append;
        if (href.endsWith(QLatin1String("&gt;"))) {
            append.append(href.right(4));
            href.chop(4);
        }

        // Don't consider trailing comma or semi-colon as part of the link.
        if (href.endsWith(QLatin1Char(',')) || href.endsWith(QLatin1Char(';'))) {
            append.append(href.right(1));
            href.chop(1);
        }

        // Don't consider trailing closing parenthesis part of the link when
        // there's an opening parenthesis preceding in the beginning of the
        // URL or there is no opening parenthesis in the URL at all.
        if (pos > 0 && href.endsWith(QLatin1Char(')'))
                && (processed.at(pos - 1) == QLatin1Char('(')
                    || !href.contains(QLatin1Char('(')))) {
            append.prepend(href.right(1));
            href.chop(1);
        }

        // Qt doesn't support (?<=pattern) so we do it here
        if (pos > 0 && processed.at(pos - 1).isLetterOrNumber()) {
            pos++;
            continue;
        }

        QString protocol;
        if (d->rx.cap(1).startsWith(QLatin1String("www."), Qt::CaseInsensitive))
            protocol = QLatin1String("http://");
        else if (d->rx.cap(1).isEmpty())
            protocol = QLatin1String("mailto:");

        QString source = href;
        source.replace(QLatin1String("&amp;"), QLatin1String("&"));

        QString link = QString(QLatin1String("<a href='%1%2'>%3</a>")).arg(protocol, source, href) + append;
        processed.replace(pos, len, link);
        pos += link.length();
    }

    return processed;
}
