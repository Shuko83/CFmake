/*!
 \file _SwTreeItem.h
 \brief d'un item d'arbre de base
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#ifndef __SwTreeItem_H
#define __SwTreeItem_H

#include <QIcon>
#include <QList>
#include <QVariant>
#include <QFont>
#include <QColor>

class _SwTreeItem
{
private:
    QList<_SwTreeItem*> _childItems;
    QList<QVariant> _itemData;
    QIcon _itemIcon;
    _SwTreeItem *_parentItem;
    QColor _foreground;
    QFont _font;

public:
    _SwTreeItem(const QList<QVariant> &data,const QIcon & icon, _SwTreeItem *parent = 0,const QColor & foreground=QColor("black"),const QFont & font=QFont());
    ~_SwTreeItem();

    void appendChild(_SwTreeItem *child);

    _SwTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    QIcon icon() const;
    QColor foreground() const;
    QFont font() const;
    int row() const;
    _SwTreeItem *parent();

};

#endif
