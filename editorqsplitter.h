#ifndef EDITORQSPLITTER_H
#define EDITORQSPLITTER_H

#include <QSplitter>
#include <QTextEdit>
#include <QLabel>
#include <QString>
#include "viewedit.h"
#include "qtexteditnumber.h"
/**
 * @author Lucie LAGARRIGUE
 * @author Ludovic VIMONT
 *
 * @brief Classe EditorQSplitter
 */
class EditorQSplitter : public QSplitter {
    Q_OBJECT

    public:
        explicit EditorQSplitter(QWidget *parent = 0);
        QTextEdit* getEdit();
        void setTabSize(int tabSize);
        void setFilename(QString filename);
        QString getFilename();
        QTextDocument* getDocument();
        void setOpen(bool isOpen);

    protected:
        void dragEnterEvent(QDragEnterEvent *e);
        void dragMoveEvent(QDragMoveEvent *e);
        void dropEvent(QDropEvent *e);

    private:
        /**
         * @brief _edit : l'élément où l'utilisateur va rentrer son texte.
         */
        QTextEditNumber *_edit;
        QLabel *_display;
        /**
         * @brief _view :  l'élément permettant d'afficher le rendu html.
         */
        ViewEdit *_view;
        /**
         * @brief _document : l'élément document lié à _edit permettant de récupérer les informations saisies.
         */
        QTextDocument *_document;
        QString _filename;
        bool _open;

    public slots:
        /**
         * @brief update : slot appelé à chaque modification du document.
         * Mets à jour le nombre de lignes, de caractères, le nom du fichier
         * et l'état du bouton sauvegarder.
         */
        void update();
        void handleSelection();
};

#endif // EDITORQSPLITTER_H
