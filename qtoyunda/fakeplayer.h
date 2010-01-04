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

#ifndef H_FAKEPLAYER
#define H_FAKEPLAYER

#include <QTimer>
#include "fileplayer.h"

/** @class FakePlayer
* @brief a fake player using a timer
*
* This is a fake player that use a QTimer to simulate the progress of video frame
*/

class FakePlayer : public FilePlayer
{
  Q_OBJECT;
  public:
    FakePlayer();

    bool	init(const QStringList opt);

  public slots:
    void	open(const QString);
    void	play();
    void	stop();
    void	seek(const int);

  private slots:
    void	newTick();

  private:
    unsigned int interval;
    QTimer	timer;
    int		frameNumber;
};

#endif
