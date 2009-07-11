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

#include "toyundasub.h"


const unsigned int ToyundaHeight = 600;
const unsigned int ToyundaWidth = 800;

const unsigned int ToyundaFontSize = 28;
const QString  ToyundaFontName = "Bitstream Vera Sans Mono Bold";


bool    operator<(const ToyundaText toy, const ToyundaText toy2)
{
  if (toy.start > toy2.start)
    return false;
  else if (toy.start == toy2.start)
  {
    if (toy.stop > toy2.stop)
      return false;
    else if (toy.stop < toy2.stop)
      return true;
    else
      return true;
  }
  else
    return true;
}

bool    operator<(const ToyundaSyl syl, const ToyundaSyl syl2)
{
  if (syl.start > syl2.start)
    return false;
  else if (syl.start == syl2.start)
  {
    if (syl.stop > syl2.stop)
      return false;
    else if (syl.stop < syl2.stop)
      return true;
    else
      return true;
  }
  else
    return true;
}


QTextStream	&operator<<(QTextStream &ts, const ToyundaText &tt)
{
  printToyundaText(ts, tt);
  return ts;
}

QTextStream	&operator<<(QTextStream &ts, const ToyundaSyl &tsyl)
{
  printToyundaSyl(ts, tsyl);
  return ts;
}

QDebug		operator<<(QDebug dbg, const ToyundaText &tt)
{
  printToyundaText(dbg.nospace(), tt);
  return dbg;
}

template<class T> void printToyundaText(T &stream, const ToyundaText &tt)
{
  stream << "Text : " << tt.text << "\n";
  stream << "Frame  : (" << tt.start << ", " << tt.stop << ")\n";
  stream << "Number of pipe : " << tt.pipeNumber << "\n";
  stream << "Position : (" << tt.posx << ", " << tt.posy << ")\n";
  stream << "Size : " << tt.size << "\n";
  stream << "Colors : -- " << tt.color1.red() << 
                    "," << tt.color1.green() <<
                    "," << tt.color1.blue() <<
                    "," << tt.color1.alpha() << "\n";
  if (tt.color2.isValid()) {
    stream << " -- " << tt.color2.red() << 
                    "," << tt.color2.green() <<
                    "," << tt.color2.blue() <<
                    "," << tt.color2.alpha();
    stream << " -- " << tt.fadingcolor.red() << 
                    "," << tt.fadingcolor.green() <<
                    "," << tt.fadingcolor.blue() <<
                    "," << tt.fadingcolor.alpha() << "\n";
  }
}


QDebug		operator<<(QDebug dbg, const ToyundaSyl &ts)
{
  printToyundaSyl(dbg.nospace(), ts);
  return dbg;
}

template<class T> void  printToyundaSyl(T &stream, const ToyundaSyl &ts)
{
  stream << "Frame  : (" << ts.start << ", " << ts.stop << ")\n";
  stream << "Number of pipe : " << ts.pipeNumber << "\n";
  stream << "Position : " << ts.pos << "\n";
}

// Enjoy the bgra format
QColor  toyundaColor2QColor(QString toycolor)
{
  QChar a, b;
  QString alp;
  qDebug() << toycolor;
  if (toycolor.size() == 8) {
    alp[0] = toycolor[0];
    alp[1] = toycolor[1];
    toycolor.remove(0, 2);
  }
  a = toycolor[0];
  b = toycolor[1];
  toycolor[0] = toycolor[4];
  toycolor[1] = toycolor[5];
  toycolor[4] = a;
  toycolor[5] = b;
  QColor toret("#" + toycolor);
  if (alp.isEmpty() == false)
    toret.setAlpha(alp.toInt((bool*) 0, 16));
  return toret;
}

