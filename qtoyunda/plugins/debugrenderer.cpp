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

#include "../toyundasub.h"
#include <qplugin.h>
#include "debugrenderer.h"

DebugRenderer::DebugRenderer() : QObject()
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

void DebugRenderer::hide()
{
}

void DebugRenderer::show()
{
}


ToyundaRenderer * DebugRenderer::getMe()
{
    return this;
}

void DebugRenderer::dispose()
{
    return;
}


Q_EXPORT_PLUGIN2(qtoyunda_debugrenderer, DebugRenderer)

