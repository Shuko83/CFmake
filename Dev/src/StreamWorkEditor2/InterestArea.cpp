/**
@file InterestArea.cpp
@brief Zone d'interet
@author F.Bighelli
 */

#include "InterestArea.h"
#include "StreamControler.h"

#define MARGE 5

/** @brief Constructor */
InterestArea::InterestArea(StreamControler * controler) {
	_bbox=QRectF(0,0,100,100);
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setZValue(-200.0);
    _color=QColor(QColor("#888888"));
    _text=new QGraphicsTextItem(this);
    _text->setPlainText("(click to edit)");
    _text->setPos(QPointF());
    _text->setTextInteractionFlags(Qt::TextEditable);
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
    if (option->state.testFlag(QStyle::State_Selected)) {
        painter->setPen(QPen(QColor(250,250,0)));
    } else {
        painter->setPen(QPen(Qt::NoPen));
    }
    painter->setBrush(QBrush(_color));
    painter->drawRect(_bbox);
    if (_hover) {
        painter->setPen(QPen(Qt::NoPen));
        painter->setBrush(QBrush(Qt::Dense5Pattern));
        QRectF r=QRectF(_bbox.width()-MARGE,0,MARGE,_bbox.height());
        painter->drawRect(r);
        r=QRectF(0,_bbox.height()-MARGE,_bbox.width(),MARGE);
        painter->drawRect(r);
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
        //
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
    } else
    if (abs(event->pos().x()-_bbox.width())<MARGE) {
        _hResize=true;
        _startPos=event->pos();
        _initialRect=_bbox;
        event->accept();
        prepareGeometryChange (); 
    } else 
    if (abs(event->pos().y()-_bbox.height())<MARGE) {
        _vResize=true;
        _startPos=event->pos();
        _initialRect=_bbox;
        event->accept();
        prepareGeometryChange (); 
    } else {
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
