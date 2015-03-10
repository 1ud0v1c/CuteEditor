#include "editorwindow.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QStatusBar>
#include <QMessageBox>
#include <QToolBar>
#include <QLabel>
#include <QTextStream>
#include <QTabWidget>
#include <QTextCursor>
#include <QString>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QFile>
#include <QMenuBar>
#include <QMenu>
#include <QClipboard>
#include <QDebug>

EditorWindow::EditorWindow(QWidget *parent) : QMainWindow(parent) {
    _tabManager = new QTabWidget(this);
    setCentralWidget(_tabManager);
    _tabManager->addTab(new EditorQSplitter(),"New Document");
    _tabManager->setTabsClosable(true);

    createMenu();
    createToolbar();
    setActions();

    _stats = new QLabel("Caractere(s) : 0  -  Ligne(s) : 0");
    statusBar()->addPermanentWidget(_stats);

    setMinimumSize(500,500);
    setWindowTitle("QT VEditeur");
    centerWindow();
}

void EditorWindow::createMenu() {
    QMenu* file = menuBar()->addMenu("File");
    _newFile = file->addAction(QIcon(":/img/document-new.png"),"Nouveau");
    _newFile->setShortcut(Qt::CTRL + Qt::Key_N);

    _openFile = file->addAction(QIcon(":/img/document-open.png"),"Ouvrir...");
    _openFile->setShortcut(Qt::CTRL + Qt::Key_O);

    _saveFile = file->addAction(QIcon(":/img/document-save.png"),"Enregistrer");
    _saveFile->setShortcut(Qt::CTRL + Qt::Key_S);

    _saveasFile = file->addAction(QIcon(":/img/document-save-as.png"),"Enregistrer sous...");

    _closeFile = file->addAction(QIcon(":/img/document-close.png"),"Fermer");
    _closeFile->setShortcut(Qt::CTRL + Qt::Key_W);

    _quit = file->addAction(QIcon(":/img/document-exit.png"), "Quitter", this, SLOT(close()));
    _quit->setShortcut(Qt::CTRL + Qt::Key_Q);

    QMenu* edit = menuBar()->addMenu("Edition");
    _copy = edit->addAction(QIcon(":/img/edit-copy.png"),"Copier");
    _cut = edit->addAction(QIcon(":/img/edit-cut.png"),"Couper");
    _paste = edit->addAction(QIcon(":/img/edit-paste.png"),"Coller");

    QMenu* about = menuBar()->addMenu("Aide");
    about->addAction(QIcon(":/img/about.png"),"A propos", this, SLOT(about()));
}

void EditorWindow::setActions() {
    connect(_newFile, SIGNAL(triggered()), this, SLOT(newTab()));
    connect(_openFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(_saveFile, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(_saveasFile, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(_closeFile, SIGNAL(triggered()), this, SLOT(closeFile()));

    connect(_copy, SIGNAL(triggered()), this, SLOT(copy()));
    connect(_cut, SIGNAL(triggered()), this, SLOT(cut()));
    connect(_paste, SIGNAL(triggered()), this, SLOT(paste()));

    connect(_tabManager,SIGNAL(tabCloseRequested(int)),this, SLOT(closeTab(int)));
}

void EditorWindow::newTab() {
    int newTab = _tabManager->addTab(new EditorQSplitter(),"New Document");
    _tabManager->setCurrentIndex(newTab);
}

void EditorWindow::closeTab(int index) {
    _tabManager->removeTab(index);
}

void EditorWindow::openFile() {
    _fileName = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                                 QString(), tr("HTML-Files (*.html);;All Files (*)"));
    if (!_fileName.isEmpty()) {
        QFile file(_fileName);
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream ReadFile(&file);

        QFileInfo fileInfo(file.fileName());
        int newTab = _tabManager->addTab(new EditorQSplitter(),fileInfo.fileName());
        _tabManager->setCurrentIndex(newTab);

        EditorQSplitter* editSplitter = getCurrentEditorQSplitter();
        if(editSplitter) {
            editSplitter->getEdit()->setPlainText(ReadFile.readAll());
        }
    } else {
        qDebug() << "Une erreur s'est produite lors de l'ouverture de ce fichier.";
    }
}

bool EditorWindow::saveFile() {
    if (_fileName.isEmpty()) {
        return saveAs();
    }
    bool success = false;
    QTextDocumentWriter writer(_fileName);
    EditorQSplitter* editSplitter = getCurrentEditorQSplitter();
    if(editSplitter) {
        writer.setFormat("plaintext");
        success = writer.write(editSplitter->getEdit()->document());
    }
    if(success) {
        editSplitter->getEdit()->document()->setModified(false);
    }
    return success;
}

bool EditorWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("HTML Files (*.html);;All Files (*)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Erreur lors de l'ecriture du fichier";
        } else {
            QTextStream stream(&file);
            EditorQSplitter* editSplitter = getCurrentEditorQSplitter();
            if(editSplitter) {
                stream << editSplitter->getEdit()->toPlainText();
            }
            stream.flush();
            file.close();
            _fileName = fileName;
        }
    } else {
        return false;
    }
    return true;
}

void EditorWindow::closeFile() {
    _tabManager->removeTab(_tabManager->currentIndex());
}

void EditorWindow::copy() {
    EditorQSplitter* editSplitter = getCurrentEditorQSplitter();
    if(editSplitter) {
        editSplitter->getEdit()->copy();
    }
}

void EditorWindow::cut() {
    EditorQSplitter* editSplitter = getCurrentEditorQSplitter();
    if(editSplitter) {
        editSplitter->getEdit()->cut();
    }
}

void EditorWindow::paste() {
    EditorQSplitter* editSplitter = getCurrentEditorQSplitter();
    if(editSplitter) {
        editSplitter->getEdit()->paste();
    }
}

EditorQSplitter* EditorWindow::getCurrentEditorQSplitter() {
    QWidget *currentWidget = (_tabManager->currentWidget());
    EditorQSplitter* editSplitter = dynamic_cast<EditorQSplitter *>(currentWidget);
    return (editSplitter) ? editSplitter : 0;
}

void EditorWindow::centerWindow() {
    QDesktopWidget desktop;
    QRect surface = desktop.screenGeometry();
    int x = surface.width()/2 - width()/2;
    int y = surface.height()/2 - height()/2;
    move(x,y);
}

void EditorWindow::createToolbar() {
    QToolBar *toolbar = new QToolBar();
    toolbar->insertAction(0,_newFile);
    toolbar->insertAction(0,_openFile);
    toolbar->insertAction(0,_saveFile);
    toolbar->insertAction(0,_saveasFile);
    toolbar->insertAction(0,_copy);
    toolbar->insertAction(0,_cut);
    toolbar->insertAction(0,_paste);
    toolbar->setMovable(false);
    toolbar->setFixedWidth(500);
    addToolBar(toolbar);
}

void EditorWindow::about() {
    QMessageBox *msg = new QMessageBox();
    msg->setText("Editeur HTML, TP4 QT");
    msg->exec();
}

QLabel* EditorWindow::getStatusBar() {
    return _stats;
}


EditorWindow::~EditorWindow() {

}
