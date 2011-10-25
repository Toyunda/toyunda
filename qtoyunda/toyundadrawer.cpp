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

#include <QList>
#include <QPoint>
#include "toyundadrawer.h"
 
ToyundaDrawer::ToyundaDrawer()
{
  m_width = ToyundaWidth;
  m_height = ToyundaHeight;
  horizontalRatio = 1.0;
  verticalRatio = 1.0;
  lineIntervalRatio = 1.0;
}

void	ToyundaDrawer::setRatio(const float hRat, const float vRat)
{
  m_width = ToyundaWidth * hRat;
  horizontalRatio = hRat;
  m_height = ToyundaHeight * vRat;
  verticalRatio = vRat;
  fixRatio();
}

void	ToyundaDrawer::fixRatio()
{
  lineInterval = int (m_height / 12) * lineIntervalRatio;
}

void	ToyundaDrawer::setRatio(const float ratio)
{
  setRatio(ratio, ratio);
}

void	ToyundaDrawer::setIntervalRatio(const float rat)
{
  lineIntervalRatio = rat;
  fixRatio();
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
  //qDebug() << newFont;
  QFontMetrics fm(s_font);
  s_font.setBold(true);
  letterWidth = fm.size(Qt::TextSingleLine, "l").width();
  letterHeight = fm.size(Qt::TextSingleLine, "l").height();
  maxLetterNumber = int (m_width / letterWidth);
  qDebug() << "font key : " << s_font.key();
  qDebug() << "Letter height : " << letterHeight <<  "Letter width : " << letterWidth;
}

void	ToyundaDrawer::setLogo(const QImage img, double imgRatio = 0.8)
{
  toyundaLogo = img;
  toyundaLogo = toyundaLogo.scaled(lineInterval * imgRatio, lineInterval * imgRatio);
}

void    ToyundaDrawer::drawGrid(QPainter &painter) const
{
  for (unsigned int i = 0; i < 12; i++) {
    painter.drawLine(0, i * lineInterval, m_width, i * lineInterval);
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
      if (tmp.posx == -1 || (tmp.posy == 0 && tmp.pipeNumber != 0)) {
        QPoint s;
        s.setY((tmp.pipeNumber + 1) * lineInterval);
	unsigned int tmpLetterWidth = letterWidth;
        // Handle long text
        if ((unsigned) tmp.text.size() > maxLetterNumber) {
          QFont cfont = s_font;
          cfont.setPointSize(s_font.pointSize() * (float (maxLetterNumber) / tmp.text.size()));
          painter.setFont(cfont);
	 
	  tmpLetterWidth = painter.fontMetrics().size(Qt::TextSingleLine, "l").width();
	}
	if (tmp.posx != -1)
          s.setX(tmp.posx);
	else
          s.setX((m_width - tmp.text.size() * tmpLetterWidth) / 2);
	painter.save();
	painter.setPen(QColor(0, 0, 0, painter.pen().color().alpha()));
        painter.drawText(s.x() + 1, s.y() + 1, tmp.text);
	painter.restore();
        painter.drawText(s, tmp.text);
	// Restore font in case of long text
        if ((unsigned) tmp.text.size() > maxLetterNumber)
	  painter.setFont(s_font);        
      } else {
        painter.drawText((int) (tmp.posx * horizontalRatio), lineInterval + (int )(tmp.posy * verticalRatio), tmp.text);
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
      s.setY((tmp.pipeNumber) * lineInterval + (lineInterval - toyundaLogo.size().height()));
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
