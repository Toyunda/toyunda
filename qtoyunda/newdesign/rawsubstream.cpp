#include <QFile>
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
  QRegExp isLogoLine("(\\|*)(\\s*)\xff");
  QRegExp isToyLine("^\\s*\\{(\\d+)\\}\\{(\\d+)\\}(.+)\n");
  QRegExp findOpt("\\{(.*)\\}");
  QRegExp findPipe("([^|]*)(\\|+)([^|]*)");
  QRegExp OptColor("\\{c:\\$([0-9A-F]+):?\\$?([0-9A-F]*)\\}");
  QRegExp OptPos("\\{o:(\\d+),(\\d+)\\}");

  QFile toyfile(filePath);
  QTextStream cerr(stderr);

  if (!toyfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    cerr << "Can't read : " << filePath << "\n";
    exit(1);
  }
  while (!toyfile.atEnd()) {
    QByteArray line = toyfile.readLine();
    if (isToyLine.exactMatch(line)) {
      QString optAndText = isToyLine.cap(3);
      //Syl
      if (isLogoLine.indexIn(optAndText) != -1) {
	ToyundaSyl newsyl;
	newsyl.pos = isLogoLine.cap(2).size();
	findPipe.exactMatch(optAndText);
	newsyl.pipeNumber = findPipe.cap(2).size();
	newsyl.length = findPipe.cap(3).size();
	newsyl.start = isToyLine.cap(1).toInt();
	newsyl.stop = isToyLine.cap(2).toInt();
	allSyl.insert(newsyl);
      }
      //Text
      else {
	ToyundaText newtext;
	newtext.start = isToyLine.cap(1).toInt();
	newtext.stop = isToyLine.cap(2).toInt();
	newtext.pipeNumber = 0;
	newtext.posx = -1;
	newtext.posy = -1;
	newtext.size = -1;
	// Colors
	if (OptColor.indexIn(optAndText) != -1) {
	  newtext.color1 = toyundaColor2QColor(OptColor.cap(1));
	  if (!OptColor.cap(2).isEmpty())
	    newtext.color2 = toyundaColor2QColor(OptColor.cap(2));
	}
	// Pos
	if (OptPos.indexIn(optAndText) != -1) {
	  newtext.posx = OptPos.cap(1).toInt();
	  newtext.posy = OptPos.cap(2).toInt();
	}
	optAndText.replace(findOpt, "");
	if (findPipe.exactMatch(optAndText)) {
	  newtext.text = findPipe.cap(3);
	  newtext.pipeNumber = findPipe.cap(2).size();
	}
	else {
	  newtext.text = optAndText;
	}
	allText.insert(newtext);
      }
    }
  }
  currentItSyl = allSyl.begin();
  currentItText = allText.begin();
}

