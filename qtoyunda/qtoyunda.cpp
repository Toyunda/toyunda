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

#include <QApplication>
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
  qDebug() << "===============Init player=================";
  player->init(s_playerOption);
  qDebug() << "===============Init renderer===============";
  renderer->init(s_rendererOption);
  qDebug() << "Connect signal/slots";
  QObject::connect(player, SIGNAL(frameChanged(int)), toyundaSub, SLOT(setCurrentFrame(int)));
  QObject::connect(toyundaSub, SIGNAL(currentSubChanged(void)), dynamic_cast<QObject*> (renderer), SLOT(renderUpdate(void)));
  QObject::connect(player, SIGNAL(finished()), this, SIGNAL(finished()));
  QObject::connect(player, SIGNAL(paused()), this, SIGNAL(paused()));
  QObject::connect(player, SIGNAL(stopped()), this, SIGNAL(stopped()));
  QObject::connect(player, SIGNAL(played()), this, SIGNAL(played()));
  //QObject::connect(toyundaSub, SIGNAL(currentSubChanged(void)), Debugrenderer, SLOT(renderUpdate(void)));
  renderer->setToyundaSubStream(toyundaSub);
}

void QToyunda::hideRenderer()
{
	renderer->hide();
}

void QToyunda::showRenderer()
{
	renderer->show();
}


void  QToyunda::load(QString videoFil, QString subtitleFil)
{
  s_subtitleFile = subtitleFil;
  s_videoFile = videoFil;
  qDebug() << "===============Load subtitle===============";
  toyundaSub->createFromFile(s_subtitleFile);
  qDebug() << "===============Open video==================";
  player->open(s_videoFile);
}

void  QToyunda::play()
{
  qDebug() << "===============Playing the video===========";
  player->play();
}

// Must find another way to find the good class
void  QToyunda::selectPlayer()
{
#ifdef FAKEPLAYER_HERE
	if (s_playerName == "fake")
	{
		player = new FakePlayer;
		return ;
	}
#endif
#ifdef QTGSTREAMER_HERE
	if (s_playerName == "qgstaudio")
	{
		player = new QGstAudioPlayer;
		return ;
	}
#endif
	qCritical() << "No player found";
}

void  QToyunda::selectRenderer()
{
#ifdef DEBUGRENDERER_HERE
	if (s_rendererName == "debug")
	{
		renderer = new DebugRenderer;
		return ;
	}
#endif
#ifdef QOSD_HERE
	if (s_rendererName == "qosd")
	{
		renderer = new QOSD;
		return ;
	}
#endif
	qCritical() << "No renderer found";
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

void	QToyunda::quit()
{
  qApp->exit(0);
}
