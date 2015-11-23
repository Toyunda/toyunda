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

#ifndef H_TOYUNDADRAWER
#define H_TOYUNDADRAWER

#include <QPainter>
#include "toyundasub.h"

class ToyundaDrawer
{
  public:
    ToyundaDrawer();
    void	draw(QPainter &painter, const QList<ToyundaText> &textSub, const QList<ToyundaSyl> &sylSub) const;
    void	setRatio(const float hRat, const float vRat);
    void	setRatio(const float ratio);
    void	setIntervalRatio(const float rat);
    void	setFont(const QFont nFont);
    void	setLogo(const QImage img, double logoratio);
    unsigned int width() const;
    unsigned int height() const;
    void        recalcMagic();
    void        drawGrid(QPainter &painter) const;
  private:
    unsigned int	m_width;
    unsigned int	m_height;
    unsigned int	letterWidth;
    unsigned int	letterHeight;
    unsigned int        maxLetterNumber;
    unsigned int	lineInterval;
    float		lineIntervalRatio;
    float               imageRatio;
    QImage		toyundaLogo;
    QImage		originalLogo;
    QFont		s_font;
    float	horizontalRatio;
    float	verticalRatio;
    void	fixRatio();
};

#endif
