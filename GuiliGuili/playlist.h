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


#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <QtCore/QMetaType>
#include <QtCore/QList>
#include "song.h"

class Playlist
{

public:
	Playlist();
	Playlist(QList<Song*>);
	Playlist(const Playlist& other);
	virtual ~Playlist();
	
	bool	save(const QString& fileName) const;
	bool	load(const QString& filename);
	void	add_song(const Song);
	void	addAt(const Song, const int);
	void	removeAt(const int);
	void	clear();
	int	count() const;
	Song	at(int) const;

private:
	QList<Song>	m_songs;
};

Q_DECLARE_METATYPE(Playlist)

#endif // PLAYLIST_H
