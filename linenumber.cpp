#include "linenumber.h"
#include "qtexteditnumber.h"

LineNumber::LineNumber(QTextEdit *editor) : QWidget(editor) {
    codeEditor = editor;
}

QSize LineNumber::sizeHint() const {
    return QSize(((QTextEditNumber *)codeEditor)->lineNumberAreaWidth(), 0);
}

void LineNumber::paintEvent(QPaintEvent *event) {
    ((QTextEditNumber *)codeEditor)->lineNumberAreaPaintEvent(event);
}
