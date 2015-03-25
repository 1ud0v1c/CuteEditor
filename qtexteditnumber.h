#ifndef QTEXTEDITNUMBER_H
#define QTEXTEDITNUMBER_H

#include <QWidget>
#include <QTextEdit>
#include <map>
/**
 *
 * @author Lucie LAGARRIGUE
 * @author Ludovic VIMONT
 *
 * @brief Classe QTextEditNumber
 */
class QTextEditNumber : public QTextEdit {
    Q_OBJECT

    public:
        explicit QTextEditNumber(QWidget *parent = 0);
        int getFirstVisibleBlockId();
        void lineNumberAreaPaintEvent(QPaintEvent *event);
        int lineNumberAreaWidth();
        /**
         * @brief createSnippets : initialise les snippets pour chaque élément
         */
        void createSnippets();
        bool getOpen();
        void setOpen(bool isOpen);

    protected:
        /**
         * @brief keyPressEvent
         * @param e : l'évènement lié à la touche pressée
         * Si la touche pressée est tab, on applique la snippet. Sinon, on passe l'évènement à QTextEdit.
         */
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
        /**
         * @brief _snippets : map contenant les snippets
         * Pour chaque snippet, on a sa clé, qui est le raccourci utilisé, et la chaîne de caractères,
         * qui est un élément html.
         */
        std::map<std::string, QString> _snippets;
};

#endif // QTEXTEDITNUMBER_H
