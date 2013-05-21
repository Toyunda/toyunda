#include "frmsyntaxhighlighter.h"

frmSyntaxHighlighter::frmSyntaxHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    lineFormat.setForeground(Qt::darkRed);
    lineRegex.setPattern("\\d+\\s\\d+");

    commentFormat.setForeground(Qt::lightGray);
    commentFormat.setFontWeight(QFont::Bold);
    commentRegex.setPattern("#[^\n]+");

    SyntaxRule  rule;

    rule.rule = lineRegex;
    rule.format = lineFormat;
    syntaxRules.append(rule);
    rule.rule = commentRegex;
    rule.format = commentFormat;
    syntaxRules.append(rule);
}

void frmSyntaxHighlighter::highlightBlock(const QString &text)
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

