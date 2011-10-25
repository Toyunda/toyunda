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


#include "song.h"
#include <qsettings.h>
#include <qregexp.h>

Song::Song()
{

}

Song::Song(const QString filePath)
{
	QRegExp		pathfmt("^([A-Z]+)\\s\\-\\s(.+)\\.ini$");
	QSettings	inifile(filePath, QSettings::IniFormat);
	
	if (pathfmt.exactMatch(filePath))
	{	
		prefix = pathfmt.cap(1);
		title = pathfmt.cap(2);
	}
	subtitlePath = inifile.value("subtitles/file").toString();
	videoPath = inifile.value("movie/aviname").toString();
}


Song::Song(const Song& other)
{
	this->title = other.title;
	this->subtitlePath = other.subtitlePath;
	this->videoPath = other.videoPath;
	this->prefix = other.prefix;
}

Song::~Song()
{

}

QDebug  operator<<(QDebug dbg, const Song& sg)
{
	dbg << sg.prefix;
	dbg << sg.title;
	return dbg;
}

