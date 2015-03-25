#ifndef HTMLHIGHLIGHTER_H
#define HTMLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextEdit>
/**
 * @author Lucie LAGARRIGUE
 * @author Ludovic VIMONT
 * @brief Classe HtmlHighlighter : permet de faire la coloration syntaxique du HTML
 */
class HtmlHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

    public:
        enum Construct {
            Entity,
            Tag,
            Comment,
            Attribute,
            Value,
            LastConstruct = Value
        };

        HtmlHighlighter(QTextDocument *document);
        void setFormatFor(Construct construct, const QTextCharFormat &format);
        QTextCharFormat formatFor(Construct construct) const
        { return m_formats[construct]; }

    protected:
        enum State {
            NormalState = -1,
            InComment,
            InTag
        };
        /**
         * @brief highlightBlock : colore syntaxiquement text en repérant les différents éléments HTML possibles.
         * @param text : le texte à colorer.
         */
        void highlightBlock(const QString &text);

    private:
        /**
         * @brief m_formats : tableau contenant la liste des différents formatages.
         */
        QTextCharFormat m_formats[LastConstruct + 1];

};

#endif // HTMLHIGHLIGHTER_H
