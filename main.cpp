#include "editorwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator *translator = new QTranslator;
    translator->load("data/qteditor_fr.qm", ".");
    a.installTranslator(translator);

    EditorWindow w;
    w.show();

    return a.exec();
}
