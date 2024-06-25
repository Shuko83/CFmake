/*!
 \file _SwTreeItem.cpp
 \brief d'un item d'arbre de base
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#include <QStringList>

#include "Component/Base/_SwTreeItem.h"

_SwTreeItem::_SwTreeItem(const QList<QVariant> &data,const QIcon & icon, _SwTreeItem *parent,const QColor & foreground,const QFont & font)
{
    _parentItem = parent;
    _itemData = data;
    _itemIcon= icon;
    _foreground=foreground;
    _font=font;
}

_SwTreeItem::~_SwTreeItem()
{
    qDeleteAll(_childItems);
}

void _SwTreeItem::appendChild(_SwTreeItem *item)
{
    _childItems.append(item);

}

_SwTreeItem *_SwTreeItem::child(int row)
{
    return _childItems.value(row);
}

int _SwTreeItem::childCount() const
{
    return _childItems.count();
}

int _SwTreeItem::columnCount() const
{
    return _itemData.count();
}

QVariant _SwTreeItem::data(int column) const
{
    return _itemData.value(column);
}
QIcon _SwTreeItem::icon() const {
    return _itemIcon;
}
QColor _SwTreeItem::foreground() const {

    return _foreground;
}
QFont _SwTreeItem::font() const {
    return _font;
}
_SwTreeItem *_SwTreeItem::parent()
{
    return _parentItem;
}
int _SwTreeItem::row() const
{
    if (_parentItem)
        return _parentItem->_childItems.indexOf(const_cast<_SwTreeItem*>(this));

    return 0;
}
