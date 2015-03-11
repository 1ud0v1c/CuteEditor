#include "viewedit.h"
#include <QMouseEvent>
#include <QDebug>
#include <QPoint>
#include <QDesktopServices>
#include <QUrl>
#include <QRect>

ViewEdit::ViewEdit(QTextEdit *parent) : QTextEdit(parent) {
    setReadOnly(true);
    setMouseTracking(true);
}

void ViewEdit::update(QString string) {
    setHtml(string);
}

void ViewEdit::mousePressEvent(QMouseEvent *e) {
    QPoint p = e->pos();
    QString link = anchorAt(p);
    if (e->button()==Qt::LeftButton){
        if(link.size() > 1) {
            QDesktopServices::openUrl(QUrl(link,QUrl::TolerantMode));
        }
    }
}

void ViewEdit::mouseMoveEvent(QMouseEvent *e) {
    QCursor cursor;
    if (this->rect().contains(e->pos())) {
        QPoint p = e->pos();
        QString link = anchorAt(p);

        if(link.size() > 1) {
            cursor.setShape(Qt::PointingHandCursor);
            viewport()->setCursor(cursor);
        } else {
            cursor.setShape(Qt::ArrowCursor);
            viewport()->setCursor(cursor);
        }
    }
}
