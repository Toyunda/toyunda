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

#ifndef H_VLCAUDIOPLAYER
#define H_VLCAUDIOPLAYER

#include <vlc/vlc.h>
#include "fileplayer.h"

class VLCAudioPlayer : public FilePlayer
{
	Q_OBJECT;

	public:
		VLCAudioPlayer();
		bool	init(const QStringList optionList = QStringList());
	public slots:
		void  open(const QString fileName);
		void  play();
		void  seek(const int step);
		void  stop();

	private slots:
		void  checkFrame();
	
	private:
		libvlc_media_player_t	*mediaPlayer;
		libvlc_exception_t	vlc_ex;
		libvlc_time_t		length;
		libvlc_instance_t	*inst;
		float	fps;
};


#endif