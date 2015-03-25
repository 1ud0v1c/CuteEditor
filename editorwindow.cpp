#include "editorwindow.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QShortcut>
#include <QKeySequence>
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
#include <sstream>
#include <QSettings>
#include <QCoreApplication>
#include <QTextCodec>

EditorWindow::EditorWindow(QWidget *parent) : QMainWindow(parent) {   
    _tabManager = new QTabWidget(this);
    setCentralWidget(_tabManager);
    _tabManager->setTabsClosable(true);

    createMenu();
    createToolbar();
    setActions();

    _stats = new QLabel(tr("Character(s) : 0  -  Line(s) : 0"));
    statusBar()->addPermanentWidget(_stats);

    setMinimumSize(500,500);
    setWindowTitle("QtEditor");
    centerWindow();

    QShortcut * shortcut = new QShortcut(QKeySequence(tr("Ctrl+H", "Hide|Show")), this);
    shortcut->setContext(Qt::ApplicationShortcut);

    connect(shortcut, SIGNAL(activated()), this, SLOT(toggleToolbar()));
    connect(QApplication::clipboard() ,SIGNAL(dataChanged()), this, SLOT(handleClipboard()));
    restoreContext();
}

void EditorWindow::restoreContext() {
    QSettings settings("QtEditor", "QtEditorSettings");

    settings.beginGroup("General");
    int nbTabs = settings.value("size").value<int>();
    bool isToolbarVisible;
    if(settings.contains("isToolbarVisible")) {
        isToolbarVisible = settings.value("isToolbarVisible").value<bool>();
    } else {
        isToolbarVisible = true;
    }
    (isToolbarVisible) ? _toolbar->show() : _toolbar->hide();
    settings.endGroup();

    if(nbTabs > 0) {
        for (int i = 0; i < nbTabs; ++i) {
            std::stringstream s;
            s << "onglet" << i;
            QString optionName = QString(s.str().c_str());

            settings.beginGroup(optionName);
            QString filename = settings.value("name").value<QString>();
            settings.endGroup();

            newTabWithName(filename.toStdString().c_str());
        }
        _closeFile->setDisabled(false);
    }
}

void EditorWindow::setSaveEnable(bool b){
    _saveFile->setEnabled(b);
    _saveasFile->setEnabled(b);
}

void EditorWindow::setActiveActionsSelection(bool b) {
    _copy->setEnabled(b);
    _cut->setEnabled(b);
}

void EditorWindow::newTabWithName(const char* name) {
    QString filename = QString(name);
    if (!filename.isEmpty()) {
        QFile file(filename);
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream readFile(&file);
        readFile.setCodec("UTF-8");

        QFileInfo fileInfo(file.fileName());
        int newTab = _tabManager->addTab(new EditorQSplitter(), fileInfo.fileName());
        _tabManager->setCurrentIndex(newTab);

        EditorQSplitter* editSplitter = getCurrentEditorQSplitter();
        if(editSplitter) {
            editSplitter->setOpen(true);
            editSplitter->getEdit()->setPlainText(readFile.readAll());
            editSplitter->setFilename(filename);
        }
    } else {
        qDebug() << "Une erreur s'est produite lors de l'ouverture de ce fichier.";
    }
}


void EditorWindow::createMenu() {
    QMenu* file = menuBar()->addMenu(tr("File"));
    _newFile = file->addAction(QIcon(":/img/document-new.png"),tr("New"));
    _newFile->setShortcut(Qt::CTRL + Qt::Key_N);

    _openFile = file->addAction(QIcon(":/img/document-open.png"),tr("Open..."));
    _openFile->setShortcut(Qt::CTRL + Qt::Key_O);

    _saveFile = file->addAction(QIcon(":/img/document-save.png"),tr("Save"));
    _saveFile->setShortcut(Qt::CTRL + Qt::Key_S);
    _saveFile->setDisabled(true);

    _saveasFile = file->addAction(QIcon(":/img/document-save-as.png"),tr("Save as..."));
    if (_tabManager->count() == 0){
        _saveasFile->setDisabled(true);
    }
    _closeFile = file->addAction(QIcon(":/img/document-close.png"),tr("Close"));
    _closeFile->setShortcut(Qt::CTRL + Qt::Key_W);
    if (_tabManager->count() == 0){
        _closeFile->setDisabled(true);
    }

    _quit = file->addAction(QIcon(":/img/document-exit.png"), tr("Quit"), this, SLOT(close()));
    _quit->setShortcut(Qt::CTRL + Qt::Key_Q);

    QMenu* edit = menuBar()->addMenu(tr("Edit"));
    _copy = edit->addAction(QIcon(":/img/edit-copy.png"),tr("Copy"));
    _copy->setDisabled(true);
    _cut = edit->addAction(QIcon(":/img/edit-cut.png"),tr("Paste"));
    _cut->setDisabled(true);
    _paste = edit->addAction(QIcon(":/img/edit-paste.png"),tr("Cut"));
    _paste->setDisabled(true);

    QMenu* about = menuBar()->addMenu(tr("Help"));
    about->addAction(QIcon(":/img/about.png"),tr("About"), this, SLOT(about()));
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
    connect(_tabManager,SIGNAL(currentChanged(int)), this, SLOT(handleChangedTab(int)));
}

void EditorWindow::newTab() {
    int newTab = _tabManager->addTab(new EditorQSplitter(),tr("New Document"));
    _tabManager->setCurrentIndex(newTab);
    _closeFile->setEnabled(true);
    _saveasFile->setEnabled(true);
    _saveFile->setEnabled(true);
}

void EditorWindow::closeTab(int index) {
    int response = verifyClose(index);
    switch(response){
        case QMessageBox::Discard:
            _tabManager->removeTab(index);
            break;
        case QMessageBox::Cancel:
            break;
        case QMessageBox::Save:
            saveFile();
            _tabManager->removeTab(index);
            break;
        default:
            _tabManager->removeTab(index);
            break;
    }
    if(_tabManager->count() == 0){
        _closeFile->setDisabled(true);
        _saveasFile->setDisabled(true);
        _saveFile->setDisabled(true);
    }
}

int EditorWindow::verifyClose(int index){
    EditorQSplitter* widget = dynamic_cast<EditorQSplitter*>(_tabManager->widget(index));
    if(widget->getDocument()->isModified()) {
        QMessageBox* save = new QMessageBox(0);
        save->setModal(true);
        std::stringstream s;
        s << "Your document \"" << widget->getFilename().toStdString() << "\" has been modified.";
        save->setText(tr(s.str().c_str()));
        save->setInformativeText(tr("Do you want to save changes ?"));
        save->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        save->setDefaultButton(QMessageBox::Save);
        save->show();
        int selection = save->exec();
        return selection;
    }
    return -1;
}

void EditorWindow::openFile() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File..."),QString(), tr("HTML-Files (*.html);;CSS-Files (*.css);;All Files (*)"));
    if (!filename.isEmpty()) {
        QFile file(filename);
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream readFile(&file);
        readFile.setCodec("UTF-8");

        QFileInfo fileInfo(file.fileName());
        int newTab = _tabManager->addTab(new EditorQSplitter(),fileInfo.fileName());
        _tabManager->setCurrentIndex(newTab);

        EditorQSplitter* editSplitter = getCurrentEditorQSplitter();
        if(editSplitter) {
            editSplitter->setOpen(true);
            editSplitter->getEdit()->setPlainText(readFile.readAll());
            editSplitter->setFilename(filename);
        }
    } else {
        qDebug() << "An error occured during the opening of the file.";
    }
}


bool EditorWindow::saveFile() {
    bool success = false;
    EditorQSplitter* editSplitter = getCurrentEditorQSplitter();
    if(editSplitter) {
        QString filename = editSplitter->getFilename();
        if (filename.isEmpty()) {
            return saveAs();
        }
        QTextDocumentWriter writer(filename);
        writer.setFormat("plaintext");
        writer.setCodec(QTextCodec::codecForName("UTF-8"));
        success = writer.write(editSplitter->getEdit()->document());
        if(success) {
            editSplitter->getDocument()->setModified(false);
            QString title = _tabManager->tabText(_tabManager->currentIndex());
            if (title.contains(QString("(*)"))){
                std::size_t found = title.toStdString().find(" (*)");
                if (found != std::string::npos) {
                    _tabManager->setTabText(_tabManager->currentIndex(), title.replace(found, QString(" (*)").size(), ""));
                }
            }
            _saveFile->setDisabled(true);
        }
    }
    return success;
}

bool EditorWindow::saveAs() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("HTML Files (*.html);;CSS-Files (*.css);;All Files (*)"));

    if (!filename.isEmpty()) {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Erreur lors de l'ecriture du fichier";
        } else {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            EditorQSplitter* editSplitter = getCurrentEditorQSplitter();
            if(editSplitter) {
                stream << editSplitter->getEdit()->toPlainText();
            }
            stream.flush();
            file.close();
            editSplitter->setFilename(filename);
            QFileInfo fileInfo(filename);
            _tabManager->setTabText(_tabManager->currentIndex(), QString(fileInfo.fileName()));
            editSplitter->getDocument()->setModified(false);

            QString title = _tabManager->tabText(_tabManager->currentIndex());
            if (title.contains(QString("(*)"))){
                std::size_t found = title.toStdString().find(" (*)");
                if (found != std::string::npos) {
                    _tabManager->setTabText(_tabManager->currentIndex(), title.replace(found, QString(" (*)").size(), ""));
                }
            }

        }
    } else {
        return false;
    }
    return true;
}

void EditorWindow::closeFile() {
    closeTab(_tabManager->currentIndex());
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
    _toolbar = new QToolBar();
    _toolbar->insertAction(0,_newFile);
    _toolbar->insertAction(0,_openFile);
    _toolbar->insertAction(0,_saveFile);
    _toolbar->insertAction(0,_saveasFile);
    _toolbar->insertAction(0,_copy);
    _toolbar->insertAction(0,_cut);
    _toolbar->insertAction(0,_paste);
    _toolbar->setMovable(false);
    _toolbar->setFixedWidth(500);
    addToolBar(_toolbar);
}

void EditorWindow::about() {
    QMessageBox *msg = new QMessageBox();
    msg->setText(tr("HTML Editor, TP4 QT : ENSICAEN\nAuthors : Vimont Ludovic & Lagarrigue Lucie"));
    msg->exec();
}

QLabel* EditorWindow::getStatusBar() {
    return _stats;
}

void EditorWindow::toggleToolbar() {
    if(_toolbar->isVisible()) {
        _toolbar->hide();
        _isToolbarVisible = false;
    } else {
        _toolbar->show();
        _isToolbarVisible = true;
    }
}


void EditorWindow::handleChangedTab(int index) {
    EditorQSplitter* editSplitter = getCurrentEditorQSplitter();
    if(editSplitter) {
        QTextDocument *doc = editSplitter->getEdit()->document();
        int lines = doc->lineCount();
        int characters = doc->characterCount()-1;
        std::ostringstream s;
        s << "Character(s) : " << characters << " - Line(s) : " << lines;
        getStatusBar()->setText(tr(s.str().c_str()));
    }
}

void EditorWindow::saveContext() {
   QSettings settings("QtEditor", "QtEditorSettings");
   settings.clear();

   settings.beginGroup("General");
   settings.setValue("isToolbarVisible", _isToolbarVisible);
   settings.setValue("size",_tabManager->count());
   settings.endGroup();

   for (int i = 0; i < _tabManager->count(); ++i) {
        EditorQSplitter* currentTab = dynamic_cast<EditorQSplitter *>(_tabManager->widget(i));

        std::stringstream s;
        s << "onglet" << i;
        QString optionName = QString(s.str().c_str());

        settings.beginGroup(optionName);
        settings.setValue("name",(!currentTab->getFilename().isEmpty()) ? currentTab->getFilename() : QString(tr("New Document")));
        settings.endGroup();
   }
}

void EditorWindow::closeEvent(QCloseEvent* event){
    int response;
    bool hasToQuit = true;
    for (int i = 0; i < _tabManager->count(); ++i) {
        response = verifyClose(i);
        if (response ==  QMessageBox::Cancel){
            hasToQuit = false;
        }
        if (response == QMessageBox::Save){
            saveFile();
        }
        if (response == QMessageBox::Discard){
            EditorQSplitter* currentTab = dynamic_cast<EditorQSplitter *>(_tabManager->widget(i));
            if (currentTab->getFilename().isEmpty()){
                _tabManager->removeTab(i);
            }
        }
    }
    if (hasToQuit){
        saveContext();
        event->accept();
    }else{
        event->ignore();
    }
}

void EditorWindow::handleClipboard() {
    QClipboard *clipboard = QApplication::clipboard();
    QString text = clipboard->text();
    if(text.length() > 0) {
        _paste->setDisabled(false);
    } else {
        _paste->setDisabled(true);
    }
}

EditorWindow::~EditorWindow() {

}
