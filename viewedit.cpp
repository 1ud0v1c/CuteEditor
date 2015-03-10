#include "viewedit.h"

ViewEdit::ViewEdit(QTextEdit *parent) : QTextEdit(parent) {
    setReadOnly(true);
}

void ViewEdit::update(QString string) {
    setHtml(string);
}
