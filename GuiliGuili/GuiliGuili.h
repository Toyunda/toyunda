#ifndef GuiliGuili_H
#define GuiliGuili_H

#include <QtGui/QMainWindow>
#include "ui_GuiliGuili.h"
#include "playlist.h"
#include "configdialog.h"
#include "qtoyunda.h"

class GuiliGuili : public QMainWindow
{
Q_OBJECT
public:
    GuiliGuili();
    virtual ~GuiliGuili();

private:
	void		readKaraokeDir();

	void		createToolbox();
	void		play();
	void		stop();
	void		nextSong();
	void		prevSong();
	void		openPlaylist(QString);
	void		savePlaylist(QString);
	
private slots :
	//Ui slots
	void		on_playButton_clicked();
	void		playlistView_selectionChanged(const QItemSelection &, const QItemSelection &);
	
	//other slots
	void		song_paused();
	void		song_playing();
	void		song_stopped();
	void		song_finished();

private :
	Song*		m_currentSong;
        ConfigDialog    m_configDialog;
	Ui::GuiliGuili	ui;
	QString		m_karaoke_dir;
	Playlist	m_currentPlaylist;
	uint		m_currentPos;
	QList<Song*>	m_allsongs;
	QToyunda	*m_qtoyunda;
	QMap<QString, QList<Song *> > m_songByAlpha;
	QMap<QString, QList<Song *> > m_songByType;
};

#endif // GuiliGuili_H
