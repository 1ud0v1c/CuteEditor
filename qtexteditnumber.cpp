#include "qtexteditnumber.h"
#include "linenumber.h"
#include <QScrollBar>
#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>
#include <QRect>
#include <QRectF>
#include <QAbstractTextDocumentLayout>
#include <QList>
#include <QTextCursor>
#include <QMimeData>
#include <QDebug>

QTextEditNumber::QTextEditNumber(QWidget *parent) : QTextEdit(parent) {
    lineNumberArea = new LineNumber(this);

    connect(document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumberArea/*_2*/(int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumberArea()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    createSnippets();
}

void QTextEditNumber::createSnippets() {
    QString html5 = "<!doctype html>\n<html lang=\"fr\">\n\t<head>\n\t\t <meta charset=\"utf-8\">\n\t\t <title>Titre</title>\n\t\t </head>\n\t<body>\n\t</body>\n</html>";
    QString input = "<input type=\"text\" name=\"\" />";
    QString strong = "<strong></strong>";
    QString ul = "<ul>\n\t<li></li>\n</ul>";
    QString img = "<img src=\"\" alt=\"\" />";
    QString link = "<a href=\"\"></a>";
    QString italic = "<em></em>";
    QString css = "<link rel=\"stylesheet\" type=\"text/css\" href=\"css/main.css\" />";
    QString js = "<script type=\"text/javascript\" src=\"js/main.js\"></script>";
    QString textarea = "<textarea name=\"\" cols=\"30\" rows=\"10\"></textarea>";
    QString select = "<select>\n\t<option value=\"\"></option>\n</select>";
    QString form = "<form action=\"#\" method=\"post\"></form>";

    _snippets["html"] = html5;
    _snippets["input"] = input;
    _snippets["strong"] = strong;
    _snippets["ul"] = ul;
    _snippets["img"] = img;
    _snippets["a"] = link;
    _snippets["em"] = italic;
    _snippets["css"] = css;
    _snippets["js"] = js;
    _snippets["texta"] = textarea;
    _snippets["select"] = select;
    _snippets["form"] = form;
}

void QTextEditNumber::keyPressEvent(QKeyEvent *e) {
    bool tabPress = false;
    switch (e->key()) {
       case Qt::Key_Tab:
            tabPress = true;
            break;
       default:
           QTextEdit::keyPressEvent(e);
           break;
    }

    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    QString word = tc.selectedText();

    if(tabPress) {
        if(_snippets.find(word.toStdString()) != _snippets.end()) {
            tc.removeSelectedText();
            tc.insertText(_snippets.find(word.toStdString())->second);
        } else {
            QTextEdit::keyPressEvent(e);
        }
    }
}


void QTextEditNumber::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::darkGray).lighter(160);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


int QTextEditNumber::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 12 +  fontMetrics().width(QLatin1Char('9')) * (digits);
    return space;
}

void QTextEditNumber::updateLineNumberAreaWidth(int) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void QTextEditNumber::updateLineNumberArea(QRectF) {
    QTextEditNumber::updateLineNumberArea();
}

void QTextEditNumber::updateLineNumberArea(int) {
    QTextEditNumber::updateLineNumberArea();
}

void QTextEditNumber::updateLineNumberArea() {
    verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition());

    QRect rect =  contentsRect();
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    updateLineNumberAreaWidth(0);

    int dy = verticalScrollBar()->sliderPosition();
    if (dy > -1) {
        lineNumberArea->scroll(0, dy);
    }

    int first_block_id = getFirstVisibleBlockId();
    if (first_block_id == 0 || textCursor().block().blockNumber() == first_block_id-1) {
        verticalScrollBar()->setSliderPosition(dy-document()->documentMargin());
    }
    highlightCurrentLine();
}


void QTextEditNumber::resizeEvent(QResizeEvent *e) {
    QTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


int QTextEditNumber::getFirstVisibleBlockId() {
    QTextCursor curs = QTextCursor(document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < document()->blockCount(); ++i) {
        QTextBlock block = curs.block();
        QRect r1 = viewport()->geometry();
        QRect r2 = document()->documentLayout()->blockBoundingRect(block).translated(viewport()->geometry().x(), viewport()->geometry().y() - (verticalScrollBar()->sliderPosition())).toRect();

        if(r1.contains(r2, true)) {
            return i;
        }
        curs.movePosition(QTextCursor::NextBlock);
    }
    return 0;
}


void QTextEditNumber::lineNumberAreaPaintEvent(QPaintEvent *event) {
    verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::darkGray);
    int blockNumber = getFirstVisibleBlockId();

    QTextBlock block = document()->findBlockByNumber(blockNumber);
    int top = viewport()->geometry().top();

    int additional_margin;
    if (blockNumber == 0) {
        additional_margin = (int) document()->documentMargin() -1 - verticalScrollBar()->sliderPosition();
    }
    top += additional_margin;

    int bottom = top + (int) document()->documentLayout()->blockBoundingRect(block).height();

    QColor blue(0, 191, 255);
    QColor white(225, 225, 255);

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen((this->textCursor().blockNumber() == blockNumber) ? blue : white);
            painter.drawText(-5, top,lineNumberArea->width(), fontMetrics().height(),Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) document()->documentLayout()->blockBoundingRect(block).height();
        ++blockNumber;
    }

}
