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

#include <QTextStream>
#include <iostream>
#include "toytype.h"

extern QTextStream cout;

bool    operator<(ToyText toy, ToyText toy2)
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

bool    operator<(ToySyl syl, ToySyl syl2)
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

void    show_ToyText(ToyText t)
{
  std::cout << "start : " << t.start << "\n";
  std::cout << "stop : " << t.stop << "\n";
  std::cout << "text : " << t.text.toStdString() << "\n";
  std::cout << "nbpipe : " << t.nbpipe << "\n";
}

void    show_ToySyl(ToySyl t)
{
  std::cout << "start : " << t.start << "\n";
  std::cout << "stop : " << t.stop << "\n";
  std::cout << "pos : " << t.pos << "\n";
  std::cout << "length : " << t.length << "\n";
  std::cout << "nbpipe : " << t.nbpipe << "\n";
}

