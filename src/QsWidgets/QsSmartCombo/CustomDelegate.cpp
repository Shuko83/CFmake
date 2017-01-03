#include "CustomDelegate.h"

#include <QTextDocument>
#include <QApplication>


//-------------------------------------------------------------------------
CustomDelegate::CustomDelegate() 
{

}


//-------------------------------------------------------------------------
CustomDelegate::~CustomDelegate()
{
}

//-------------------------------------------------------------------------
void CustomDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QStyleOptionViewItemV4 options = option;
	initStyleOption(&options, index);

	QPainter *_painter = painter;

	_painter->save();

	QTextDocument doc;
	doc.setHtml(options.text);

	options.text = "";

	const QWidget *widget = option.widget;
	QStyle *style = widget ? widget->style() : QApplication::style();
	style->drawControl(QStyle::CE_ItemViewItem, &options, painter, widget);

	doc.setDefaultFont(options.font);
	
	_painter->translate(options.rect.left(), options.rect.top());
	QRect clip(0, 0, options.rect.width(), options.rect.height());
	doc.drawContents(_painter);// , clip);

	_painter->restore();
}

//-------------------------------------------------------------------------
QSize CustomDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QStyleOptionViewItemV4 options = option;
	initStyleOption(&options, index);

	QTextDocument doc;
	doc.setHtml(options.text);
	doc.setTextWidth(options.rect.width());

	return QSize(doc.idealWidth(), doc.size().height());
}
