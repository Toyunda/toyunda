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

#include <QApplication>
#include <QObject>
#include <QTextStream>
#include "fakeplayer.h"
#include "rawsubstream.h"
#include "debugrenderer.h"

int	main(int ac, char *ag[])
{
  QApplication app(ac, ag);
  FakePlayer fp;
  DebugRenderer dr;
  QString tickInter("50");

  if (ac != 2 and ac != 3) {
    QTextStream cerr(stderr);
    cerr << "Argument required, syntax :\n" << ag[0] << " subfile [tick Interval]\n";
    return 1;
  }
  if (ac == 3)
    tickInter = ag[2];
  RawSubStream ts(ag[1]);

  QObject::connect(&fp, SIGNAL(frameChanged(int)), &ts, SLOT(setCurrentFrame(int)));
  QObject::connect(&ts, SIGNAL(currentSubChanged(void)), &dr, SLOT(renderUpdate(void)));
  dr.setToyundaSubStream(&ts);
  QStringList l;
  l << tickInter;
  fp.init(l);
  fp.play();
  return app.exec();
}
