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

#include "qtoyunda.h"
#include "allplayer.h"
#include "allrenderer.h"
#include "rawsubstream.h"


QToyunda::QToyunda(QString playerNam, QString rendererNam, QStringList playerOpt, QStringList rendererOpt) : QObject()
{
  s_playerName = playerNam;
  s_rendererName = rendererNam;
  s_playerOption = playerOpt;
  s_rendererOption = rendererOpt;
  player = NULL;
  renderer = NULL;
}

void  QToyunda::init()
{
  selectPlayer();
  selectRenderer();
  //DebugRenderer *Debugrenderer = new DebugRenderer();
  toyundaSub = new RawSubStream();
  qDebug() << "Init player";
  player->init(s_playerOption);
  qDebug() << "Init renderer";
  renderer->init(s_rendererOption);
  qDebug() << "Connect signal/slots";
  QObject::connect(player, SIGNAL(frameChanged(int)), toyundaSub, SLOT(setCurrentFrame(int)));
  QObject::connect(toyundaSub, SIGNAL(currentSubChanged(void)), renderer, SLOT(renderUpdate(void)));
  //QObject::connect(toyundaSub, SIGNAL(currentSubChanged(void)), Debugrenderer, SLOT(renderUpdate(void)));
  renderer->setToyundaSubStream(toyundaSub);
}


void  QToyunda::load(QString videoFil, QString subtitleFil)
{
  s_subtitleFile = subtitleFil;
  s_videoFile = videoFil;
  qDebug() << "Load subtitle";
  toyundaSub->createFromFile(s_subtitleFile);
  qDebug() << "Open video";
  player->open(s_videoFile);
}

void  QToyunda::play()
{
  player->play();
}

// Must find another way to find the good class
void  QToyunda::selectPlayer()
{
  if (s_playerName == "fake")
    player = new FakePlayer;
#ifdef Q_WS_X11
  if (s_playerName == "xineaudio")
    player = new XineAudioPlayer;
#endif
}

void  QToyunda::selectRenderer()
{
  if (s_rendererName == "debug")
    renderer = new DebugRenderer;
  if (s_rendererName == "qosd")
    renderer = new QOSD;
}

void	QToyunda::showPlayerOption()
{
  if (player == NULL)
    selectPlayer();
  player->showOption();
}

void	QToyunda::showRendererOption()
{
  if (renderer == NULL)
    selectRenderer();
  renderer->showOption();
}
