#ifndef LINENUMBER_H
#define LINENUMBER_H

#include <QWidget>
#include <QTextEdit>
#include <QSize>

/**
 *
 * @author Lucie LAGARRIGUE
 * @author Ludovic VIMONT
 *
 * @brief Classe LineNumber
 */
class LineNumber : public QWidget {
    Q_OBJECT

    public:
        LineNumber(QTextEdit *editor);
        QSize sizeHint() const;

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        QTextEdit *codeEditor;
};

#endif // LINENUMBER_H
