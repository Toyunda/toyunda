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

#include <QApplication>
#include <QPluginLoader>
#include <QDir>
#include <QThread>
#include "qtoyunda.h"
#include "rawsubstream.h"
#include "abstractrenderer.h"
#include "abstractfileplayer.h"


QToyunda::QToyunda(SQErrorHandler* errH) : QObject()
{
  s_errorHandler = errH;
  player = NULL;
  renderer = NULL;
}


bool  QToyunda::init()
{
  bool  selplayer, selrenderer = true;
  selplayer = selectPlayer();
  selrenderer = selectRenderer();
  if (!selplayer && !selrenderer) {
      s_errorHandler->addError(SQError(SQError::Critical, "Can't found selected player or selected renderer"));
      return false;
  }
  toyundaSub = new RawSubStream();

  QObject::connect(player, SIGNAL(error(SQError)), this, SLOT(s_pluginError(SQError)));
  QObject::connect(dynamic_cast<QObject *>(renderer), SIGNAL(error(SQError)), this, SLOT(s_pluginError(SQError)));

  qDebug() << "===============Init player=================" << player->identifier();
  if (!player->init(s_playerOption))
    return false;
  qDebug() << "===============Init renderer===============" << renderer->identifier();
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
  return true;
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
  qDebug() << "===============Load subtitle===============" << s_subtitleFile;
  toyundaSub->createFromFile(s_subtitleFile);
  qDebug() << "===============Open video==================" << s_videoFile;
  player->open(s_videoFile);
}

void  QToyunda::play()
{
  qDebug() << "===============Playing the video===========";
//  qDebug() << "Hi I am qtoyunda play and I am in thread : " << QThread::currentThreadId();
  player->play();
}


bool  QToyunda::selectPlayer()
{
    if (s_filePlayerPlugins.size() == 0)
    {
        s_errorHandler->addError(SQError(SQError::Critical, "No player found"));
        return false;
    }
    foreach(FilePlayer *item, s_filePlayerPlugins)
    {
        if (item->identifier() == s_playerName)
        {
            player = item;
            return true;
        }
    }
    s_errorHandler->addError(SQError(SQError::Critical, "Can't find the selected player, or no player selected"));
    return false;
}

bool  QToyunda::selectRenderer()
{
    if (s_toyundaRendererPlugins.size() == 0)
    {
        s_errorHandler->addError(SQError(SQError::Critical, "No renderer found"));
        return false;
    }
    foreach(ToyundaRenderer *item, s_toyundaRendererPlugins)
    {
        if (item->identifier() == s_rendererName)
        {
            renderer = item;
            return true;
        }
    }
    s_errorHandler->addError(SQError(SQError::Critical, "Can't find the selected renderer, or no renderer selected"));
    return false;
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


bool    QToyunda::loadPlugins()
{
    foreach(QString fileName, s_pluginDirectory.entryList(QDir::Files))
    {
        if (!fileName.contains(QRegExp(".*\\.(dll|so)$")))
            continue;
        qDebug() << "PLUGINS : Loading " << fileName;
        QPluginLoader loader(s_pluginDirectory.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
        {
            qDebug() << "Module loaded";
            AbstractRenderer* toyrend = qobject_cast<AbstractRenderer*>(plugin);
            if (toyrend) {
                PluginInfo  info;
                info.fileInfo = QFileInfo(fileName);

                ToyundaRenderer* testtruc = toyrend->getMe();
                qDebug() << "Renderer Module found   : " << testtruc->identifier();
                info.name = testtruc->identifier();
                info.type = PluginType::Renderer;
                s_toyundaRendererPlugins.append(testtruc);
                s_pluginInfos.append(info);
            }
            AbstractFilePlayer* toyplay = qobject_cast<AbstractFilePlayer*>(plugin);
            if (toyplay) {
                PluginInfo  info;
                info.fileInfo = QFileInfo(fileName);
                FilePlayer* testtruc = toyplay->getMe();
                qDebug() << "FilePlayer Module found : " << testtruc->identifier();
                info.name = testtruc->identifier();
                info.type = PluginType::Player;
                s_filePlayerPlugins.append(testtruc);
                s_pluginInfos.append(info);
            }
            s_pluginList.append(plugin);
        } else {
            s_errorHandler->addError(SQError(SQError::Warning, "Can't load module : " + fileName, loader.errorString()));
        }
    }
    if (s_pluginList.size() > 0)
        return true;
    s_errorHandler->addError(SQError(SQError::Critical, "No plugins found in : " + s_pluginDirectory.absolutePath()));
    return false;
}


void    QToyunda::s_pluginError(SQError err)
{
    s_errorHandler->addError(err);
}


//Getter/setter

const QList<PluginInfo>& QToyunda::getPluginInfos() const
{
    return s_pluginInfos;
}

void QToyunda::setPluginDirectory(QDir pluginDirectory)
{
    s_pluginDirectory = pluginDirectory;
}

void QToyunda::setPlayerName(QString playerNam)
{
    s_playerName = playerNam;
}

const QString& QToyunda::playerName() const
{
    return s_playerName;
}

void QToyunda::setRendererName(QString rendererNam)
{
    s_rendererName = rendererNam;
}

const QString& QToyunda::rendererName() const
{
    return s_rendererName;
}

void QToyunda::setRendererOption(QStringList rendererOpt)
{
    s_rendererOption = rendererOpt;
}

void QToyunda::setPlayerOption(QStringList playerOpt)
{
    s_playerOption = playerOpt;
}

void QToyunda::setRendererQWidgetParent(QWidget *wid)
{
    renderer->setQWidgetParent(wid);
}

void QToyunda::dispose()
{
    player->dispose();
    renderer->dispose();
}

void QToyunda::stop()
{
    player->stop();
    toyundaSub->reinit();
}
