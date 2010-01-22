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

#ifndef H_PTOYUNDASUB
#define H_PTOYUNDASUB
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QColor>


extern const unsigned int ToyundaHeight;
extern const unsigned int ToyundaWidth;

extern const unsigned int ToyundaFontSize;
extern const QString  ToyundaFontName;

struct ToyundaText {
  int		start;
  int		stop;
  int		pipeNumber;
  int		posx;
  int		posy;
  int		size;
  QString	text;
  QColor	color1;
  QColor	color2;
  QColor	fadingcolor;
};

struct ToyundaSyl {
  int	start;
  int	stop;
  int	pipeNumber;
  int	length;
  int	pos;
};

bool    operator<(const ToyundaText, const ToyundaText);
bool    operator<(const ToyundaSyl, const ToyundaSyl);

QDebug	operator<<(QDebug, const ToyundaText&);
QDebug	operator<<(QDebug, const ToyundaSyl&);

QTextStream	&operator<<(QTextStream &, const ToyundaText &);
QTextStream	&operator<<(QTextStream &, const ToyundaSyl &);

QDataStream	&operator<<(QDataStream &, const ToyundaText &);
QDataStream	&operator<<(QDataStream &, const ToyundaSyl &);
bool    operator==(const ToyundaText t1, const ToyundaText t2);
bool    operator==(const ToyundaSyl s1, const ToyundaSyl s2);

template<typename T> void printToyundaText(T &stream, const ToyundaText &);
template<typename T> void printToyundaSyl(T &stream, const ToyundaSyl &);

QColor	toyundaColor2QColor(QString toycolor);

#include "toyundasub.hxx"
#endif
