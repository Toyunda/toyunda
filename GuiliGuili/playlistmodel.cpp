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


#include "playlistmodel.h"
#include <qmimedata.h>
#include <qstringlist.h>
#include <qstandarditemmodel.h>

QVariant PlaylistModel::data(const QModelIndex& index, int role) const
{
	QVariant	ret = QVariant();
	if (!index.isValid())
		return QVariant();
	if (index.row() >= m_playlist->count())
		return QVariant();
	if (role == Qt::DisplayRole)
		return m_playlist->at(index.row()).title;
        if (role == Qt::ToolTipRole)
            return m_playlist->at(index.row()).iniFile;
	return ret;
}

int	PlaylistModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
        return m_playlist->count();
}

PlaylistModel::PlaylistModel(Playlist* pl)
{
	m_playlist = pl;
    setSupportedDragActions(Qt::MoveAction);
}

PlaylistModel::~PlaylistModel()
{

}

void PlaylistModel::setPlaylist(Playlist* pl)
{
	m_playlist = pl;
    reset();
}


bool PlaylistModel::insertRows(int row, int count, const QModelIndex& parent)
{
        /*if (row == -1)
                row = 0;
        beginInsertRows(parent, row, row + count);
        endInsertRows();*/
        return QAbstractItemModel::insertRows(row, count, parent);
}

bool PlaylistModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if (row < 0 && count < 1)
        return false;
    for (int i = 0; i < count; i++)
        m_playlist->removeAt(row + i);
    return true;
    //return QAbstractItemModel::removeRows(row, count, parent);
}



bool PlaylistModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	Q_UNUSED(action);
	Q_UNUSED(row);
	Q_UNUSED(column);
    //qDebug() << "drop";
    if (data->hasFormat(SongListMimeType))
    {
        if (row == -1)
            if (parent.isValid())
                row = parent.row() + 1;
            else
                row = m_playlist->count();
        int cpt = 0;
        QList<Song> sgl = Song::listDeserialize(data->data(SongListMimeType));
        beginInsertRows(parent, row, row + sgl.count());
        QListIterator<Song> it(sgl);
        while (it.hasNext())
        {
            Song sg = it.next();
            m_playlist->addAt(sg, row + cpt);
            cpt++;
        }
        endInsertRows();
    }
	return true;
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

	if (index.isValid())
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList PlaylistModel::mimeTypes() const
{
    return QStringList() << SongListMimeType;
}

QMimeData *PlaylistModel::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.count() < 0)
        return 0;
    QListIterator<QModelIndex> it(indexes);
    QList<Song> songs;
    while (it.hasNext())
    {
        Song sg = m_playlist->at(it.next().row());
        songs.append(sg);
    }
    QMimeData*  toret = new QMimeData();
    QByteArray  data = Song::listSerialize(songs);
    toret->setData(SongListMimeType, data);
    toret->setText(songs.at(0).title);
    //qDebug() << "drag started ? " << songs.at(0).title;
    return toret;
}


Qt::DropActions PlaylistModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}


