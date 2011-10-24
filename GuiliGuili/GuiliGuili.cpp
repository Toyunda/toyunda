#include "GuiliGuili.h"

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
        QSettings settings("skarsnik.nyo.fr", "GuiliGuili");
        m_qtoyunda = new QToyunda();
        QDir pluginPath = qApp->applicationDirPath();
        pluginPath.cd("plugins");
        qDebug() << pluginPath;
        m_qtoyunda->setPluginDirectory(pluginPath);
        m_qtoyunda->loadPlugins();
        if (!settings.contains("karaoke_dir"))
        {
            int diagretour = m_configDialog.exec();
            if (diagretour) {
                m_karaoke_dir = m_configDialog.ui.karaokeDirLineEdit->text();
                settings.setValue("karaoke_dir", m_karaoke_dir);
            }
        } else
        {
            m_karaoke_dir = settings.value("karaoke_dir").toString();
        }
        m_qtoyunda->setPlayerName("qgstaudio");
        m_qtoyunda->setRendererName("qosd");
	QStringList rendererOption;
        rendererOption << "logo=:/main/Toyunda logo.png";
        m_qtoyunda->setRendererOption(rendererOption);
	QDir::setCurrent(m_karaoke_dir);
	readKaraokeDir();
	createToolbox();
	m_currentSong = new Song();
	m_qtoyunda->init();
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
	qDebug() << "play";
	m_qtoyunda->showRenderer();
	m_qtoyunda->load("Videos/" + m_currentSong->videoPath, "Lyrics/" + m_currentSong->subtitlePath);
	m_qtoyunda->play();
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
		*m_currentSong = m_currentPlaylist.at(m_currentPos + 1);
		play();
	}
}



// Init fonction
void	GuiliGuili::createToolbox()
{	
	QMutableMapIterator<QString, QList<Song *> > it(m_songByAlpha);
	
	QStandardItemModel	*model =  new QStandardItemModel();
	QStandardItem	*parentItem = model->invisibleRootItem();
	//SongListTreeItemModel	*model = new SongListTreeItemModel();
	
	while (it.hasNext())
	{
		it.next();
		QList<Song *>* lsg = &(it.value());
		QStandardItem	*categoryItem = new QStandardItem();
		categoryItem->setData(it.key(), Qt::DisplayRole);
		QVariant v1 = qVariantFromValue((uint) lsg);
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
			QVariant v = qVariantFromValue((uint) sg);
			songItem->setData(v, Qt::UserRole + 1);
			categoryItem->appendRow(songItem);
		}
	}
	
	ui.songTreeView->setModel(model);
}

void GuiliGuili::on_playButton_clicked()
{
	play();
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
	*m_currentSong = s;
}

