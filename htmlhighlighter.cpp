#include "htmlhighlighter.h"
#include <QDebug>

HtmlHighlighter::HtmlHighlighter(QTextDocument *document) : QSyntaxHighlighter(document) {
    QColor yellow(233, 247, 158);
    QColor orange(222, 121, 51);
    QColor gray(73, 108, 132);
    QColor darkCyan(38, 162, 158);

    QTextCharFormat entityFormat;
    entityFormat.setForeground(QColor(0, 128, 0));
    entityFormat.setFontWeight(QFont::Bold);
    setFormatFor(Entity, entityFormat);

    QTextCharFormat tagFormat;
    tagFormat.setForeground(darkCyan);
    tagFormat.setFontWeight(QFont::Bold);
    setFormatFor(Tag, tagFormat);

    QTextCharFormat commentFormat;
    commentFormat.setForeground(gray);
    commentFormat.setFontItalic(true);
    setFormatFor(Comment, commentFormat);

    QTextCharFormat attributeFormat;
    attributeFormat.setForeground(orange);
    attributeFormat.setFontWeight(QFont::Bold);
    setFormatFor(Attribute, attributeFormat);

    QTextCharFormat valueFormat;
    valueFormat.setForeground(yellow);
    valueFormat.setFontWeight(QFont::Bold);
    setFormatFor(Value, valueFormat);
}

void HtmlHighlighter::setFormatFor(Construct construct, const QTextCharFormat &format) {
    m_formats[construct] = format;
    rehighlight();
}

void HtmlHighlighter::highlightBlock(const QString &text) {
    static const QLatin1Char tab = QLatin1Char('\t');
    static const QLatin1Char space = QLatin1Char(' ');
    static const QLatin1Char amp = QLatin1Char('&');
    static const QLatin1Char startTag = QLatin1Char('<');
    static const QLatin1Char endTag = QLatin1Char('>');
    static const QLatin1Char quot = QLatin1Char('"');
    static const QLatin1Char apos = QLatin1Char('\'');
    static const QLatin1Char semicolon = QLatin1Char(';');
    static const QLatin1Char equals = QLatin1Char('=');
    static const QLatin1String startComment("<!--");
    static const QLatin1String endComment("-->");
    static const QLatin1String endElement("/>");

    int state = previousBlockState();
    int len = text.length();
    int start = 0;
    int pos = 0;

    while (pos < len) {
        switch (state) {
            case NormalState:
            default:
                while (pos < len) {
                    QChar ch = text.at(pos);
                    if (ch == startTag) {
                        if (text.mid(pos, 4) == startComment) {
                            state = InComment;
                        } else {
                            state = InTag;
                            start = pos;
                            while (pos < len && text.at(pos) != space && text.at(pos) != endTag && text.at(pos) != tab && text.mid(pos, 2) != endElement)
                                ++pos;
                            if (text.mid(pos, 2) == endElement)
                                ++pos;
                            setFormat(start, pos - start,m_formats[Tag]);
                            break;
                        }
                        break;
                    } else if (ch == amp) {
                        start = pos;
                        while (pos < len && text.at(pos++) != semicolon);
                        setFormat(start, pos - start, m_formats[Entity]);
                    } else {
                        ++pos;
                    }
                }
                break;
            case InComment:
                start = pos;
                while (pos < len) {
                    if (text.mid(pos, 3) == endComment) {
                        pos += 3;
                        state = NormalState;
                        break;
                    } else {
                        ++pos;
                    }
                }
                setFormat(start, pos - start, m_formats[Comment]);
                break;
            case InTag:
                QChar quote = QChar::Null;
                while (pos < len) {
                    QChar ch = text.at(pos);
                    if (quote.isNull()) {
                        start = pos;
                        if (ch == apos || ch == quot) {
                            quote = ch;
                        } else if (ch == endTag) {
                            ++pos;
                            setFormat(start, pos - start, m_formats[Tag]);
                            state = NormalState;
                            break;
                        } else if (text.mid(pos, 2) == endElement) {
                            pos += 2;
                            setFormat(start, pos - start, m_formats[Tag]);
                            state = NormalState;
                            break;
                        } else if (ch != space && text.at(pos) != tab) {
                            ++pos;
                            while (pos < len && text.at(pos) != space && text.at(pos) != tab && text.at(pos) != equals) {
                                ++pos;
                            }
                            setFormat(start, pos - start, m_formats[Attribute]);
                            start = pos;
                        }
                    } else if (ch == quote) {
                        quote = QChar::Null;
                        setFormat(start, (pos - start)+1, m_formats[Value]);
                    }
                    ++pos;
                }
                break;
        }
    }
    setCurrentBlockState(state);
}
