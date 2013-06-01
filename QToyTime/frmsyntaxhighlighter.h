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
