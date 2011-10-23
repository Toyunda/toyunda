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

#include <QDebug>
#include "fakeplayer.h"
#include "qplugin.h"


FakePlayer::FakePlayer() : FilePlayer()
{
  setIdentifier("fake");
  addOption("interval", QString("50"), "The time in mlsec between two tick");
  addOption("start", 0, "The start time in frame number");
}

bool	FakePlayer::init(const QStringList opt)
{
  handleOption(opt);
  interval = optionValue["interval"].toInt();
  frameNumber = optionValue["start"].toInt();
  return true;
}

void	FakePlayer::open(const QString file)
{
  
}

void	FakePlayer::seek(const int toseek)
{
  frameNumber += toseek;
  emit frameChanged(frameNumber);
}

void	FakePlayer::play()
{
  connect(&timer, SIGNAL(timeout()), this, SLOT(newTick()));
  //emit played();
  timer.start(interval);
}

void	FakePlayer::stop()
{
  timer.stop();
  emit stopped();
}

void	FakePlayer::newTick()
{
    emit frameChanged(frameNumber++);
}

FilePlayer* FakePlayer::getMe()
{
    return this;
}

Q_EXPORT_PLUGIN2(qtoyunda_fakeplayer, FakePlayer)
