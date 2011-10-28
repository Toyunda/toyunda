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


void	RawSubStream::createFromFile(QString filePath)
{
/*  QRegExp isLogoLine("(\\|*)(\\s*)\xff");
  QRegExp isToyLine("^\\s*\\{(\\d+)\\}\\{(\\d+)\\}(.+)\n");
  QRegExp findPipe("^\\s*(\\|+)([^|]*)");
  QRegExp findMultiLine("^\\s*(\\|*)([^|]+)(\\|+)(.*)");*/

  QFile toyfile(filePath);

  currentFrame = 0;
  if (!toyfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qDebug() << "plop";
    qCritical() << "Can't read : " << filePath;
    return ;
  }
  uint	Tid;
  allSyl.clear();
  allText.clear();
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
	text.posx = g.posx;
	text.posy = g.posy;
	text.text = g.text;
	text.size = g.size;
	text._id = Tid;
	text.pipeNumber = g.pipeNumber;
        allText.insert(text);
	Tid++;
      } // Syl
	else {
        ToyundaSyl syl;
	syl.start = g.start;
	syl.stop = g.stop;
	//syl.posx = g.posx;
	//syl.posy = g.posy;
	syl.length = g.lenght;
	syl.pos = g.pos;
	syl.pipeNumber = g.pipeNumber;
	allSyl.insert(syl);
      }
    }
  }
  qDebug() << "=============================";
  std::set<ToyundaText>::iterator it = allText.begin();
  for (;it != allText.end(); ++it)
  {
      qDebug() << (*it).start;
  }
  currentItSyl = allSyl.begin();
  currentItText = allText.begin();
}

QList<RawSubStream::GenLineDesc>	RawSubStream::parseToyundaLine(QString line)
{
  QRegExp	rToyLine("^\\s*\\{(\\d+)\\}\\{(\\d+)\\}\\s*");
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
	  if (line[0] == '|') {
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
	    newline2.text = line;
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
  QRegExp OptColor("\\{c:\\$([0-9A-F]+):?\\$?([0-9A-F]*)\\}");
  QRegExp OptPos("\\{o:(\\d+),(\\d+)\\}");

  if (isOpt.exactMatch(line)) {
    if (OptColor.indexIn(line) != -1) {
      linedesc.color = toyundaColor2QColor(OptColor.cap(1));
      if (!OptColor.cap(2).isEmpty())
        linedesc.color2 = toyundaColor2QColor(OptColor.cap(2));
    }
    // Pos
    if (OptPos.indexIn(line) != -1) {
      linedesc.posx = OptPos.cap(1).toInt();
      linedesc.posy = OptPos.cap(2).toInt();
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

