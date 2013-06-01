/*
    This file is part of GuiliGuili software
    Copyright (C) 2013  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

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

#ifndef SONGTREEMODEL_H
#define SONGTREEMODEL_H

#include <QAbstractItemModel>
#include <QStringList>
#include "songtreeitem.h"
#include "song.h"

class SongTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit SongTreeModel(QObject *parent = 0);
    Qt::ItemFlags   flags(const QModelIndex &index) const;
    QVariant        data(const QModelIndex &index, int role) const;
    SongTreeItem*   itemForIndex(const QModelIndex& index) const;
    int             rowCount(const QModelIndex &parent) const;
    int             columnCount(const QModelIndex &parent) const;
    QModelIndex     index(int row, int column, const QModelIndex &parent) const;
    QModelIndex     parent(const QModelIndex &child) const;
    bool            setHeaderData(int , Qt::Orientation , const QVariant , int=Qt::EditRole)
    { return false;}
    bool            setData(const QModelIndex , const QVariant , int )
    {return false;}

    Qt::DropActions supportedDragActions() const;
    QMimeData*      mimeData(const QModelIndexList &indexes) const;
    QStringList     mimeTypes() const;
    QModelIndex     addSong(Song* sg, QString cat);
    void            clearCat(QString cat);
    
signals:
    
public slots:

private:
    SongTreeItem*    m_rootItem;
    
};

#endif // SONGTREEMODEL_H
