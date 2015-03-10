#ifndef EDITORQSPLITTER_H
#define EDITORQSPLITTER_H

#include <QSplitter>
#include <QTextEdit>
#include <QLabel>
#include "viewedit.h"
#include "qtexteditnumber.h"

class EditorQSplitter : public QSplitter {
    Q_OBJECT

    public:
        explicit EditorQSplitter(QWidget *parent = 0);
        QTextEdit* getEdit();
        void setTabSize(int tabSize);

    private:
        QTextEditNumber *_edit;
        QLabel *_display;
        ViewEdit *_view;
        bool _changed;
        QTextDocument *_document;

    public slots:
        void update();
};

#endif // EDITORQSPLITTER_H
