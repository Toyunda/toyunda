#include "songtreemodel.h"
#include <qmimedata.h>

SongTreeModel::SongTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = NULL;
}

Qt::ItemFlags SongTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags   theFlags = QAbstractItemModel::flags(index);
    if (index.isValid())
    {
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
    }
    return theFlags;
}

QVariant SongTreeModel::data(const QModelIndex &index, int role) const
{
    if (!m_rootItem || !index.isValid() || index.column() < 0 || index.column() > 1)
        return QVariant();
    if (SongTreeItem *item = itemForIndex(index))
    {
        if (role == Qt::DisplayRole)
        {
            if (item->childs.isEmpty())
            {
                Song *sg = static_cast<Song *>(item->data);
                return sg->title;
            }
            else
            {
                if (item != m_rootItem)
                {
                    QString* cat = static_cast<QString *>(item->data);
                    return *cat;
                }
                return QVariant();
            }
        }
        if (role == Qt::ToolTipRole)
        {
            if (item->childs.isEmpty())
            {
                Song *sg = static_cast<Song *>(item->data);
                return sg->iniFile;
            }
            else
            {
                if (item != m_rootItem)
                {
                    QString* cat = static_cast<QString *>(item->data);
                    return *cat;
                }
                return QVariant();
            }
        }
    }
    return QVariant();
}

SongTreeItem *SongTreeModel::itemForIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (SongTreeItem* item = static_cast<SongTreeItem*>(index.internalPointer()))
            return item;
    }
    return m_rootItem;
}

int SongTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return 0;
    SongTreeItem* item = itemForIndex(parent);
    return item ? item->childs.count() : 0;
}

int SongTreeModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() && parent.column() != 0 ? 0 : 1;
}

QModelIndex SongTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!m_rootItem || row < 0 || column < 0 || column >= 1 || (parent.isValid() && parent.column() != 0))
        return QModelIndex();
    SongTreeItem*   parentItem = itemForIndex(parent);
    if (SongTreeItem* item = parentItem->childs.at(row))
        return createIndex(row, column, item);
    return QModelIndex();
}

QModelIndex SongTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();
    if (SongTreeItem* childItem = itemForIndex(child))
    {
        if (SongTreeItem* parentItem = childItem->parent)
        {
            if (parentItem == m_rootItem)
                return QModelIndex();
            if (SongTreeItem* gpItem = parentItem->parent)
            {
                int row = gpItem->childs.indexOf(parentItem);
                return createIndex(row, 0, parentItem);
            }
        }
    }
    return QModelIndex();
}

Qt::DropActions SongTreeModel::supportedDragActions() const
{
    return Qt::CopyAction;
}

QMimeData *SongTreeModel::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.count() < 0)
        return 0;
    QListIterator<QModelIndex> it(indexes);
    QList<Song> songs;
    while (it.hasNext())
    {
        SongTreeItem *item = itemForIndex(it.next());
        if (item->childs.isEmpty())
        {
            Song*   sg = (Song*) item->data;
            songs.append(*sg);
        }
        else
        {
            QListIterator<SongTreeItem*>   it2(item->childs);
            while (it2.hasNext())
            {
                SongTreeItem* tmpItem = it2.next();
                Song*   sg = (Song*) tmpItem->data;
                songs.append(*sg);
            }

        }
    }
    QMimeData*  toret = new QMimeData();
    QByteArray  data = Song::listSerialize(songs);
    toret->setData(SongListMimeType, data);
    toret->setText(songs.at(0).title);
    return toret;
}

QStringList SongTreeModel::mimeTypes() const
{
    return QStringList() << SongListMimeType;
}

QModelIndex SongTreeModel::addSong(Song *sg, QString cat)
{
    SongTreeItem*   catItem;

    bool    catFound = false;

    if (m_rootItem == NULL)
        m_rootItem = new SongTreeItem();
    QListIterator<SongTreeItem*>it(m_rootItem->childs);

    while (it.hasNext())
    {
            catItem = it.next();
            QString* ncat = static_cast<QString*>(catItem->data);
            if (*ncat == cat)
            {
                catFound = true;
                break;
            }
    }
    if (!catFound)
    {
        catItem = new SongTreeItem();
        catItem->parent = m_rootItem;
        catItem->data = new QString(cat);
        m_rootItem->childs.append(catItem);
    }
    SongTreeItem*   newItem = new SongTreeItem();
    newItem->data = sg;
    newItem->parent = catItem;
    catItem->childs.append(newItem);

    /*if (catFound)
        beginInsertRows(createIndex(m_rootItem->childs.indexOf(catItem), 0, catItem), catItem->childs.count() - 1, catItem->childs.count() - 1);
    else
        beginInsertRows(createIndex(0, 0, m_rootItem), m_rootItem->childs.indexOf(catItem), m_rootItem->childs.indexOf(catItem));
    endInsertRows();*/
    reset();
    return createIndex(catItem->childs.count() - 1, 0, newItem);
}

void SongTreeModel::clearCat(QString cat)
{
    if (!m_rootItem)
        return;
    QMutableListIterator<SongTreeItem*> it(m_rootItem->childs);
    SongTreeItem*   catItem;
    while (it.hasNext())
    {
        catItem = it.next();
        QString* ncat = static_cast<QString*>(catItem->data);
        if (*ncat == cat)
        {
            catItem->childs.clear();
            delete catItem;
            it.remove();
            return ;
        }
    }
}
