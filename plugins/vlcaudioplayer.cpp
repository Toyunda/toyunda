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

#include <QTimer>
#include <QDebug>
#include "vlcaudioplayer.h"


VLCAudioPlayer::VLCAudioPlayer() : FilePlayer()
{
	setIdentifier("vlcaudio");
	addOption("ao", "alsa", "Audio output driver");
}

bool	VLCAudioPlayer::init(QStringList opt)
{
	handleOption(opt);
	const char * const vlc_args[] = {
              "-I", "dummy", "-A", optionValue["ao"].toString().toStdString().c_str(), /* Don't use any interface */
             "--ignore-config", "--no-video"}; /* Don't use VLC's config */

	libvlc_event_manager_t	*em;
	libvlc_exception_init(&vlc_ex);
	inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args, &vlc_ex);
	mediaPlayer = libvlc_media_player_new(inst, &vlc_ex);
	em = libvlc_media_player_event_manager(mediaPlayer, &vlc_ex);
	return true;
}

void	VLCAudioPlayer::open(QString filename)
{
	libvlc_media_t *m = libvlc_media_new(inst, filename.toStdString().c_str(), &vlc_ex);
	libvlc_media_player_set_media(mediaPlayer, m, &vlc_ex);
	libvlc_media_release(m);
}

void	VLCAudioPlayer::play()
{
	libvlc_media_player_play(mediaPlayer, &vlc_ex);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(checkFrame()));
	timer->start(50);
}

void	VLCAudioPlayer::seek(const int step)
{

}

void	VLCAudioPlayer::stop()
{

}

void	VLCAudioPlayer::checkFrame()
{
	static int framenb = -1;

 
	fps = libvlc_media_player_get_fps(mediaPlayer, &vlc_ex);
	length = libvlc_media_player_get_length(mediaPlayer, &vlc_ex);
	float pos = libvlc_media_player_get_position(mediaPlayer, &vlc_ex);
	qDebug() << pos;
	int	frame = ((length * fps) / 1000) * pos;
	if ((int) frame != framenb) {
		framenb = frame;
		emit frameChanged(framenb);
	}
}