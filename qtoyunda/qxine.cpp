#include <QTextStream>
#include <QTimer>
#include <iostream>
#include "qxine.h"

extern QTextStream cout;

QXine::QXine() : QObject()
{
  ;
}

void	QXine::init()
{
  xine_video_port_t   *vo_port;
  xine_audio_port_t   *ao_port;
  xine = xine_new();
  xine_init(xine);
  cout << "open video driver\n";
  vo_port = xine_open_video_driver(xine, NULL, XINE_VISUAL_TYPE_NONE, NULL);
  cout << "open audio driver\n";
  ao_port = xine_open_audio_driver(xine , "alsa", NULL);
  stream = xine_stream_new(xine, ao_port, vo_port);
}

void	QXine::start(QString mfile)
{
  const char* file = mfile.toLocal8Bit().constData();
  if (!xine_open(stream, file) || !xine_play(stream, 0, 0)) {
     cout << "unable to open mrl : " << mfile;
        exit(1);
  }
  duree_moisie = xine_get_stream_info(stream, XINE_STREAM_INFO_FRAME_DURATION);
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(checkFrame()));
  timer->start(50);
}

void	QXine::checkFrame()
{
  static int framenb = -1;
  float t = 0;
  int m1, m2, m3;
  xine_get_pos_length(stream, &m1, &m2, &m3);
  t = m2 * 90 / duree_moisie + 0.5;
  if ((int) t != framenb) {
    framenb = t;
    emit frameChanged(framenb);
  }
}

