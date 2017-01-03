#pragma once

#include <QStyledItemDelegate>
#include <QPainter>


class CustomDelegate : public QStyledItemDelegate
{
public:
	CustomDelegate();
	~CustomDelegate();

protected:
	void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;


	
};

