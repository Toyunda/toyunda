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


#ifndef GuiliGuili_H
#define GuiliGuili_H

#include <QMainWindow>
#include <QSettings>
#include <QStandardItem>
#include "ui_GuiliGuili.h"
#include "playlist.h"
#include "configdialog.h"
#include "qtoyunda.h"
#include "profil.h"
#include "profilmodel.h"
#include "graphicerrorhandler.h"
#include "songtreemodel.h"




class GuiliGuili : public QMainWindow
{
Q_OBJECT


struct SongState
    {
        enum Type
        {
            Playing,
            Paused,
            Stopped
        };
 };
public:
    GuiliGuili();
    virtual ~GuiliGuili();

private:

	void		play();
        void            play_once(QString video, QString lyrics);
	void		stop();
	void		nextSong();
        void		prevSong();
	void		setKaraokeDir();
	void		readKaraokeDir();
	void		populateSongView();
        void            PopulateTreePluginInfo(QList<PluginInfo>);
	void		openPlaylist(QString);
	void		savePlaylist(QString);
	void		sortAlphaSongMap();
    void            addCategorySongView(SongTreeModel* model, QMap<QString, QList<Song*> > songList);
    bool        initProfil(Profil*);
	
private slots :
	//Ui slots
        void            init();
	void		on_playButton_clicked();
        void            on_configurationButton_clicked();
	void		on_stopButton_clicked();
	void		on_playlistView_doubleClicked(const QModelIndex&);
        void            on_volumeSlider_valueChanged(int);
        void            on_searchInput_editingFinished();
        void            on_searchButton_clicked();
        void            on_songTreeView_doubleClicked(const QModelIndex &);
	void		playlistView_selectionChanged(const QItemSelection &, const QItemSelection &);

        void            closeEvent(QCloseEvent *);

        void            on_error_and_quit();
        void            on_error_only();
	
	//other slots
	void		song_paused();
	void		song_playing();
	void		song_stopped();
	void		song_finished();

	void on_clearPlaylistButton_clicked();


    void on_ProfilComboBox_activated(int index);

    void on_openPlaylistButton_clicked();

    void on_savePlaylistButton_clicked();

signals:
        void            error_and_quit();
        void            error_only();

private :


	Song		m_currentSong;
    ConfigDialog    m_configDialog;
	Ui::GuiliGuili	ui;
	QString		m_karaoke_dir;
	Playlist	m_currentPlaylist;
	uint		m_currentPos;
	QSettings*	m_settings;
	QList<Song*>	m_allsongs;
    QList<Song*>    m_searchResult;
    SongState::Type	m_songState;
    GraphicErrorHandler*    m_errorHandler;
    QMap<QString, QList<Song *> > m_songByAlpha;
	QMap<QString, QList<Song *> > m_songByType;

    Profil*          m_currentProfil;
    ProfilModel*     m_profilmodel;
};
#endif // GuiliGuili_H
