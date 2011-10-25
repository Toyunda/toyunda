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


#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QtCore/qabstractitemmodel.h>
#include <QList>
#include <QMetaType>
#include "playlist.h"

class PlaylistModel : public QAbstractListModel
{

public:
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    int         rowCount(const QModelIndex& parent = QModelIndex()) const;
    bool	dropMimeData( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );
    bool	insertRows ( int row, int count, const QModelIndex & parent = QModelIndex());
    bool	removeRows ( int row, int count, const QModelIndex & parent = QModelIndex());
    Qt::ItemFlags	flags(const QModelIndex &index) const;
    
    Qt::DropActions	supportedDropActions() const;
    
    PlaylistModel(Playlist *pl);
    virtual ~PlaylistModel();
    void	setPlaylist(Playlist* pl);

private:
	Playlist* m_playlist;
};

#endif // PLAYLISTMODEL_H
