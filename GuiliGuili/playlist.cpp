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
#include <QFile>
#include <QFileInfo>
#include <QDebug>

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
	Q_UNUSED(other);
}

Playlist::~Playlist()
{

}

/* The playlist file only containt the base name, so the user can share playlist */

bool	Playlist::load(const QString& fileName, const QString& basePath)
{
    QFile	file(fileName);

    clear();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QString line = file.readLine();
            line.chop(1);
            Song sg(basePath + "/" + line);
            add_song(sg);
        }
        file.close();
        return true;
    }
    return false;
}

bool	Playlist::save(const QString& fileName, const QString& basePath) const
{
    QFile   file(fileName);
    Q_UNUSED(basePath);

    qDebug() << m_songs.count();
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        QListIterator<Song> i(m_songs);
        while(i.hasNext())
        {
            const Song& sg = i.next();
            qDebug() << sg.iniFile;
            QFileInfo   fi(sg.iniFile);
            stream << fi.fileName() << "\n";
        }
        file.close();
        return true;
    } else
        qDebug() <<  file.errorString();
    return false;
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

