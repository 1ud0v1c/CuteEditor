#ifndef QTEXTEDITNUMBER_H
#define QTEXTEDITNUMBER_H

#include <QWidget>
#include <QTextEdit>

class QTextEditNumber : public QTextEdit {
    Q_OBJECT

    public:
        explicit QTextEditNumber(QWidget *parent = 0);
        int getFirstVisibleBlockId();
        void lineNumberAreaPaintEvent(QPaintEvent *event);
        int lineNumberAreaWidth();

    protected:
        void keyPressEvent(QKeyEvent *e);

    signals:

    public slots:
        void resizeEvent(QResizeEvent *e);
        void highlightCurrentLine();

    private slots:
        void updateLineNumberAreaWidth(int newBlockCount);
        void updateLineNumberArea(QRectF /*rect_f*/);
        void updateLineNumberArea(int /*slider_pos*/);
        void updateLineNumberArea();

    private:
        const QString html5 = "<!doctype html>\n<html lang=\"fr\">\n\t<head>\n\t\t <meta charset=\"utf-8\">\n\t\t <title>Titre</title>\n\t\t </head>\n\t<body>\n\t</body>\n</html>";
        QWidget *lineNumberArea;

};

#endif // QTEXTEDITNUMBER_H
