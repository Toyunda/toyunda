/*
    This file is part of GuiliGuili software
    Copyright (C) 2011  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

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


#include "GuiliGuili.h"
#include <QThread>
#include <QtGui/QLabel>
#include <QSettings>
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <qdir.h>
#include <QDebug>
#include "playlistmodel.h"
#include "songlisttreeitemmodel.h"
#include <qstandarditemmodel.h>
#include <qevent.h>
#include <qdialog.h>


GuiliGuili::GuiliGuili()
{
	ui.setupUi(this);
        m_settings = new QSettings("skarsnik.nyo.fr", "GuiliGuili");

        // Create qtoyunda

        m_errorHandler = new GraphicErrorHandler();
        m_qtoyunda = new QToyunda(m_errorHandler);
        QObject::connect(this, SIGNAL(error_and_quit()), this, SLOT(on_error_and_quit()));
        QObject::connect(this, SIGNAL(error_only()), this, SLOT(on_error_only()));

        QDir pluginPath = qApp->applicationDirPath();
        pluginPath.cd("plugins");
        qDebug() << pluginPath;
        m_songState = SongState::Playing;
        m_qtoyunda->setPluginDirectory(pluginPath);
        ui.openPlaylistButton->setIcon(style()->standardPixmap(QStyle::SP_DialogOpenButton));
        ui.savePlaylistButton->setIcon(style()->standardPixmap(QStyle::SP_DialogSaveButton));
        // Ensure event loop is started
        QTimer::singleShot(0, this, SLOT(init()));
}
    
void GuiliGuili::init()
{
        if (!m_qtoyunda->loadPlugins())
        {
            emit error_and_quit();
            return ;
        }
        PopulateTreePluginInfo(m_qtoyunda->getPluginInfos());
        if (!m_settings->contains("karaoke_dir"))
        {
            int diagretour = m_configDialog.exec();
            if (diagretour) {
                m_karaoke_dir = m_configDialog.ui.karaokeDirLineEdit->text();
                m_settings->setValue("karaoke_dir", m_karaoke_dir);
            }
        } else
        {
            m_karaoke_dir = m_settings->value("karaoke_dir").toString();
        }
        //qDebug() << "I am the GuiGui and I am in thread : " << QThread::currentThreadId();
#ifdef Q_WS_WI
        m_qtoyunda->setPlayerName("fake");
        QStringList playerOption;
        playerOption << "duration=5000";
        m_qtoyunda->setPlayerOption(playerOption);
#else
        m_qtoyunda->setPlayerName("qgstaudio");
#endif
        m_qtoyunda->setRendererName("qosd");
        QStringList rendererOption;
        rendererOption << "logo=:/main/Toyunda logo.png";
        m_qtoyunda->setRendererOption(rendererOption);


        // Ui elemen
        QStandardItemModel	*model =  new QStandardItemModel();
        ui.songTreeView->setModel(model);

        setKaraokeDir();
        if(! m_qtoyunda->init())
        {
            emit error_and_quit();
            return ;
        }
        //m_qtoyunda->setRendererQWidgetParent(this);
        PlaylistModel *plmodel = new PlaylistModel(&m_currentPlaylist);
        ui.playlistView->setModel(plmodel);
        ui.playlistView->setAcceptDrops(true);
        QObject::connect(ui.playlistView->selectionModel(),
                SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(playlistView_selectionChanged(QItemSelection, QItemSelection)));

        QObject::connect(m_qtoyunda, SIGNAL(played()), this, SLOT(song_playing()));
        QObject::connect(m_qtoyunda, SIGNAL(paused()), this, SLOT(song_paused()));
        QObject::connect(m_qtoyunda, SIGNAL(stopped()), this, SLOT(song_stopped()));
        QObject::connect(m_qtoyunda, SIGNAL(finished()), this, SLOT(song_finished()));
}


void    GuiliGuili::on_error_and_quit()
{
    m_errorHandler->showError();
    //qApp->exit(1);
}


void    GuiliGuili::on_error_only()
{
    m_errorHandler->showError();
}

GuiliGuili::~GuiliGuili()
{
	
	
}

void	GuiliGuili::stop()
{
    if (m_songState == SongState::Playing)
    {
	m_qtoyunda->stop();
	m_qtoyunda->hideRenderer();
    }
}

void GuiliGuili::play()
{
    if (!m_currentSong.title.isEmpty()) {
	    if (m_songState == SongState::Playing)
		stop();
            qDebug() << "play";
	    qDebug() << m_currentSong.videoPath;
            if (m_qtoyunda->load("Videos/" + m_currentSong.videoPath, "Lyrics/" + m_currentSong.subtitlePath))
            {
                m_qtoyunda->play();
                m_qtoyunda->showRenderer();
            }
            else
                emit error_only();
	}
}

void	GuiliGuili::sortAlphaSongMap()
{
    QList<QString> keysSong = m_songByAlpha.keys();
    foreach (QString key, keysSong) {
	qSort(m_songByAlpha[key].begin(), m_songByAlpha[key].end(), Song::lessThan);
    }
}

void GuiliGuili::readKaraokeDir()
{
	/*
	  Cache file for karaoke format :
	  inifile=iniFileName
	  subtitlefile=subtitleFile
	  videofile=videoFile
	  */
#define	CACHE_KEY_INI "inifile"
#define CACHE_KEY_SUB "subtitlefile"
#define CACHE_KEY_VIDEO "videofile"
	if (m_settings->contains("karaoke_dir_cache"))
	{
	    qDebug() << "entry for cache";
	    qDebug() << m_settings->value("karaoke_dir_cache").toString();
	    if (m_settings->value("karaoke_dir_cache").toString() == m_karaoke_dir)
	    {
		QFile	cache(qApp->applicationDirPath().toLocal8Bit() + "/KaraokeDirCache.txt");
		if (cache.open(QIODevice::ReadOnly | QIODevice::Text)) {
		    QRegExp keyValue("([^=]+)=([^\\n]+)\\n");
		    while(!cache.atEnd())
		    {
			Song *sg;
			QString inifile = cache.readLine();
			if (keyValue.exactMatch(inifile) && keyValue.cap(1) == CACHE_KEY_INI)
			    sg = new Song(keyValue.cap(2), false);
			QString subfile = cache.readLine();
			if (keyValue.exactMatch(subfile) && keyValue.cap(1) == CACHE_KEY_SUB)
			    sg->subtitlePath = keyValue.cap(2);
			QString videofile = cache.readLine();
			if (keyValue.exactMatch(videofile) && keyValue.cap(1) == CACHE_KEY_VIDEO)
			    sg->videoPath = keyValue.cap(2);
			m_allsongs.append(sg);
			m_songByAlpha[sg->title.at(0).toUpper()].append(sg);
			m_songByType[sg->prefix].append(sg);
		    }
		    cache.close();
		    sortAlphaSongMap();
		    return ;
		}

	    }
	}
	QDir	kdir(m_karaoke_dir);
	
	if (!kdir.exists())
	{
	    qCritical() << "Can't open karaoke dir : " << m_karaoke_dir;
	    return ;
	}
	QStringList dirFilter;
	dirFilter << "*.ini";
	kdir.setNameFilters(dirFilter);
	QStringList strlt = kdir.entryList();

	bool cacheWritable = true;
	QFile	cache(qApp->applicationDirPath().toLocal8Bit() + "/KaraokeDirCache.txt");
	QTextStream cacheStream(&cache);
	if (!cache.open(QIODevice::WriteOnly | QIODevice::Text))
	{
	    qWarning() << "Can't open cache for writing : " << cache.errorString();
	    cacheWritable = false;
	} else
	    m_settings->setValue("karaoke_dir_cache", m_karaoke_dir);
	QStringListIterator it(strlt);
	while (it.hasNext())
	{
		Song *sg = new Song(it.next().toLocal8Bit());
		if (cacheWritable) {
		    cacheStream << CACHE_KEY_INI << "=" << sg->iniFile << "\n";
		    cacheStream << CACHE_KEY_SUB << "=" << sg->subtitlePath << "\n";
		    cacheStream << CACHE_KEY_VIDEO << "=" << sg->videoPath << "\n";
		}
		m_allsongs.append(sg);
		m_songByAlpha[sg->title.at(0).toUpper()].append(sg);
		m_songByType[sg->prefix].append(sg);
	}
	cache.close();
	sortAlphaSongMap();
}



// Song event 
void	GuiliGuili::song_finished()
{
	ui.statusbar->showMessage("Song finished");
	m_qtoyunda->hideRenderer();
	nextSong();
}

void	GuiliGuili::song_paused()
{
	ui.statusbar->showMessage("Song paused");
	m_songState = SongState::Paused;
}

void	GuiliGuili::song_playing()
{
	ui.statusbar->showMessage("Playing song : " + m_currentSong.title);
	m_songState = SongState::Playing;
}

void	GuiliGuili::song_stopped()
{
	ui.statusbar->showMessage("Song stopped");
	m_qtoyunda->hideRenderer();
	m_songState = SongState::Stopped;
}


void	GuiliGuili::nextSong()
{
	if ((m_currentPos + 1 < m_currentPlaylist.count()))
	{
            m_currentPos++;
            ui.playlistView->setCurrentIndex(ui.playlistView->model()->index(m_currentPos, 0));
                m_currentSong = m_currentPlaylist.at(m_currentPos);

		play();
	}
}




void	GuiliGuili::populateSongView()
{	
	QMutableMapIterator<QString, QList<Song *> > it(m_songByAlpha);
	
	QStandardItemModel *model = static_cast<QStandardItemModel*>(ui.songTreeView->model());
	QStandardItem	*parentItem = model->invisibleRootItem();
        ui.songTreeView->header()->hide();
	
	while (it.hasNext())
	{
		it.next();
		QList<Song *>* lsg = &(it.value());
		QStandardItem	*categoryItem = new QStandardItem();
		categoryItem->setData(it.key(), Qt::DisplayRole);
		QVariant v1 = qVariantFromValue((quintptr) lsg);
		categoryItem->setData(v1, Qt::UserRole + 1);
		categoryItem->setEditable(false);
		parentItem->appendRow(categoryItem);
		QListIterator<Song *>	itSong(*lsg);
		while (itSong.hasNext())
		{
			Song *sg = itSong.next();
			QStandardItem	*songItem = new QStandardItem();
			songItem->setData(sg->title, Qt::DisplayRole);
			songItem->setEditable(false);
			songItem->setToolTip(sg->iniFile);
			QVariant v = qVariantFromValue((quintptr) sg);
			songItem->setData(v, Qt::UserRole + 1);
			categoryItem->appendRow(songItem);
		}
	}
}


void	GuiliGuili::on_stopButton_clicked()
{
    stop();
}


void	GuiliGuili::on_playlistView_doubleClicked(const QModelIndex& index)
{
    Song s = index.data(Qt::UserRole + 1).value<Song>();
    m_currentPos = index.row();
    m_currentSong = s;
    play();
}

void	GuiliGuili::on_playButton_clicked()
{
	play();
}

void    GuiliGuili::on_configurationButton_clicked()
{
        m_configDialog.ui.karaokeDirLineEdit->setText(m_karaoke_dir);
        if (m_configDialog.exec())
	{
	    m_karaoke_dir = m_configDialog.ui.karaokeDirLineEdit->text();
	    m_settings->setValue("karaoke_dir", m_karaoke_dir);
	    setKaraokeDir();
	}
}

void GuiliGuili::playlistView_selectionChanged(const QItemSelection& selected , const QItemSelection& deselected)
{
	QModelIndex index;
	QModelIndexList items = selected.indexes();
	
	index = items.at(0);
	qDebug() << index.data(Qt::DisplayRole);
	Song s = index.data(Qt::UserRole + 1).value<Song>();
	qDebug() << s;
	qDebug() << s.prefix;
	m_currentPos = index.row();
	m_currentSong = s;
}

void GuiliGuili::PopulateTreePluginInfo(QList<PluginInfo> plInfo)
{

    QTreeWidgetItem *playerItem = new QTreeWidgetItem(m_configDialog.ui.pluginInfoTreeWidget);
    playerItem->setText(0, "Player");
    QTreeWidgetItem *rendererItem = new QTreeWidgetItem(m_configDialog.ui.pluginInfoTreeWidget);
    rendererItem->setText(0, "Renderer");
    foreach (PluginInfo plugInfo, plInfo) {
        QTreeWidgetItem *interfaceItem;
        if (plugInfo.type == PluginType::Player)
            interfaceItem = new QTreeWidgetItem(playerItem);
        if (plugInfo.type == PluginType::Renderer)
            interfaceItem = new QTreeWidgetItem(rendererItem);
        qDebug() << plugInfo.fileInfo.baseName();
        interfaceItem->setText(0, plugInfo.fileInfo.baseName());
        QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
        featureItem->setText(0, plugInfo.name);
    }
    m_configDialog.ui.pluginInfoTreeWidget->expandAll();
}

void GuiliGuili::closeEvent(QCloseEvent *)
{
    m_qtoyunda->dispose();
}

void GuiliGuili::setKaraokeDir()
{
    m_allsongs.clear();
    m_songByAlpha.clear();
    m_songByType.clear();
    QDir::setCurrent(m_karaoke_dir);
    readKaraokeDir();
    delete ui.songTreeView->model();
    QStandardItemModel	*model = new QStandardItemModel;
    ui.songTreeView->setModel(model);
    populateSongView();
}



