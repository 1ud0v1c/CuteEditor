#ifndef EDITORQSPLITTER_H
#define EDITORQSPLITTER_H

#include <QSplitter>
#include <QTextEdit>
#include <QLabel>
#include <QString>
#include "viewedit.h"
#include "qtexteditnumber.h"

class EditorQSplitter : public QSplitter {
    Q_OBJECT

    public:
        explicit EditorQSplitter(QWidget *parent = 0);
        QTextEdit* getEdit();
        void setTabSize(int tabSize);
        void setFilename(QString filename);
        QString getFilename();

    protected:
        void dragEnterEvent(QDragEnterEvent *e);
        void dragMoveEvent(QDragMoveEvent *e);
        void dropEvent(QDropEvent *e);

    private:
        QTextEditNumber *_edit;
        QLabel *_display;
        ViewEdit *_view;
        QTextDocument *_document;
        QString _filename;
        bool _changed;

    public slots:
        void update();
};

#endif // EDITORQSPLITTER_H
