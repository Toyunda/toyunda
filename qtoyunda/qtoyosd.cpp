#include <QDebug>
#include <QWidget>
#include <QList>
#include <QPainter>
#include <iostream>
#include "qtoyosd.h"

QToyOSD::QToyOSD(QWidget *parent) : QWidget(parent)
{
  fontSize = 18;
  toyundaHeight = 600;
  toyundaWidth = 800;
  ratio = 1;
  QFont f("Monospace", fontSize);
  setFont(f);
  QFontMetrics fm(font());
  letterw = fm.width("a");
  //spacingSize = fm.width("aa") - letterw * 2;
  letterh = fm.height();
  QImage origtoylogo("Toyunda logo.png");
  toylogo = origtoylogo.scaled(letterw * 1.5, letterw * 1.5);
  f.setFixedPitch(true);
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  //setAttribute(Qt::WA_NoSystemBackground, true);
  //setWindowOpacity(0.1);
  setFixedSize(QSize(toyundaWidth * ratio, toyundaHeight * ratio));
}

void	QToyOSD::setStream(ToyStream *s)
{
  toys = s;
}

void	QToyOSD::paintEvent(QPaintEvent *event)
{
  int h = toyundaHeight * ratio;
  int w = toyundaWidth * ratio;
  QPainter painter(this);
  QList<ToyText> tmpt = toys->getCurrentText();
  QList<ToySyl> tmps = toys->getCurrentSyl();
  // Text
  if (tmpt.isEmpty() == false) {
    QListIterator<ToyText> ittext(tmpt);
    while (ittext.hasNext()) {
      ToyText tmp = ittext.next();
      if (tmp.color1.isValid()) {
        if (tmp.tmpcolor.isValid()) {
	  painter.setPen(tmp.tmpcolor);
	} else {
          painter.setPen(tmp.color1);
	}
      } else {
        painter.setPen(Qt::darkBlue);
      }
      if (tmp.posx == -1) {
        //QPoint s((w - (tmp.text.size() * letterw) - (tmp.text.size() - 1) * spacingSize) / 2,
        QPoint s((w - tmp.text.size() * letterw) / 2,
        30 + tmp.nbpipe * letterh * 2);
        painter.drawText(s, tmp.text);
      } else {
        painter.drawText((int) tmp.posx / ratio, (int) tmp.posy / ratio, tmp.text);
      }
    }
  }
  // Syl
  if (tmps.isEmpty() == false) {
    QListIterator<ToySyl> itsyl(tmps);

    while (itsyl.hasNext()) {
      ToySyl tmp = itsyl.next();
      painter.setPen(Qt::black);
      //QPoint s((w - tmp.length * letterw - (tmp.length - 1) * spacingSize) / 2 + tmp.pos * (letterw + spacingSize),
      QPoint s((w - tmp.length * letterw) / 2 + tmp.pos * letterw,
      letterh + (tmp.nbpipe - 1) * letterh * 2);
      painter.drawImage(s, toylogo);
      //painter.drawText(s, "n");
    }
  }
}

