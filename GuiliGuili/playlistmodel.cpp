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
	if (role == Qt::UserRole + 1)
	{
		QVariant v = qVariantFromValue<Song>(m_playlist->at(index.row()));
		return v;
	}
	return ret;
}

int	PlaylistModel::rowCount(const QModelIndex& parent) const
{
        return m_playlist->count();
}

PlaylistModel::PlaylistModel(Playlist* pl)
{
	m_playlist = pl;
	qDebug() << "nyo";
}

PlaylistModel::~PlaylistModel()
{

}

void PlaylistModel::setPlaylist(Playlist* pl)
{
	m_playlist = pl;
}


bool PlaylistModel::insertRows(int row, int count, const QModelIndex& parent)
{
        qDebug() << "row : " << row << " - count :" << count;
        if (row == -1)
                row = 0;
        beginInsertRows(parent, row, row + count);
	endInsertRows();
	return true;
	
}

bool PlaylistModel::removeRows(int row, int count, const QModelIndex& parent)
{
    return QAbstractItemModel::removeRows(row, count, parent);
}


// From qstandarditemmodel.cpp
static void decodeDataRecursive(QDataStream &stream, QStandardItem *item)
{
    int colCount, childCount;
    stream >> *item;
    stream >> colCount >> childCount;
    item->setColumnCount(colCount);
    
    int childPos = childCount;
    
    while(childPos > 0) {
        childPos--;
        QStandardItem *child = new QStandardItem;
        decodeDataRecursive(stream, child);
        item->setChild( childPos / colCount, childPos % colCount, child);
    }
}


bool PlaylistModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	QStringListIterator it(data->formats());
	
	while (it.hasNext())
	{
		qDebug() << it.next();
	}
	// drop from songs list
	if (data->hasFormat("application/x-qstandarditemmodeldatalist"))
	{
	
		/* Code from qstandarditemmodel.cpp in qt */
		QByteArray encoded = data->data("application/x-qstandarditemmodeldatalist");
		QDataStream stream(&encoded, QIODevice::ReadOnly);


		//code based on QAbstractItemModel::decodeData
		// adapted to work with QStandardItem
                int top = INT_MAX;
                int left = INT_MAX;
		int bottom = 0;
		int right = 0;
		QVector<int> rows, columns;
		QVector<QStandardItem *> items;

		while (!stream.atEnd()) {
			int r, c;
			QStandardItem *item = new QStandardItem;
			stream >> r >> c;
			decodeDataRecursive(stream, item);

			rows.append(r);
			columns.append(c);
			items.append(item);
			top = qMin(r, top);
			left = qMin(c, left);
			bottom = qMax(r, bottom);
			right = qMax(c, right);
			if (item->hasChildren())
			{
				QList<Song*>* sgl = (QList<Song *>*) item->data(Qt::UserRole + 1).value<uint>();
				QListIterator<Song *> it(*sgl);
				while (it.hasNext())
				{
					Song *sg = it.next();
					m_playlist->add_song(*sg);
					insertRows(row, 1 , parent);
				}
			}
			else
			{
				Song* sg = (Song *) item->data(Qt::UserRole + 1).value<uint>();
				m_playlist->add_song(*sg);
				insertRows(row, 1 , parent);
			}
		}
		
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


Qt::DropActions PlaylistModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}


