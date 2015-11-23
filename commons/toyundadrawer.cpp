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
  horizontalRatio = hRat;
  verticalRatio = vRat;
}

void	ToyundaDrawer::setRatio(const float ratio)
{
  setRatio(ratio, ratio);
}

void	ToyundaDrawer::setIntervalRatio(const float rat)
{
  lineIntervalRatio = rat;
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
  qDebug() << "new Font : " << newFont;
}

void	ToyundaDrawer::setLogo(const QImage img, double imgRatio = 0.8)
{
  toyundaLogo = img;
  originalLogo = img;
  imageRatio = imgRatio;
}


void ToyundaDrawer::recalcMagic()
{
    m_width = ToyundaWidth * horizontalRatio;
    m_height = ToyundaHeight * verticalRatio;
    lineInterval = int (m_height / 12) * lineIntervalRatio;

    toyundaLogo = originalLogo;
    toyundaLogo = toyundaLogo.scaled(lineInterval * imageRatio, lineInterval * imageRatio);

    QFontMetrics fm(s_font);
    s_font.setBold(true);
    s_font.setStyleHint(QFont::Monospace);
    letterWidth = fm.size(Qt::TextSingleLine, "o").width();
    letterHeight = fm.size(Qt::TextSingleLine, "lp").height();
    maxLetterNumber = int (m_width / letterWidth);
    qDebug() << "Size :     " << m_width << "x" << m_height;
    qDebug() << "Max Letter : " << maxLetterNumber;
    qDebug() << "font key : " << s_font.key();
    qDebug() << "Letter height : " << letterHeight <<  "Letter width : " << letterWidth;
}



void    ToyundaDrawer::drawGrid(QPainter &painter) const
{
  for (unsigned int i = 0; i < 12; i++) {
    painter.drawLine(0, i * lineInterval, m_width, i * lineInterval);
  }
  //for (unsigned int i = 0; i < maxLetterNumber)
}

void	ToyundaDrawer::draw(QPainter &painter, const QList<ToyundaText> &textSub, const QList<ToyundaSyl> &sylSub) const
{
    painter.setFont(s_font);
    drawGrid(painter);
    if (textSub.isEmpty() == false) {
        QListIterator<ToyundaText> ittext(textSub);
        while (ittext.hasNext())
        {
            const ToyundaText &tmp = ittext.next();
            // Set painter color, fading, fixed color or default
            if (tmp.color1.isValid()) {
                if (tmp.fadingcolor.isValid()) {
                    painter.setPen(tmp.fadingcolor);
                } else {
                    painter.setPen(tmp.color1);
                }
            } else {
                painter.setPen(Qt::darkBlue);
            }
            // Text with no position
            if ((tmp.posx == -1 && tmp.posy == -1) || (tmp.posy == 0 && tmp.pipeNumber != 0))
            {
                QPoint s;

                unsigned int tmpLetterWidth = letterWidth;
                // Handle long text
                if ((unsigned) tmp.text.size() >= maxLetterNumber) {
                    QFont cfont = s_font;

                    cfont.setPointSize((s_font.pointSize() * (float (maxLetterNumber) / tmp.text.size())));
                    painter.setFont(cfont);
                    tmpLetterWidth = painter.fontMetrics().size(Qt::TextSingleLine, "o").width();
                    // Correct font size too big, pointsize != metrics width
                    while (tmpLetterWidth * tmp.text.size() > m_width)
                    {
                        cfont.setPointSize(cfont.pointSize() - 1);
                        painter.setFont(cfont);
                        tmpLetterWidth = painter.fontMetrics().size(Qt::TextSingleLine, "o").width();
                    }
                }
                // Size
                if (tmp.size != -1) {
                    QFont cfont = s_font;
                    if (tmp.size2 == -1)
                        cfont.setPixelSize(tmp.size);
                    else
                        cfont.setPixelSize(tmp.intersize);
                    painter.setFont(cfont);
                    tmpLetterWidth = painter.fontMetrics().size(Qt::TextSingleLine, "o").width();
                }
                int plop = (lineInterval - painter.fontMetrics().size(Qt::TextSingleLine, "o").height()) / 2;
                qDebug() << "Piko:" << plop;
                s.setY((tmp.pipeNumber + 1) * lineInterval - plop);// + painter.fontMetrics().size(Qt::TextSingleLine, "o").height());
                s.setX((m_width - tmp.text.size() * tmpLetterWidth) / 2);
                painter.save();
                // Outline
                painter.setPen(QColor(0, 0, 0, painter.pen().color().alpha()));
                painter.drawText(s.x() + 1, s.y() + 1, tmp.text);
                painter.restore();
                painter.drawText(s, tmp.text);
                // Restore font in case of long text
                if ((unsigned) tmp.text.size() > maxLetterNumber || tmp.size != -1)
                    painter.setFont(s_font);
                // Set text at position pos
            } else {
                if (tmp.size != -1) {
                    QFont cfont = s_font;
                    if (tmp.size2 == -1)
                        cfont.setPixelSize(tmp.size);
                    else
                        cfont.setPixelSize(tmp.intersize);
                    painter.setFont(cfont);
                }
                QPoint s;
                // Yes the pipenumber change the y position. wtf...
                if (tmp.pos2x == -1)
                    s.setX((int) (tmp.posx * horizontalRatio));
                else
                    s.setX((int)(tmp.interposx * horizontalRatio));
                if (tmp.pos2y == -1)
                    s.setY(painter.fontMetrics().size(Qt::TextSingleLine, "o").height() + (int )((tmp.posy + (tmp.pipeNumber * lineInterval)) * verticalRatio));
                else
                    s.setY(painter.fontMetrics().size(Qt::TextSingleLine, "o").height() + (int )((tmp.interposy + (tmp.pipeNumber * lineInterval)) * verticalRatio));
                painter.save();
                // Outline
                painter.setPen(QColor(0, 0, 0, painter.pen().color().alpha()));
                painter.drawText(s.x() + 1, s.y() + 1, tmp.text);
                painter.restore();
                painter.drawText(s.x(), s.y(), tmp.text);

            }
            if (tmp.size != -1) {
                painter.setFont(s_font);
            }
        }
    }
    // Syl
    if (sylSub.isEmpty() == false) {
        QListIterator<ToyundaSyl> itsyl(sylSub);

        while (itsyl.hasNext()) {
            const ToyundaSyl &tmp = itsyl.next();
            painter.setPen(Qt::black);
            QPoint s;
            s.setY((tmp.pipeNumber) * lineInterval + (lineInterval - toyundaLogo.size().height()));
            unsigned int tmpLetterWidth = letterWidth;
            // Handle long text
            if ((unsigned) tmp.length >= maxLetterNumber) {
                QFont cfont = s_font;

                cfont.setPointSize((s_font.pointSize() * (float (maxLetterNumber) / tmp.length)));
                painter.setFont(cfont);
                tmpLetterWidth = painter.fontMetrics().size(Qt::TextSingleLine, "o").width();
                // Correct font size too big, pointsize != metrics width
                while (tmpLetterWidth * tmp.length > m_width)
                {
                    cfont.setPointSize(cfont.pointSize() - 1);
                    painter.setFont(cfont);
                    tmpLetterWidth = painter.fontMetrics().size(Qt::TextSingleLine, "o").width();
                }
                painter.setFont(s_font);
            }
            s.setX((m_width - tmp.length * tmpLetterWidth) / 2 + tmp.pos * tmpLetterWidth);
            // Fading !
            if (tmp.alpha != -1)
            {
                if (tmp.alpha2 != -1)
                {
                    QImage cpy = toyundaLogo;
                    QPainter painterimg(&cpy);
                    painterimg.setCompositionMode(QPainter::CompositionMode_DestinationIn);
                    painterimg.fillRect(cpy.rect(), QColor(0, 0, 0, tmp.fadingalpha));
                    painter.drawImage(s, cpy);
                }
                else
                {
                    QImage cpy = toyundaLogo;
                    QPainter painterimg(&cpy);
                    painterimg.setCompositionMode(QPainter::CompositionMode_DestinationIn);
                    painterimg.fillRect(cpy.rect(), QColor(0, 0, 0, tmp.alpha));
                    painter.drawImage(s, cpy);
                }
            } else
                painter.drawImage(s, toyundaLogo);
        }
    }
}

