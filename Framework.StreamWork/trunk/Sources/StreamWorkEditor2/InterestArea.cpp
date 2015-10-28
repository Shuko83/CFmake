/**
@file InterestArea.cpp
@brief Zone d'interet
@author F.Bighelli
*/

#include "InterestArea.h"
#include "StreamControler.h"
#include <QTextCursor>

#define MARGE 15

//-----------------------------------------------------------------------
InterestArea::InterestArea(StreamControler * controler)
{
	_bbox = QRectF(0, 0, 100, 100);
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);

	setZValue(-200.0);
	_color = QColor(QColor(136, 136, 136, 136));
	_text = new QGraphicsTextItem(this);
	_text->setPlainText("Text");
	QFont font = _text->font();
	font.setPointSize(18);
	_text->setFont(font);
	_text->setPos(QPointF());
	_text->setTextInteractionFlags(Qt::TextEditable | Qt::TextBrowserInteraction);
	_text->setDefaultTextColor(QColor("#FFFFFF"));

	//_text->setFlag(QGraphicsItem::ItemIgnoresTransformations);
	_vResize = false;
	_hResize = false;
	setAcceptHoverEvents(true);
	_hover = false;
	_controler = controler;
}

//-----------------------------------------------------------------------
InterestArea::~InterestArea()
{

}

//-----------------------------------------------------------------------
QRectF InterestArea::boundingRect() const
{
	return _bbox;
}

//-----------------------------------------------------------------------
void InterestArea::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{

	painter->save();

	if ( option->state.testFlag(QStyle::State_Selected) )
	{
		QPen tmp = QPen(QColor("#fcbd00"));
		tmp.setWidth(2);
		painter->setPen(tmp);

	}
	else
	{
		painter->setPen(QPen(Qt::NoPen));
	}
	painter->setBrush(QBrush(_color));
	painter->drawRect(_bbox);

	painter->restore();

	if ( _hover )
	{
		painter->setPen(QPen(Qt::white));
		painter->drawLine(_bbox.width() - 15, _bbox.height() - 2, _bbox.width() - 2, _bbox.height() - 15);
		painter->drawLine(_bbox.width() - 10, _bbox.height() - 2, _bbox.width() - 2, _bbox.height() - 10);
		painter->drawLine(_bbox.width() - 5, _bbox.height() - 2, _bbox.width() - 2, _bbox.height() - 5);

	}
}

//-----------------------------------------------------------------------
void InterestArea::updateAttributs()
{

}

//-----------------------------------------------------------------------
void InterestArea::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	QColor color = QColorDialog::getColor(_color, 0, "Select color", QColorDialog::ShowAlphaChannel);
	if ( color.isValid() )
	{
		_color = color;
		if ( qGray(_color.rgb()) > 128 )
		{
			_text->setDefaultTextColor(QColor("#000000"));
		}
		else
		{
			_text->setDefaultTextColor(QColor("#FFFFFF"));
		}
		update();
		_controler->streamControlerChanged();
	}
}

//-----------------------------------------------------------------------
void InterestArea::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	if ( !_vResize && !_hResize )
	{
		QGraphicsItem::mouseMoveEvent(event);
	}
	else
	{
		QPointF p = event->pos();
		_bbox = _initialRect;
		if ( _hResize )
		{
			int newWidth = _bbox.width() + p.x() - _startPos.x();
			if ( newWidth < 10 )
				newWidth = 10;

			newWidth = floor(newWidth / 5)*5;

			_bbox.setWidth(newWidth);
		}
		if ( _vResize )
		{
			int newHeight = _bbox.height() + p.y() - _startPos.y();
			if ( newHeight < 10 )
				newHeight = 10;
			newHeight = floor(newHeight / 5) * 5;
			_bbox.setHeight(newHeight);
		}

		update();
	}
}


//-----------------------------------------------------------------------
void InterestArea::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if ( abs(event->pos().x() - _bbox.width()) < MARGE && abs(event->pos().y() - _bbox.height()) < MARGE )
	{
		_vResize = true;
		_hResize = true;
		_startPos = event->pos();
		_initialRect = _bbox;
		event->accept();
		prepareGeometryChange();
	}
	else
	{
		QGraphicsItem::mousePressEvent(event);
	}

}

//-----------------------------------------------------------------------
void InterestArea::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if ( _vResize || _hResize )
	{
		QPointF p = event->pos();
		_bbox = _initialRect;
		if ( _hResize )
		{
			int newWidth = _bbox.width() + p.x() - _startPos.x();
			if ( newWidth < 10 )
				newWidth = 10;
			_bbox.setWidth(newWidth);
		}
		if ( _vResize )
		{
			int newHeight = _bbox.height() + p.y() - _startPos.y();
			if ( newHeight < 10 )
				newHeight = 10;
			_bbox.setHeight(newHeight);
		}
		_vResize = false;
		_hResize = false;
		update();
		event->accept();
	}
	else
	{
		QGraphicsItem::mouseReleaseEvent(event);
	}
	_controler->streamControlerChanged();
}

//-----------------------------------------------------------------------
void InterestArea::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	_hover = true;
	update();
}

//-----------------------------------------------------------------------
void InterestArea::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	_hover = false;
	update();
}

//-----------------------------------------------------------------------
QColor InterestArea::getColor()
{
	return _color;
}

//-----------------------------------------------------------------------
void InterestArea::setColor(QColor c)
{
	_color = c;
	if ( qGray(_color.rgb()) > 128 )
	{
		_text->setDefaultTextColor(QColor("#000000"));
	}
	else
	{
		_text->setDefaultTextColor(QColor("#FFFFFF"));
	}
	update();
	_controler->streamControlerChanged();
}

//-----------------------------------------------------------------------
QString InterestArea::getText() const
{
	return _text->toPlainText();
}

//-----------------------------------------------------------------------
void InterestArea::setText(QString t)
{
	_text->setPlainText(t);
	_text->update();
	_controler->streamControlerChanged();
}

//-----------------------------------------------------------------------
void InterestArea::setBoundingRect(QRectF bbox)
{
	_bbox = bbox;
}
/** @brief sur changement de l'item */
QVariant InterestArea::itemChange(GraphicsItemChange change,const QVariant & value)
{
	QVariant result = QGraphicsItem::itemChange(change, value);
	if ( change == ItemPositionHasChanged )
	{
		if ( !childItems().isEmpty() )
		{
			//-----------------------------------------------------------------------
			foreach(QGraphicsItem *item, childItems())
			{
				ComponentGraphicItem * component = dynamic_cast<ComponentGraphicItem*>(item);
				if ( component )
				{
					component->updateAttributs();
				}
			}
		}
		//_controler->streamControlerChanged();
	}

	if ( change == ItemPositionChange )
	{
		QPointF newPos = value.toPointF();
		QPointF closestPoint = computeTopLeftGridPoint(newPos);
		return  closestPoint;
	}

	return result;
}

//-----------------------------------------------------------------------
QList<ComponentGraphicItem *> InterestArea::getRelatedComponents()
{
	QList<QGraphicsItem *> items = collidingItems();
	QList<ComponentGraphicItem *> result;
	for ( int i = 0; i < items.count(); i++ )
	{
		ComponentGraphicItem * cgitem = dynamic_cast<ComponentGraphicItem *>(items.at(i));
		if ( cgitem != 0 )
		{
			result.push_back(cgitem);
		}
	}
	return result;
}

//-----------------------------------------------------------------------
QPointF InterestArea::computeTopLeftGridPoint(const QPointF& pointP)
{
	QSize gridSize(5, 5);
	qreal xV = floor(pointP.x() / gridSize.width())*gridSize.width();
	qreal yV = floor(pointP.y() / gridSize.height())*gridSize.height();
	return QPointF(xV, yV);
}
