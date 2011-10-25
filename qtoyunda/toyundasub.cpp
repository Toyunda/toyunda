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

#include "toyundasub.h"


const unsigned int ToyundaHeight = 600;
const unsigned int ToyundaWidth = 800;

const unsigned int ToyundaFontSize = 30;
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


bool    operator==(const ToyundaText t1, const ToyundaText t2)
{
  bool toret = false;
  toret = (t1.start == t2.start);
  toret = toret && (t1.stop == t2.stop);
  toret = toret && (t1.pipeNumber == t2.pipeNumber);
  toret = toret && (t1.posy == t2.posx);
  toret = toret && (t1.posy == t2.posy);
  toret = toret && (t1.size == t2.size);
  toret = toret && (t1.text == t2.text);
  toret = toret && (t1.color1 == t2.color1);
  toret = toret && (t1.color2 == t2.color2);
  toret = toret && (t1.fadingcolor == t2.fadingcolor);
  return toret;
}

bool    operator==(const ToyundaSyl s1, const ToyundaSyl s2)
{
  bool toret = false;
  toret = (s1.start == s2.start);
  toret = toret && (s1.stop == s2.stop);
  toret = toret && (s1.pipeNumber == s2.pipeNumber);
  toret = toret && (s1.length == s2.length);
  toret = toret && (s1.pos == s2.pos);
  return toret;
}


QDataStream	&operator<<(QDataStream &ts, const ToyundaText &tt)
{
  printToyundaText(ts, tt);
  return ts;
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


QDataStream	&operator<<(QDataStream &ts, const ToyundaSyl &tsyl)
{
  printToyundaSyl(ts, tsyl);
  return ts;
}


QDebug		operator<<(QDebug dbg, const ToyundaText &tt)
{
  printToyundaText(dbg.nospace(), tt);
  return dbg;
}


QDebug		operator<<(QDebug dbg, const ToyundaSyl &ts)
{
  printToyundaSyl(dbg.nospace(), ts);
  return dbg;
}


// Enjoy the bgra format
QColor  toyundaColor2QColor(QString toycolor)
{
  QChar a, b;
  QString alp;
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

