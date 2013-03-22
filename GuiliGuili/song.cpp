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
#include <QFile>

Song::Song()
{

}

Song::Song(const QString filePath, bool realFile)
{
	QRegExp		pathfmt("^([A-Z]+)\\s\\-\\s(.+)\\.ini$");

	iniFile = filePath;
	if (pathfmt.exactMatch(filePath))
	{	
		prefix = pathfmt.cap(1);
		title = pathfmt.cap(2);
	}
	if (realFile)
	{
            QFile   finifile(filePath);
            if (!finifile.open(QIODevice::Text | QIODevice::ReadOnly))
            {
                qCritical() << "Can't open : " << filePath << "  - " << finifile.errorString();
                return ;
            }
            QRegExp keyValue("([^=]+)=([^\\n]+)\\n{0,1}");
            while(!finifile.atEnd())
            {
                QString tmp = finifile.readLine();
                if (keyValue.exactMatch(tmp))
                {
                    if (keyValue.cap(1).toLower() == "aviname")
                        videoPath = keyValue.cap(2);
                    if (keyValue.cap(1).toLower() == "file")
                        subtitlePath = keyValue.cap(2);
                }
            }

	}
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


bool	operator<(const Song& s1, const Song& s2)
{
    int p = QString::localeAwareCompare(s1.title, s2.title);
    if (p < 0)
	return true;
    return false;
}

QDebug  operator<<(QDebug dbg, const Song& sg)
{
	dbg << sg.prefix;
	dbg << sg.title;
	return dbg;
}

bool Song::lessThan(Song* s1, Song* s2)
{
    return *s1 < *s2;
}

