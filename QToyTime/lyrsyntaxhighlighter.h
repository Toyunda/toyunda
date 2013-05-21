#ifndef LYRSYNTAXHIGHLIGHTER_H
#define LYRSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegExp>
#include <QVector>

class LyrSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    LyrSyntaxHighlighter(QTextDocument *parent);

    void    highlightBlock(const QString &text);

private:
    struct SyntaxRule
    {
        QTextCharFormat format;
        QRegExp         rule;
    };

    QVector<SyntaxRule> syntaxRules;

    QTextCharFormat commentFormat;
    QTextCharFormat lyrLineFormat;
    QTextCharFormat lyrSymbolFormat;
    QTextCharFormat declareFormat;

    QRegExp         commentRegex;
    QRegExp         lyrLineRegex;
    QRegExp         lyrSymbolRegex;
    QRegExp         declareRegex;
};

#endif // LYRSYNTAXHIGHLIGHTER_H
