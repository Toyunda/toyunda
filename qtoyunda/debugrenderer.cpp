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
#include "debugrenderer.h"

DebugRenderer::DebugRenderer()
{
  setIdentifier("debug");
}

void	DebugRenderer::renderUpdate()
{
  QList<ToyundaText> subText = toySubStream->getCurrentTextSubtitle();
  QList<ToyundaSyl> subSyl = toySubStream->getCurrentSylSubtitle();

  QTextStream cout(stdout);
  QTextStream cerr(stderr);
  if (subText.isEmpty()) {
    cout << "no current text to display\n";
  } else {
    cout << "----" << toySubStream->getCurrentFrame() << "----\n";
    QListIterator<ToyundaText> itText(subText);
    while (itText.hasNext())
    {
      cout << (ToyundaText) itText.next();
    }
  }
  cout << "== Syl ==" << "\n";
  if (subSyl.isEmpty()) {
    cout << "no current Syl to display\n";
  } else {
    QListIterator<ToyundaSyl> itSyl(subSyl);
    while (itSyl.hasNext())
    {
      cout << (ToyundaSyl) itSyl.next();
    }
  }
}
