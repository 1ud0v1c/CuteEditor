#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QClipboard>
#include <QCloseEvent>
#include "editorqsplitter.h"
/**
 * @author Lucie LAGARRIGUE
 * @author Ludovic VIMONT
 * @brief  Classe EditorWindow : fenêtre principale de l'application.
 */
class EditorWindow : public QMainWindow {
    Q_OBJECT

    public:
        EditorWindow(QWidget *parent = 0);
        ~EditorWindow();
        /**
         * @brief createMenu : crée les éléments du menu et les actions associées.
         */
        void createMenu();
        void centerWindow();
        void createToolbar();
        void setActions();
        QLabel* getStatusBar();
        EditorQSplitter* getCurrentEditorQSplitter();
        void setFilename(QString fileName);
        /**
         * @brief newTabWithName : crée un nouvel onglet avec le nom donné. Utilisé pour l'ouverture de fichiers existants.
         * @param name : le nom du fichier.
         */
        void newTabWithName(const char *name);
        /**
         * @brief restoreContext : charge les informations contenues dans les fichiers de configuration au démarrage de l'application.
         */
        void restoreContext();
        /**
         * @brief setSaveEnable : permet d'activer ou non les boutons de sauvegarde du fichier.
         * @param b : true si on veut activer les boutons, false sinon.
         */
        void setSaveEnable(bool b);
        /**
         * @brief setActiveActionsSelection : permet d'activer ou non les boutons de copier et couper.
         * @param b : true si on veut activer les boutons, false sinon.
         */
        void setActiveActionsSelection(bool b);

    protected :
         void closeEvent(QCloseEvent* event);

    private :
         /**
         * @brief verifyClose
         * @param index : numéro de l'onglet à fermer
         * @return
         */
        int verifyClose(int index);

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
        /**
         * @brief saveContext : sauvegarde les paramètres de l'application. Appelée lorsqu'on ferme l'application.
         */
        void saveContext();
        /**
         * @brief handleClipboard : vérifie s'il existe quelque chose dans le presse-papier et active ou désactive le bouton coller.
         */
        void handleClipboard();

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
        /**
         * @brief _tabManager : objet contenant tous les onglets ouverts
         */
        QTabWidget *_tabManager;
        QToolBar *_toolbar;
        QLabel *_stats;

        bool _isToolbarVisible;
};

#endif // EDITORWINDOW_H
