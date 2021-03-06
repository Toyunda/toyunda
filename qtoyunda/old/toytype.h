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

#ifndef H_PLOPT
#define H_PLOPT
#include <QString>
#include <QColor>

struct ToyText {
  int		start;
  int		stop;
  int		nbpipe;
  int		posx;
  int		posy;
  QString	text;
  QColor	color1;
  QColor	color2;
  QColor	tmpcolor;
};

struct ToySyl {
  int	start;
  int	stop;
  int	nbpipe;
  int	length;
  int	pos;
};

bool    operator<(ToyText, ToyText);
bool    operator<(ToySyl, ToySyl);
void	show_ToyText(ToyText);
void	show_ToySyl(ToySyl);


#endif
