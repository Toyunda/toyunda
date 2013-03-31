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


#ifndef SONG_H
#define SONG_H
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QPointer>
#include <QDebug>

class Song
{
public:
	Song();
	Song(const QString, bool realFile = true);
	Song(const Song& other);
	virtual ~Song();
    
	QString	title;
	QString	iniFile;
	QString	prefix;
	QString	subtitlePath;
	QString	videoPath;
	static bool lessThan(Song*, Song*);
    static QByteArray   listSerialize(QList<Song>);
    static QList<Song>  listDeserialize(QByteArray);
};

const   QString SongMimeType = "application/x-guiliguili-song";
const   QString SongListMimeType = "application/x-guiliguili-song-list";

bool	operator<(const Song&, const Song&);

QDebug  operator<<(QDebug, const Song&);

Q_DECLARE_METATYPE(Song)
Q_DECLARE_METATYPE(Song*)

Q_DECLARE_METATYPE(QList<Song*>*)

#endif // SONG_H
