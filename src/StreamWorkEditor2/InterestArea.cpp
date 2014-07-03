/**
@file InterestArea.cpp
@brief Zone d'interet
@author F.Bighelli
 */

#include "InterestArea.h"
#include "StreamControler.h"
#include <QTextCursor>

#define MARGE 15

/** @brief Constructor */
InterestArea::InterestArea(StreamControler * controler) {
	_bbox=QRectF(0,0,100,100);
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);

    setZValue(-200.0);
    _color=QColor(QColor(136,136,136,136));
    _text=new QGraphicsTextItem(this);
    _text->setPlainText("Text");
    QFont font=_text->font();
    font.setPointSize(18);
    _text->setFont(font);
    _text->setPos(QPointF());
    _text->setTextInteractionFlags(Qt::TextEditable | Qt::TextBrowserInteraction);
    _text->setDefaultTextColor(QColor("#FFFFFF"));
	
    //_text->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    _vResize=false;
    _hResize=false;
    setAcceptHoverEvents(true);
    _hover=false;
    _controler=controler;
}
/** @brief Destructor */
InterestArea::~InterestArea() {

}
/** @brief Definition du rectangle englobant */
QRectF InterestArea::boundingRect() const {
    return _bbox;
}
/** @brief Fonction de dessin */
void InterestArea::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) {
    
	painter->save();

    if (option->state.testFlag(QStyle::State_Selected)) {
		QPen tmp = QPen(QColor("#fcbd00"));
		tmp.setWidth(2);
		painter->setPen(tmp);
		
    } else {
        painter->setPen(QPen(Qt::NoPen));
    }
    painter->setBrush(QBrush(_color));
    painter->drawRect(_bbox);

	painter->restore();

    if (_hover) 
	{
        painter->setPen(QPen(Qt::white));
		painter->drawLine(_bbox.width()-15,_bbox.height()-2,_bbox.width()-2,_bbox.height()-15); 
		painter->drawLine(_bbox.width()-10,_bbox.height()-2,_bbox.width()-2,_bbox.height()-10); 
		painter->drawLine(_bbox.width()-5,_bbox.height()-2,_bbox.width()-2,_bbox.height()-5); 

    }
}
/** @brief mise a jour des attributs de l'item */
void InterestArea::updateAttributs() {

}
/** @brief sur double click dans l'item */
void InterestArea::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event ) {
    QColor color=QColorDialog::getColor(_color,0,"Select color",QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
        _color=color;
        if (qGray(_color.rgb())>128) {
            _text->setDefaultTextColor(QColor("#000000"));
        } else {
            _text->setDefaultTextColor(QColor("#FFFFFF"));
        }
        update();
        _controler->streamControlerChanged();
    }
}
/** @brief sur mouvement sourie */
void InterestArea::mouseMoveEvent ( QGraphicsSceneMouseEvent * event ) {
    if (!_vResize && !_hResize) {
        QGraphicsItem::mouseMoveEvent(event);
    } else {
        QPointF p=event->pos();
        _bbox=_initialRect;
        if (_hResize) {
            int newWidth=_bbox.width()+p.x()-_startPos.x();
            if (newWidth<10)
                    newWidth=10;
            _bbox.setWidth(newWidth);
        }
        if (_vResize) {
            int newHeight=_bbox.height()+p.y()-_startPos.y();
            if (newHeight<10)
                    newHeight=10;
            _bbox.setHeight(newHeight);
        }
		
        update();
    }
}

/** @brief sur press sourie */
void InterestArea::mousePressEvent ( QGraphicsSceneMouseEvent * event ){
    if (abs(event->pos().x()-_bbox.width())<MARGE && abs(event->pos().y()-_bbox.height())<MARGE) {
        _vResize=true;
        _hResize=true;
        _startPos=event->pos();
        _initialRect=_bbox;
        event->accept();
        prepareGeometryChange (); 
    } 
	else 
	{
        QGraphicsItem::mousePressEvent(event);
    }

}
/** @brief sur release sourie */
void InterestArea::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ){
    if (_vResize || _hResize) {
        QPointF p=event->pos();
        _bbox=_initialRect;
        if (_hResize) {
            int newWidth=_bbox.width()+p.x()-_startPos.x();
            if (newWidth<10)
                    newWidth=10;
            _bbox.setWidth(newWidth);
        }
        if (_vResize) {
            int newHeight=_bbox.height()+p.y()-_startPos.y();
            if (newHeight<10)
                    newHeight=10;
            _bbox.setHeight(newHeight);
        }
        _vResize=false;
        _hResize=false;
        update();
        event->accept();
    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
    _controler->streamControlerChanged();
}
/** @brief sur hover enter */
void InterestArea::hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) {
    _hover=true;
    update();
}
/** @brief sur hover leave */
void InterestArea::hoverLeaveEvent  ( QGraphicsSceneHoverEvent * event ) {
    _hover=false;
    update();
}
/** @brief getter et setter color*/
QColor InterestArea::getColor() {
    return _color;
}
void InterestArea::setColor(QColor c){
    _color=c;
    if (qGray(_color.rgb())>128) {
        _text->setDefaultTextColor(QColor("#000000"));
    } else {
        _text->setDefaultTextColor(QColor("#FFFFFF"));
    }
    update();
    _controler->streamControlerChanged();
}
/** @brief getter et setter text*/
QString InterestArea::getText() const{
    return _text->toPlainText();
}
void InterestArea::setText(QString t){
    _text->setPlainText(t);
    _text->update();
    _controler->streamControlerChanged();
}
/** @brief getter et setter color*/
void InterestArea::setBoundingRect(QRectF bbox){
    _bbox=bbox;
}
/** @brief sur changement de l'item */
QVariant InterestArea::itemChange ( GraphicsItemChange change,
                                   const QVariant & value ) {
    QVariant result=QGraphicsItem::itemChange(change, value);
    if (change == ItemPositionHasChanged) {
		if(!childItems().isEmpty())
		{
			foreach(QGraphicsItem *item, childItems())
			{
				ComponentGraphicItem * component = dynamic_cast<ComponentGraphicItem*>(item);
				if(component)
				{
					component->updateAttributs();
				}
			}
		}
        //_controler->streamControlerChanged();
    }
    return result;
}
/** @brief renvoie tout les component graphic item qui touche la zone d'interet */
QList<ComponentGraphicItem *> InterestArea::getRelatedComponents() {
    QList<QGraphicsItem *> items=collidingItems();
    QList<ComponentGraphicItem *> result;
    for(int i=0;i<items.count();i++) {
        ComponentGraphicItem * cgitem=dynamic_cast<ComponentGraphicItem *>(items.at(i));
        if (cgitem!=0) {
            result.push_back(cgitem);
        }
    }
    return result;
}
