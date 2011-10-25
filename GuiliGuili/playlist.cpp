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


#include "playlist.h"

Playlist::Playlist()
{

}

Playlist::Playlist(QList< Song *> lsg)
{
	QListIterator<Song*> it(lsg);
	while (it.hasNext())
	{
		Song *sg = it.next();
		m_songs.append(*sg);
	}
}


Playlist::Playlist(const Playlist& other)
{

}

Playlist::~Playlist()
{

}

bool	Playlist::load(const QString& fileName)
{
    return true;
}

bool	Playlist::save(const QString& fileName) const
{
    return true;
}

void	Playlist::add_song(const Song toadd)
{
	m_songs.append(toadd);
}

void	Playlist::addAt(const Song toadd, const int at)
{
	m_songs.insert(at, toadd);
}


void	Playlist::removeAt(const int i)
{
	m_songs.removeAt(i);
}


void	Playlist::clear()
{
	m_songs.clear();
}

int	Playlist::count() const
{
	return m_songs.size();
}

Song	Playlist::at(int index) const
{
	return m_songs.at(index);
}

