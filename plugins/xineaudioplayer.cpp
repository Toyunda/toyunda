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
#include <QTimer>
#include "xineaudioplayer.h"

XineAudioPlayer::XineAudioPlayer() : FilePlayer()
{
  setIdentifier("xineaudio");
  addOption("ao", "alsa", "Audio output driver");
}

bool  XineAudioPlayer::init(QStringList opt)
{
  xine_video_port_t   *vo_port;
  xine_audio_port_t   *ao_port;
  xine_event_queue_t  *event_queue;
  xine = xine_new();
  xine_init(xine);

  handleOption(opt);
  xine_engine_set_param(xine, XINE_ENGINE_PARAM_VERBOSITY, XINE_VERBOSITY_DEBUG);
  qDebug() << "open video driver";
  vo_port = xine_open_video_driver(xine, NULL, XINE_VISUAL_TYPE_NONE, NULL);
  if (vo_port == NULL) {
    qCritical() << "trouble with the video driver";
    exit(1);
  }

  qDebug() << "open audio driver";
  qDebug() << optionValue["ao"].toString() << "\n";
  ao_port = xine_open_audio_driver(xine , optionValue["ao"].toString().toStdString().c_str(), NULL);
  if (ao_port == NULL) {
    qCritical() << "Trouble with the audio driver";
    exit(1);
  }
  stream = xine_stream_new(xine, ao_port, vo_port);
  if (stream == NULL) {
    qCritical() << "Trouble creating the stream";
    exit(1);
  }
  event_queue = xine_event_new_queue(stream);
  xine_event_create_listener_thread(event_queue, xine_audio_play_event_listener, (void *) this);
  return true;
}

void  XineAudioPlayer::open(QString filename)
{
  const char* file = filename.toUtf8().constData();
  if (!xine_open(stream, file)) {
     qCritical() << xine_get_error(stream);
     qCritical() << "unable to open mrl : " << file;
        exit(1);
  }
  s_fileName = filename;
}

void  XineAudioPlayer::play()
{
  qDebug() << "=============================" << stream;
  if (!xine_play(stream, 0, 0)) {
    qCritical() << xine_get_error(stream);
  }
  duree_moisie = xine_get_stream_info(stream, XINE_STREAM_INFO_FRAME_DURATION);
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(checkFrame()));
  timer->start(50);
}

void  XineAudioPlayer::seek(const int step)
{

}

void  XineAudioPlayer::stop()
{

}


void  XineAudioPlayer::checkFrame()
{
  static int framenb = -1;
  float t = 0;
  int m1, m2, m3;
  xine_get_pos_length(stream, &m1, &m2, &m3);
  t = m2 * 90 / duree_moisie + 0.5;
  if ((int) t != framenb) {
    framenb = t;
    qDebug() <<"frame";
    emit frameChanged(framenb);
  }
}

extern "C" void xine_audio_play_event_listener(void *user_data, const xine_event_t *event)
{
  XineAudioPlayer *xa = (XineAudioPlayer *) user_data;
  switch(event->type) {
  case XINE_EVENT_UI_PLAYBACK_FINISHED:
      xa->finished();
      break;
    }
}
