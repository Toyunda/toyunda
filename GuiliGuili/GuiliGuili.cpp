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
    
    /* Load config
     */
	m_settings = new QSettings("skarsnik.nyo.fr", "GuiliGuili");
        m_qtoyunda = new QToyunda();
        QDir pluginPath = qApp->applicationDirPath();
        pluginPath.cd("plugins");
        qDebug() << pluginPath;
        m_qtoyunda->setPluginDirectory(pluginPath);
        m_qtoyunda->loadPlugins();
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
	qDebug() << "I am the GuiGui and I am in thread : " << QThread::currentThreadId();
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
        PopulateTreePluginInfo(m_qtoyunda->getPluginInfos());
	m_qtoyunda->init();
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

GuiliGuili::~GuiliGuili()
{
	
	
}

void GuiliGuili::play()
{
    if (!m_currentSong.title.isEmpty()) {
	    qDebug() << "play";
	    m_qtoyunda->showRenderer();
	    qDebug() << m_currentSong.videoPath;
	    m_qtoyunda->load("Videos/" + m_currentSong.videoPath, "Lyrics/" + m_currentSong.subtitlePath);
	    m_qtoyunda->play();
	}
}


void GuiliGuili::readKaraokeDir()
{
	QDir	kdir(m_karaoke_dir);
	
	QStringList dirFilter;
	dirFilter << "*.ini";
	kdir.setNameFilters(dirFilter);
	QStringList strlt = kdir.entryList();

	QStringListIterator it(strlt);
	while (it.hasNext())
	{
		Song *sg = new Song(it.next().toLocal8Bit());
		m_allsongs.append(sg);
		m_songByAlpha[sg->title.at(0).toUpper()].append(sg);
		m_songByType[sg->prefix].append(sg);
	}	
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
}

void	GuiliGuili::song_playing()
{
	ui.statusbar->showMessage("Song playing");
}

void	GuiliGuili::song_stopped()
{
	ui.statusbar->showMessage("Song stopped");
	m_qtoyunda->hideRenderer();
}


void	GuiliGuili::nextSong()
{
	if ((m_currentPos + 1 < m_currentPlaylist.count()))
	{
		m_currentSong = m_currentPlaylist.at(m_currentPos + 1);
		play();
	}
}



// Init fonction
void	GuiliGuili::populateSongView()
{	
	QMutableMapIterator<QString, QList<Song *> > it(m_songByAlpha);
	
	QStandardItemModel *model = static_cast<QStandardItemModel*>(ui.songTreeView->model());
	QStandardItem	*parentItem = model->invisibleRootItem();
	//SongListTreeItemModel	*model = new SongListTreeItemModel();
	
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
			QVariant v = qVariantFromValue((quintptr) sg);
			songItem->setData(v, Qt::UserRole + 1);
			categoryItem->appendRow(songItem);
		}
	}
}

void GuiliGuili::on_playButton_clicked()
{
	play();
}

void    GuiliGuili::on_configurationButton_clicked()
{
	m_configDialog.ui.karaokeDirLineEdit->setText(m_karaoke_dir);
	if (m_configDialog.exec());
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
        interfaceItem->setText(0, plugInfo.fileInfo.baseName());
        QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
        featureItem->setText(0, plugInfo.name);
    }
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

