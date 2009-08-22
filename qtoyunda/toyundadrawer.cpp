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
  m_width = ToyundaWidth;
  m_height = ToyundaHeight;
  horizontalRatio = 1.0;
  verticalRatio = 1.0;
}

void	ToyundaDrawer::setRatio(const float hRat, const float vRat)
{
  m_width = ToyundaWidth * hRat;
  horizontalRatio = hRat;
  m_height = ToyundaHeight * vRat;
  verticalRatio = vRat;
}

void	ToyundaDrawer::setRatio(const float ratio)
{
  setRatio(ratio, ratio);
}

unsigned int ToyundaDrawer::width() const
{
  return m_width;
}

unsigned int ToyundaDrawer::height() const
{
  return m_height;
}

void	ToyundaDrawer::setFont(const QFont newFont)
{
  s_font = newFont;
  qDebug() << newFont;
  QFontMetrics fm(s_font);
  s_font.setBold(true);
  letterWidth = fm.size(Qt::TextSingleLine, "l").width();
  letterHeight = fm.size(Qt::TextSingleLine, "l").height();
  maxLetterNumber = int (m_width / letterWidth);
  qDebug() << letterHeight << letterWidth;
}

void	ToyundaDrawer::setLogo(const QImage img)
{
  toyundaLogo = img;
  toyundaLogo = toyundaLogo.scaled(letterHeight * 0.8, letterHeight * 0.8);
}

void    ToyundaDrawer::drawGrid(QPainter &painter) const
{
  for (unsigned int i = 0; i < m_width / letterHeight; i++) {
    painter.drawLine(0, i * letterHeight, m_width, i * letterHeight);
  }
}

void	ToyundaDrawer::draw(QPainter &painter, const QList<ToyundaText> &textSub, const QList<ToyundaSyl> &sylSub) const
{
  painter.setFont(s_font);
  drawGrid(painter);
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
        QPoint s;
        s.setY((tmp.pipeNumber + 1) * letterHeight * verticalRatio);
        // Handle long text
        if ((unsigned) tmp.text.size() > maxLetterNumber) {
          QFont cfont = s_font;
          //qDebug() << maxLetterNumber << tmp.text.size();
          //qDebug() << (float (maxLetterNumber) / tmp.text.size());
          //cfont.setStretch(100 * (float (maxLetterNumber) / tmp.text.size()));
          cfont.setPointSize(s_font.pointSize() * (float (maxLetterNumber) / tmp.text.size()));
          painter.setFont(cfont);
          s.setX((m_width - tmp.text.size() * painter.fontMetrics().size(Qt::TextSingleLine, "l").width()) / 2);
          painter.drawText(s, tmp.text);
          painter.setFont(s_font);
        } else {
          s.setX((m_width - tmp.text.size() * letterWidth) / 2);
          painter.drawText(s, tmp.text);
        }
      } else {
        painter.drawText((int) (tmp.posx * horizontalRatio), letterHeight + (int )(tmp.posy * verticalRatio), tmp.text);
      }
    }
  }
  // Syl
  if (sylSub.isEmpty() == false) {
    QListIterator<ToyundaSyl> itsyl(sylSub);

    while (itsyl.hasNext()) {
      ToyundaSyl tmp = itsyl.next();
      painter.setPen(Qt::black);
      QPoint s;
      s.setY((tmp.pipeNumber) * letterHeight + (letterHeight - toyundaLogo.size().height()));
      if ((unsigned) tmp.length > maxLetterNumber) {
          QFont cfont = s_font;
          cfont.setPointSize(s_font.pointSize() * (float (maxLetterNumber) / tmp.length));
          painter.setFont(cfont);
          s.setX((m_width - tmp.length * painter.fontMetrics().size(Qt::TextSingleLine, "l").width()) / 2 +
          tmp.pos * painter.fontMetrics().size(Qt::TextSingleLine, "l").width());
          painter.drawImage(s, toyundaLogo);
          //painter.drawText(s, "N");
          painter.setFont(s_font);
        } else {
          s.setX((m_width - tmp.length * letterWidth) / 2 + tmp.pos * letterWidth);
          //painter.drawText(s, "N");
          painter.drawImage(s, toyundaLogo);
        }
    }
  }
}
