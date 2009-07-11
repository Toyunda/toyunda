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
    void	setFont(const QFont nFont);
    void	setLogo(const QImage img);
  private:
    unsigned int	width;
    unsigned int	height;
    unsigned int	letterWidth;
    unsigned int	letterHeight;
    unsigned int        maxLetterNumber;
    QImage		toyundaLogo;
    QFont		s_font;
    float	horizontalRatio;
    float	verticalRatio;
};

#endif
