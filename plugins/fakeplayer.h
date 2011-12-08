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

#ifndef H_FAKEPLAYER
#define H_FAKEPLAYER

#include <QTimer>
#include "fileplayer.h"
#include "abstractfileplayer.h"

/** @class FakePlayer
* @brief a fake player using a timer
*
* This is a fake player that use a QTimer to simulate the progress of video frame
*/

class FakePlayer : public FilePlayer, public AbstractFilePlayer
{
  Q_OBJECT
  Q_INTERFACES(AbstractFilePlayer)
  public:
    FakePlayer();

    bool	init(const QStringList opt);
    void        dispose();
    FilePlayer  *getMe();
    bool        hasVolumeControl();

  public slots:
    void	open(const QString);
    void	play();
    void	stop();
    void	seek(const int);
    void        setVolume(uint);

  private slots:
    void	newTick();

  private:
    unsigned int interval;
    QTimer	timer;
    int         duration;
    int		frameNumber;
};

#endif
