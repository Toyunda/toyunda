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


#ifndef SONGLISTTREEITEMMODEL_H
#define SONGLISTTREEITEMMODEL_H

#include <QtCore/qabstractitemmodel.h>
#include <QList>
#include "song.h"

class SongListTreeItemModel : public QAbstractItemModel
{

public:
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& child) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    SongListTreeItemModel();
    ~SongListTreeItemModel();
    
    void	addSongList(const QString, QList<Song *>*);

private:
	QMap<QString, QList<Song *>*> m_songList;
	QMap<QString, int>	m_mapKeyId;
};

#endif // SONGLISTTREEITEMMODEL_H
