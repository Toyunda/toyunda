#include <QDebug>
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

 
  std::cout << nb << std::endl;
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
    ToyText &tmp = j.next();
    if (tmp.stop <= nb) {
      j.remove();
      emitchange = true;
    } 
    else {
      if (tmp.color1.isValid() and tmp.color2.isValid()) {
	int r1, r2, g1, g2, b1, b2, a1, a2;
	#define MYABS(x) ((x) < 0 ? - (x) : (x))
	float rap = (float) (nb - tmp.start) / (float) (tmp.stop - tmp.start);
	r1 = tmp.color1.red();
	r2 = tmp.color2.red();
	b1 = tmp.color1.blue();
	b2 = tmp.color2.blue();
	g1 = tmp.color1.green();
	g2 = tmp.color2.green();
	a1 = tmp.color1.alpha();
	a2 = tmp.color2.alpha();
	tmp.tmpcolor.setRed(r1 + rap * (r2 - r1));
	tmp.tmpcolor.setBlue(b1 + rap * (b2 - b1));
	tmp.tmpcolor.setGreen(g1 + rap * (g2 - g1));
	tmp.tmpcolor.setAlpha(a1 + rap * (a2 - a1));
	emitchange = true;
      }
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

// Enjoy the abgr format..

QColor  ToyStream::toycolor2QColor(QString toycolor)
{
  QChar a, b;
  QString alp;
  if (toycolor.size() == 8) {
    alp[0] = toycolor[0];
    alp[1] = toycolor[1];
    toycolor.remove(0, 2);
  }
  a = toycolor[0];
  b = toycolor[1];
  toycolor[0] = toycolor[4];
  toycolor[1] = toycolor[5];
  toycolor[4] = a;
  toycolor[5] = b;
  QColor toret("#" + toycolor);
  if (alp.isEmpty() == false)
    toret.setAlpha(alp.toInt((bool*) 0, 16));
  return toret;
}

