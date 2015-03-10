#include "editorqsplitter.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextDocument>
#include <QTabWidget>
#include <QDebug>
#include <QFontMetrics>
#include <QFont>
#include <sstream>
#include "editorwindow.h"
#include "htmlhighlighter.h"

EditorQSplitter::EditorQSplitter(QWidget *parent) : QSplitter(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    _edit = new QTextEditNumber();
    _edit->setStyleSheet("background: #c0c0c0;");
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

    _changed = false;
    connect(_document,SIGNAL(contentsChanged()),this,SLOT(update()));
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

    if(tabWidget && !_changed) {
        tabWidget->setTabText(tabWidget->currentIndex(), tabWidget->tabText(tabWidget->currentIndex()) +" (*)");
        _changed = true;
    }
}

QTextEdit* EditorQSplitter::getEdit() {
    return _edit;
}
