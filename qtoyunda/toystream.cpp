#include <QRegExp>
#include <QFile>
#include <iostream>
#include <QTextStream>
#include "toystream.h"

extern QTextStream cout;
extern QTextStream cerr;

ToyStream::ToyStream() : QObject()
{

}

//{c:$80FF55CC:$00FF55CC}{s:20}{o:350,540}

void	ToyStream::loadFile(QString filePath)
{
  QRegExp isLogoLine("(\\|*)(\\s*)\xff");
  QRegExp isToyLine("^\\s*\\{(\\d+)\\}\\{(\\d+)\\}(.+)\n");
  QRegExp findOpt("\\{(.*)\\}");
  QRegExp findPipe("([^|]*)(\\|+)([^|]*)");
  QRegExp OptColor("\\{c:\\$([0-9A-F]+):?\\$?([0-9A-F]*)\\}");
  QRegExp OptPos("\\{o:(\\d+),(\\d+)\\}");

  QFile toyfile(filePath);
 
  if (!toyfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
     cerr << "Can't read : " << filePath << "\n";
     exit(1);
  }
  while (!toyfile.atEnd()) {
    QByteArray line = toyfile.readLine();
    if (isToyLine.exactMatch(line)) {
      QString optAndText = isToyLine.cap(3);
      // Syl
      if (isLogoLine.indexIn(optAndText) != -1) {
        ToySyl newsyl;
	newsyl.pos = isLogoLine.cap(2).size();
	findPipe.exactMatch(optAndText);
        newsyl.nbpipe = findPipe.cap(2).size();
	newsyl.length = findPipe.cap(3).size();
	newsyl.start = isToyLine.cap(1).toInt();
	newsyl.stop = isToyLine.cap(2).toInt();
	syl.insert(newsyl);
	//show_ToySyl(newsyl);
      }
      //Text
      else {
        ToyText newtext;
	newtext.start = isToyLine.cap(1).toInt();
	newtext.stop = isToyLine.cap(2).toInt();
	newtext.nbpipe = 0;
        newtext.posx = -1;
        newtext.posy = -1;
	// Colors
	if (OptColor.indexIn(optAndText) != -1) {
          newtext.color1 = toycolor2QColor(OptColor.cap(1));
	  if (!OptColor.cap(2).isEmpty())
	    newtext.color2 = toycolor2QColor(OptColor.cap(2));
	}
	// Pos
	if (OptPos.indexIn(optAndText) != -1) {
          newtext.posx = OptPos.cap(1).toInt();
	  newtext.posy = OptPos.cap(2).toInt();
	}
	optAndText.replace(findOpt, "");
	if (findPipe.exactMatch(optAndText)) {
	  newtext.text = findPipe.cap(3);
	  newtext.nbpipe = findPipe.cap(2).size();
	}
	else {
	  newtext.text = optAndText;
	}
        text.insert(newtext);
	//show_ToyText(newtext);
      }
    }
  }
}

QList<ToyText> ToyStream::getCurrentText()
{
  return currentText;
}


QList<ToySyl> ToyStream::getCurrentSyl()
{
  return currentSyl;
}

void	ToyStream::setCurrentFrame(int nb)
{
  currentframe = nb;
  ToySyl tmps;
  bool emitchange = false;

  
  // see if we have something to delete
  QMutableListIterator<ToySyl> i(currentSyl);
  while (i.hasNext()) {
    if (i.next().stop <= nb) {
      i.remove();
      emitchange = true;
    }
  }
  QMutableListIterator<ToyText> j(currentText);
  while (j.hasNext()) {
    if (j.next().stop <= nb) {
      j.remove();
      emitchange = true;
    }
  }
  // update the currentSyl and currentText
  for (std::set<ToySyl>::iterator iter = syl.begin(); iter != syl.end();) {
    if (nb < (*iter).start)
      break;
    emitchange = true;
    currentSyl << (*iter);
    syl.erase(iter++);
  }
  for (std::set<ToyText>::iterator iter = text.begin(); iter != text.end();) {
    if (nb < (*iter).start)
      break;
    emitchange = true;
    currentText << (*iter);
    text.erase(iter++);
  }
  if (emitchange) {
    emit subChanged();
  }
}

// Enjoy the bgra format..

QColor  ToyStream::toycolor2QColor(QString toycolor)
{
  QChar a, b;
  a = toycolor[0];
  b = toycolor[1];
  toycolor[0] = toycolor[4];
  toycolor[1] = toycolor[5];
  toycolor[4] = a;
  toycolor[5] = b;
  if (toycolor.size() == 6) {
    return QColor(QString("#" + toycolor));
  }
  QString alp;
  alp[0] = toycolor[6];
  alp[1] = toycolor[7];
  toycolor.remove(6, 2);
  QColor toret("#" + toycolor);
  toret.setAlpha(alp.toInt((bool*) 0, 16));
  return toret;
}

