#include "lyrsyntaxhighlighter.h"

LyrSyntaxHighlighter::LyrSyntaxHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    lyrLineFormat.setForeground(Qt::darkBlue);
    lyrLineRegex.setPattern("[^\n#]+&[^\n]+");

    lyrSymbolFormat.setForeground(Qt::blue);
    lyrSymbolRegex.setPattern("&");

    commentFormat.setForeground(Qt::lightGray);
    commentFormat.setFontWeight(QFont::Bold);
    commentRegex.setPattern("#[^\n]+");

    declareFormat.setForeground(Qt::darkRed);
    declareRegex.setPattern("%[^\n]+");

    SyntaxRule  rule;

    rule.rule = lyrLineRegex;
    rule.format = lyrLineFormat;
    syntaxRules.append(rule);
    rule.rule = lyrSymbolRegex;
    rule.format = lyrSymbolFormat;
    syntaxRules.append(rule);
    rule.rule = commentRegex;
    rule.format = commentFormat;
    syntaxRules.append(rule);
    rule.rule = declareRegex;
    rule.format = declareFormat;
    syntaxRules.append(rule);
}

void LyrSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const SyntaxRule &rule, syntaxRules) {
        QRegExp expression(rule.rule);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

}
