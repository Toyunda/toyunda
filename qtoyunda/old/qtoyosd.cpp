/*
Copyright (C) 2008 - Sylvain Colinet
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <QtGui>
#include <QDebug>
#include <QWidget>
#include <QList>
#include <QPainter>
#include <iostream>
#include "qtoyosd.h"

QToyOSD::QToyOSD(QWidget *parent) : QWidget(parent)
{
  fontSize = 18;
  toyundaHeight = 300;
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
  setAttribute(Qt::WA_NoSystemBackground, true);
  //setAutoFillBackground(true);
  /*QPixmap pix("fond.png");
  setMask(pix.mask());*/
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
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setClipRect(event->rect());

    //make sure you clean your widget with a transparent
    //  color before doing any rendering
    //  note the usage of a composition mode Source
    //  it's important!
  painter.save();
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(rect(), Qt::transparent);
  painter.restore();

  //painter.setBackground(Qt::transparent);
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
        30 + tmp.nbpipe * letterh);
        painter.drawText(s, tmp.text);
      } else {
        painter.drawText((int) tmp.posx / ratio, (int )((int) tmp.posy / ratio) / 2, tmp.text);
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
      letterh + (tmp.nbpipe - 1) * letterh);
      painter.drawImage(s, toylogo);
      //painter.drawText(s, "n");
    }
  }
}

