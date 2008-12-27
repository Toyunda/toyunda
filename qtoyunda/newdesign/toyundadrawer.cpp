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

#include "toyundadrawer.h"
 
ToyundaDrawer::ToyundaDrawer()
{
  ;
}

ToyundaDrawer::draw(QPainter &painter, const QList<ToyundaText> &textSub, const QList<ToyundaSyl> &sylSub)
{
  if (textSub.isEmpty() == false) {
    QListIterator<ToyText> ittext(textSub);
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
  if (sylSub.isEmpty() == false) {
    QListIterator<ToySyl> itsyl(sylSub);

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
