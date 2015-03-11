#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QClipboard>
#include "editorqsplitter.h"

class EditorWindow : public QMainWindow {
    Q_OBJECT

    public:
        EditorWindow(QWidget *parent = 0);
        ~EditorWindow();
        void createMenu();
        void centerWindow();
        void createToolbar();
        void setActions();
        QLabel* getStatusBar();
        EditorQSplitter* getCurrentEditorQSplitter();

    public slots:
        bool saveFile();
        void newTab();
        void openFile();
        bool saveAs();
        void closeFile();
        void copy();
        void cut();
        void paste();
        void about();
        void closeTab(int index);
        void toggleToolbar();
        void handleChangedTab(int index);
        void saveContext();

    private:
        QAction* _newFile;
        QAction* _openFile;
        QAction* _saveFile;
        QAction* _saveasFile;
        QAction* _closeFile;
        QAction* _quit;
        QAction* _copy;
        QAction* _cut;
        QAction* _paste;

        QTabWidget *_tabManager;
        QToolBar *_toolbar;
        QLabel *_stats;
        QString _fileName;
};

#endif // EDITORWINDOW_H
