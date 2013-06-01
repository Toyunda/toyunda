/*
    This file is part of QToyunda software
    Copyright (C) 2013  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
