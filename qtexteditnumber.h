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
        bool getOpen();
        void setOpen(bool isOpen);

    protected:
        void keyPressEvent(QKeyEvent *e);

    signals:

    public slots:
        void resizeEvent(QResizeEvent *e);
        void highlightCurrentLine();

    private slots:
        void updateLineNumberAreaWidth(int newBlockCount);
        void updateLineNumberArea(QRectF);
        void updateLineNumberArea(int);
        void updateLineNumberArea();

    private:
        QWidget *lineNumberArea;
        bool _open;
        std::map<std::string, QString> _snippets;
};

#endif // QTEXTEDITNUMBER_H
