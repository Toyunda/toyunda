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

#include <QList>
#include <QPoint>
#include "toyundadrawer.h"
 
ToyundaDrawer::ToyundaDrawer()
{
  width = ToyundaWidth;
  height = ToyundaHeight;
  horizontalRatio = 1.0;
  verticalRatio = 1.0;
}

void	ToyundaDrawer::setRatio(const float hRat, const float vRat)
{
  width = ToyundaWidth * hRat;
  horizontalRatio = hRat;
  height = ToyundaHeight * vRat;
  verticalRatio = vRat;
}

void	ToyundaDrawer::setRatio(const float ratio)
{
  setRatio(ratio, ratio);
}

void	ToyundaDrawer::setFont(const QFont newFont)
{
  font = newFont;
  QFontMetrics fm(font);
  letterWidth = fm.width("a");
  letterHeight = fm.height();
}

void	ToyundaDrawer::setLogo(const QImage img)
{
  toyundaLogo = img;
}

void	ToyundaDrawer::draw(QPainter &painter, const QList<ToyundaText> &textSub, const QList<ToyundaSyl> &sylSub) const
{
  painter.setFont(font);
  if (textSub.isEmpty() == false) {
    QListIterator<ToyundaText> ittext(textSub);
    while (ittext.hasNext()) {
      ToyundaText tmp = ittext.next();
      if (tmp.color1.isValid()) {
        if (tmp.fadingcolor.isValid()) {
          painter.setPen(tmp.fadingcolor);
        } else {
          painter.setPen(tmp.color1);
        }
      } else {
        painter.setPen(Qt::darkBlue);
      }
      if (tmp.posx == -1) {
        //QPoint s((w - (tmp.text.size() * letterWidth) - (tmp.text.size() - 1) * spacingSize) / 2,
        QPoint s((width - tmp.text.size() * letterWidth) / 2,
        (tmp.pipeNumber + 1) * letterHeight * verticalRatio);
        painter.drawText(s, tmp.text);
      } else {
        painter.drawText((int) (tmp.posx * horizontalRatio), (int )(tmp.posy * verticalRatio), tmp.text);
      }
    }
  }
  // Syl
  if (sylSub.isEmpty() == false) {
    QListIterator<ToyundaSyl> itsyl(sylSub);

    while (itsyl.hasNext()) {
      ToyundaSyl tmp = itsyl.next();
      painter.setPen(Qt::black);
      //QPoint s((w - tmp.length * letterWidth - (tmp.length - 1) * spacingSize) / 2 + tmp.pos * (letterWidth + spacingSize),
      QPoint s((width - tmp.length * letterWidth) / 2 + tmp.pos * letterWidth,
      letterHeight + (tmp.pipeNumber - 1) * letterHeight);
      //painter.drawImage(s, toyundaLogo);
      painter.drawText(s, "n");
    }
  }
}
