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


#include "songlisttreeitemmodel.h"

QVariant SongListTreeItemModel::data(const QModelIndex& index, int role) const
{     
	if (!index.isValid())
		return QVariant();
	if (role != Qt::DisplayRole)
		return QVariant();
	QList<Song *> *tmp;
	tmp = static_cast<QList<Song*> *>(index.internalPointer());
	if (tmp == NULL)
		return m_mapKeyId.key(index.row());
	return  tmp->at(index.row())->title;

}

int SongListTreeItemModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

int SongListTreeItemModel::rowCount(const QModelIndex& parent) const
{
	if (!parent.isValid())
	{
		qDebug() << "Category size";
		return m_songList.count();
	}
	qDebug() << parent;
	QList<Song *> *tmp;
	tmp = static_cast<QList<Song*> *>(parent.internalPointer());
	qDebug() << "Pointer hold by parent" << tmp;
	qDebug() << m_mapKeyId.key(parent.row());
	if (tmp != NULL)
		return 0;
	return m_songList[m_mapKeyId.key(parent.row())]->count();
	return 0;
}

QModelIndex SongListTreeItemModel::parent(const QModelIndex& child) const
{
	//qDebug() << "parent";
	if (!child.isValid())
		return QModelIndex();
	// No internal pointer, it's a qmap element
	QList<Song *> *tmp = static_cast<QList<Song *> *>(child.internalPointer());
	if (tmp == NULL)
	{
		//qDebug() << "tmp null, it's Index value";
		return QModelIndex();
	}
	return createIndex(m_mapKeyId[m_songList.key(tmp)], 0, static_cast<void *>(tmp));
	
}

QModelIndex SongListTreeItemModel::index(int row, int column, const QModelIndex& parent) const
{
	//qDebug() << "index";
	if (!hasIndex(row, column, parent))
	{
		//qDebug() << "has not index";
		return QModelIndex();
	}
	//it's a song title
	if (parent.isValid())
	{
		//qDebug() << parent;
		//qDebug() << "create index for song";
		QList<Song *> *tmp;
		tmp = m_songList[m_mapKeyId.key(parent.row())];
		//qDebug() << "parent : " << tmp;
		return createIndex(row, column, static_cast<void *>(tmp));
	}
	else
	{
		//qDebug() << "create index for Category";
		return createIndex(row, column, static_cast<void *>(NULL));
	}
}

SongListTreeItemModel::SongListTreeItemModel()
{

}

SongListTreeItemModel::~SongListTreeItemModel()
{

}

void	SongListTreeItemModel::addSongList(const QString str, QList< Song* > *songList)
{
	m_songList[str] = songList;
	
	QMapIterator<QString, QList<Song *>*> it(m_songList);
	int i = 0;
	while (it.hasNext())
	{
		it.next();
		m_mapKeyId[it.key()] = i;
		qDebug() << "I : " << i << "Key : " << it.key() << "value" << it.value();
		i++;
	}
}

