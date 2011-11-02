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

#ifndef H_XINEAUDIOPLAYER
#define H_XINEAUDIOPLAYER

#include "fileplayer.h"
#include <xine.h>
#include <xine/xineutils.h>


/** @class XineAudiPlayer
* @brief A xine player that only play audio
*
* This player only the audio part of a file, It's a xine based player.
* The video output is set to Null, see FilePlayer documentation
*
* Identifier : xineaudio
*/

extern "C" void xine_audio_play_event_listener(void *user_data, const xine_event_t *event);


class XineAudioPlayer : public FilePlayer
{
  Q_OBJECT;
  public:
    XineAudioPlayer();
    bool	init(const QStringList optionList = QStringList());

  public slots:
    void  open(const QString fileName);
    void  play();
    void  seek(const int step);
    void  stop();
    /**
     * Function called by xine when a xine's event occur (see xine documentation)
     * @param user_data data given in the xine function that create event thread, here is the class itself (this)
     * @param event the xine event
     */
    friend void xine_audio_play_event_listener(void *user_data, const xine_event_t *event);

  private slots:
    void  checkFrame();

  private:
    QString       s_fileName;
    xine_t        *xine;
    xine_stream_t *stream;
    int           duree_moisie;
};



#endif