#ifndef FRMSYNTAXHIGHLIGHTER_H
#define FRMSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QVector>
#include <QRegExp>
#include <QTextCharFormat>

class frmSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    frmSyntaxHighlighter(QTextDocument  *parent);
    void    highlightBlock(const QString &text);

private:
    struct SyntaxRule
    {
        QTextCharFormat format;
        QRegExp         rule;
    };

    QVector<SyntaxRule>      syntaxRules;
    QTextCharFormat commentFormat;
    QRegExp         commentRegex;
    QTextCharFormat lineFormat;
    QRegExp         lineRegex;
};

#endif // FRMSYNTAXHIGHLIGHTER_H
