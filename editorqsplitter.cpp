#include "editorqsplitter.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextDocument>
#include <QTabWidget>
#include <QDebug>
#include <QFontMetrics>
#include <QFont>
#include <sstream>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include "editorwindow.h"
#include "htmlhighlighter.h"

EditorQSplitter::EditorQSplitter(QWidget *parent) : QSplitter(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    _edit = new QTextEditNumber();
    _edit->setStyleSheet("background: #c0c0c0;");
    _edit->setUndoRedoEnabled(true);
    setTabSize(4);
    layout->addWidget(_edit);

    QVBoxLayout *vboxLayout = new QVBoxLayout();
    QWidget *rightPanel = new QWidget();
    rightPanel->setLayout(vboxLayout);
    _display = new QLabel("Apercu");
    vboxLayout->addWidget(_display);
    _document = _edit->document();
    HtmlHighlighter *highlighter = new HtmlHighlighter(_document);
    _view = new ViewEdit();
    vboxLayout->addWidget(_view);

    layout->addWidget(rightPanel);

    setAcceptDrops(true);
    connect(_document,SIGNAL(contentsChanged()),this,SLOT(update()));
    connect(_edit, SIGNAL(selectionChanged()),this, SLOT(handleSelection()));
}

void EditorQSplitter::setTabSize(int tabSize) {
    QFont font;
    QFontMetrics metrics(font);
    _edit->setTabStopWidth(tabSize * metrics.width(' '));
}

void EditorQSplitter::update() {
    _view->update(_edit->toPlainText());

    QObject *currentWidget = this->parent();
    QTabWidget* tabWidget = qobject_cast<QTabWidget *>(currentWidget->parent());
    EditorWindow* window = qobject_cast<EditorWindow *>(tabWidget->parent());
    int lines = _document->lineCount();
    int characters = _document->characterCount()-1;

    std::ostringstream s;
    s << "Caractere(s) : " << characters << " - Lignes : " << lines;
    window->getStatusBar()->setText(s.str().c_str());

    if(tabWidget && _document->isModified() && !_edit->getOpen()) {
        QString title = tabWidget->tabText(tabWidget->currentIndex());
        if (!title.contains(QString("(*)"))){
            window->setSaveEnable(true);
            tabWidget->setTabText(tabWidget->currentIndex(), title +" (*)");
        }
    }
}

void EditorQSplitter::handleSelection() {
    QString selectedText = _edit->textCursor().selectedText();
    EditorWindow* window = qobject_cast<EditorWindow *>(this->parent()->parent()->parent());
    if(selectedText.length() > 0) {
        window->setActiveActionsSelection(true);
    } else {
        window->setActiveActionsSelection(false);
    }
}


QTextEdit* EditorQSplitter::getEdit() {
    return _edit;
}

QString EditorQSplitter::getFilename() {
    return _filename;
}

void EditorQSplitter::setFilename(QString filename) {
    _filename = filename;
}

void EditorQSplitter::dragEnterEvent(QDragEnterEvent *e) {
    e->accept();
}

void EditorQSplitter::dragMoveEvent(QDragMoveEvent *e) {
    e->accept();
}

void EditorQSplitter::dropEvent(QDropEvent *e) {
    if (e->mimeData()->hasFormat("text/uri-list")) {
        QList<QUrl> urls = e->mimeData()->urls();
        if (urls.isEmpty()) {
            return;
        }

        QString fileName = urls.first().toLocalFile();
        if (fileName.isEmpty()) {
           return;
        } else {
            QObject *currentWidget = this->parent();
            QTabWidget* tabWidget = qobject_cast<QTabWidget *>(currentWidget->parent());
            EditorWindow* window = qobject_cast<EditorWindow *>(tabWidget->parent());

            QFile file(fileName);
            file.open(QFile::ReadOnly | QFile::Text);
            QTextStream ReadFile(&file);

            QFileInfo fileInfo(file.fileName());
            int newTab = tabWidget->addTab(new EditorQSplitter(),fileInfo.fileName());
            tabWidget->setCurrentIndex(newTab);

            EditorQSplitter* editSplitter = window->getCurrentEditorQSplitter();
            if(editSplitter) {
                editSplitter->getEdit()->setPlainText(ReadFile.readAll());
                editSplitter->setFilename(fileName);
            }
        }
    }
}

QTextDocument *EditorQSplitter::getDocument() {
    return _document;
}

void EditorQSplitter::setOpen(bool isOpen) {
    _edit->setOpen(isOpen);
}
