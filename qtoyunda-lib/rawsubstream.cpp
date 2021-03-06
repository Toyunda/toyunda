/*
    This file is part of qtoyunda software
    Copyright (C) 2008  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

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

#include <QFile>
#include <QList>
#include "rawsubstream.h"

RawSubStream::RawSubStream() : ToyundaSubStream()
{
  identifier = "raw";
}

RawSubStream::RawSubStream(const QString filePath) : ToyundaSubStream()
{
  identifier = "raw";
  createFromFile(filePath);
}


bool	RawSubStream::createFromFile(QString filePath)
{
/*  QRegExp isLogoLine("(\\|*)(\\s*)\xff");
  QRegExp isToyLine("^\\s*\\{(\\d+)\\}\\{(\\d+)\\}(.+)\n");
  QRegExp findPipe("^\\s*(\\|+)([^|]*)");
  QRegExp findMultiLine("^\\s*(\\|*)([^|]+)(\\|+)(.*)");*/

  QFile toyfile(filePath);

  currentFrame = 0;
  if (!toyfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qCritical() << "Can't read : " << filePath;
    return false;
  }
  uint	Tid = 0;
  allSyl.clear();
  allSyl.reserve((toyfile.size() / 20) / 2);
  allText.clear();
  allText.reserve((toyfile.size() / 20));
  currentItSyl = allSyl.begin();
  currentItText = allText.begin();
  while (!toyfile.atEnd()) {
    QByteArray line = toyfile.readLine();
    QList<GenLineDesc> lg = parseToyundaLine(line);
    QListIterator<GenLineDesc> it(lg);
    while (it.hasNext()) {
      GenLineDesc g = it.next();
      // Text
      if (g.lenght == -1) {
        ToyundaText text;
        text.start = g.start;
        text.stop = g.stop;
        text.color1 = g.color;
        text.color2 = g.color2;
        text.fadingcolor = g.color;
	text.posx = g.posx;
    text.pos2x = g.pos2x;
    text.interposx = g.posx;
    text.interposy = g.posy;
    text.intersize = g.size;
    text.pos2y = g.pos2y;
	text.posy = g.posy;
	text.text = g.text;
	text.size = g.size;
    text.size2 = g.size2;
	text._id = Tid;
	text.pipeNumber = g.pipeNumber;
        allText.append(text);
	Tid++;
      } // Syl
	else {
        ToyundaSyl syl;
	syl.start = g.start;
	syl.stop = g.stop;
        syl.posx = g.posx;
        syl.posy = g.posy;
	syl.length = g.lenght;
	syl.pos = g.pos;
        syl.alpha = -1;
        if (g.color.isValid())
            syl.alpha = g.color.alpha();
        syl.alpha2 = -1;
        if (g.color2.isValid()) {
            syl.alpha2 = g.color2.alpha();
            syl.fadingalpha = g.color.alpha();
        }
        syl.size = g.size;
	syl.pipeNumber = g.pipeNumber;
        allSyl.append(syl);
      }
    }
  }
  qSort(allSyl.begin(), allSyl.end());
  qSort(allText.begin(), allText.end());
  currentItSyl = allSyl.begin();
  currentItText = allText.begin();
  return true;
}

QList<RawSubStream::GenLineDesc>	RawSubStream::parseToyundaLine(QString line)
{
    QRegExp	rToyLine("^\\s*\\{(\\d+)\\}\\{(\\d+)\\}");
    QRegExp	isToyLine("^\\s*\\{(\\d+)\\}\\{(\\d+)\\}.*");
    QRegExp	isSylLine("^(\\s*)\xff.*");
    QRegExp	notPipe("^([^|]+)");

    QList<GenLineDesc> list;
    if (isToyLine.exactMatch(line)) {
        GenLineDesc	newline;
        newline.pipeNumber = 0;
        newline.size = -1;
        newline.posx = -1;
        newline.posy = -1;
        newline.pos2x = -1;
        newline.pos2y = -1;
        newline.size2 = -1;
        newline.lenght = -1;
        newline.start = isToyLine.cap(1).toInt();
        newline.stop = isToyLine.cap(2).toInt();
        line.replace(rToyLine, "");
        line.replace(QRegExp("\\n$"), "");
        parseOption(line, newline);
        parsePipe(line, newline);
        parseOption(line, newline);
        if (isSylLine.exactMatch(line)) {
            notPipe.indexIn(line);
            newline.lenght = notPipe.cap(1).size();
            newline.pos = isSylLine.cap(1).size();
            list.append(newline);
            return list;
        } else {
            QRegExp	rText("^([^|]+)");
            if (rText.indexIn(line) != -1) {
                newline.text = rText.cap(1);
                line.replace(rText, "");
                list.append(newline);
                while (line[0] == '|') {
                    GenLineDesc newline2;
                    newline2.pipeNumber = 0;
                    newline2.size = -1;
                    newline2.posx = -1;
                    newline2.posy = -1;
                    newline2.lenght = -1;
                    newline2.start = newline.start;
                    newline2.stop = newline.stop;
                    parsePipe(line, newline2);
                    parseOption(line, newline2);
                    if (rText.indexIn(line) != -1)
                    {
                        newline2.text = rText.cap(1);
                        line.replace(rText, "");
                    }
                    list.append(newline2);
                }
            }
            return list;
        }
    }
    else
        return list;
}

void	RawSubStream::parseOption(QString &line, GenLineDesc &linedesc)
{
  QRegExp rOpt("^(\\{(.*)\\})+");
  QRegExp isOpt("^(\\{(.*)\\})+.*");
  QRegExp OptColor("\\{c:\\$?([0-9A-F]+):?\\$?([0-9A-F]*)\\}");
  QRegExp OptPos("\\{o:(-?\\d+),(-?\\d+):?(?:(-?\\d+),(-?\\d+))?\\}");
  QRegExp OptSize("\\{s:(\\d+):?(?:(\\d+))?\\}");

  if (isOpt.exactMatch(line)) {
      if (OptSize.indexIn(line) != -1)
      {
        linedesc.size = OptSize.cap(1).toInt();
        if (!OptSize.cap(2).isEmpty())
            linedesc.size2 = OptSize.cap(2).toInt();
      }
    if (OptColor.indexIn(line) != -1) {
      linedesc.color = toyundaColor2QColor(OptColor.cap(1));
      if (!OptColor.cap(2).isEmpty())
        linedesc.color2 = toyundaColor2QColor(OptColor.cap(2));
    }
    // Pos
    if (OptPos.indexIn(line) != -1) {
      linedesc.posx = OptPos.cap(1).toInt();
      linedesc.posy = OptPos.cap(2).toInt();
      if (!OptPos.cap(3).isEmpty())
      {
	  linedesc.pos2x = OptPos.cap(3).toInt();
	  linedesc.pos2y = OptPos.cap(4).toInt();
      }
    }
  }
  line.replace(rOpt, "");
}

void	RawSubStream::parsePipe(QString &line, GenLineDesc &linedesc)
{
  QRegExp  rPipe("^\\s*(\\|+)");
  QRegExp  isPipe("^\\s*(\\|+).*");

  if (isPipe.exactMatch(line)) {
    linedesc.pipeNumber = isPipe.cap(1).size();
    line.replace(rPipe, "");
  }
}

void RawSubStream::reinit()
{
    currentItSyl = allSyl.begin();
    currentSyl.clear();
    currentItText = allText.begin();
    currentText.clear();
}

RawSubStream::~RawSubStream()
{
}
