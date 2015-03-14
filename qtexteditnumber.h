#ifndef QTEXTEDITNUMBER_H
#define QTEXTEDITNUMBER_H

#include <QWidget>
#include <QTextEdit>
#include <map>

class QTextEditNumber : public QTextEdit {
    Q_OBJECT

    public:
        explicit QTextEditNumber(QWidget *parent = 0);
        int getFirstVisibleBlockId();
        void lineNumberAreaPaintEvent(QPaintEvent *event);
        int lineNumberAreaWidth();
        void createSnippets();

    protected:
        void keyPressEvent(QKeyEvent *e);
        void dragEnterEvent(QDragEnterEvent *e);
        void dragMoveEvent(QDragMoveEvent *e);
        void dropEvent(QDropEvent *e);

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
        QWidget *lineNumberArea;
        std::map<std::string, QString> _snippets;
};

#endif // QTEXTEDITNUMBER_H
