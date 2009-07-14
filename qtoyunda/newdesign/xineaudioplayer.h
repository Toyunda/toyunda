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

  private slots:
    void  checkFrame();

  private:
    QString       s_fileName;
    xine_t        *xine;
    xine_stream_t *stream;
    int           duree_moisie;
};

#endif