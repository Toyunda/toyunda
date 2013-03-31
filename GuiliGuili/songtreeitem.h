#ifndef SONGTREEITEM_H
#define SONGTREEITEM_H

#include <QList>

class SongTreeItem
{
public:
    SongTreeItem();
    void*   data;
    QList<SongTreeItem*>    childs;
    SongTreeItem*   parent;
};

#endif // SONGTREEITEM_H
