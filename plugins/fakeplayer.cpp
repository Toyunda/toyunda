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

#include <QDebug>
#include "fakeplayer.h"
#include "qplugin.h"


FakePlayer::FakePlayer()
{
  setIdentifier("fake");
  addOption("interval", 50, "The time in mlsec between two tick");
  addOption("start", 0, "The start time in frame number");
  addOption("duration", 0, "The duration of the fake stream");
}

bool	FakePlayer::init(const QStringList opt)
{
  handleOption(opt);
  interval = optionValue["interval"].toInt();
  frameNumber = optionValue["start"].toInt();
  qDebug() << "Interval : " << interval << " -- Start : " << frameNumber;
  return true;
}

void	FakePlayer::open(const QString file)
{
    frameNumber = optionValue["start"].toInt();
}

void	FakePlayer::seek(const int toseek)
{
  frameNumber += toseek;
  emit frameChanged(frameNumber);
}

void	FakePlayer::play()
{
  duration = optionValue["duration"].toInt();
  static bool first = true;
  if (first)
            connect(&timer, SIGNAL(timeout()), this, SLOT(newTick()));
  first = false;
  emit played();
  timer.start(interval);
}

void	FakePlayer::stop()
{
  timer.stop();
  emit stopped();
}

void	FakePlayer::newTick()
{
 //   qDebug() << frameNumber;
    emit frameChanged(frameNumber++);
    if (duration != 0 && frameNumber * interval >= duration) {
        emit finished();
        timer.stop();
    }
}

FilePlayer* FakePlayer::getMe()
{
    return this;
}


void FakePlayer::dispose()
{
    ;
}

Q_EXPORT_PLUGIN2(qtoyunda_fakeplayer, FakePlayer)
