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
    bool            setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int=Qt::EditRole)
    {return false;}
    bool            setData(const QModelIndex &index, const QVariant &value, int role)
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
