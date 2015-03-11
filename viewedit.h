#ifndef VIEWEDIT_H
#define VIEWEDIT_H

#include <QTextEdit>

class ViewEdit : public QTextEdit {
    Q_OBJECT

    public:
        explicit ViewEdit(QTextEdit *parent = 0);
        void update(QString string);

    protected:
        void mousePressEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);

    signals:

    public slots:

};

#endif // VIEWEDIT_H
