/**
@file ConnectorGraphicItem.cpp
@brief Item graphique correspondant a un connecteur
@author F.Bighelli
 */

#include "ConnectorGraphicItem.h"
#include "ComponentGraphicItem.h"
#include "GraphicsResources.h"
#include "LinkGraphicItem.h"

#include "ManageLinkColor.h"


/** @brief Constructor */
ConnectorGraphicItem::ConnectorGraphicItem(QGraphicsItem * parent,QString name,QString type,TConnectorType ctype):
        QGraphicsItem(parent),
        _name(name),
        _type(type),
        _ctype(ctype)
{
    setToolTip(name);
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
    _cpos=UNDEFINED;
    _highlightExtern=false;
    _showHover=false;
    setZValue(0.0);
}
/** @brief Definition du rectangle englobant */
QRectF ConnectorGraphicItem::boundingRect() const {
    return QRectF(-CL_CONNECTOR_SIZE/2.0,-CL_CONNECTOR_SIZE/2.0,CL_CONNECTOR_SIZE,CL_CONNECTOR_SIZE);
}
/** @brief Fonction de dessin */
void ConnectorGraphicItem::paint ( QPainter * painter,
                                   const QStyleOptionGraphicsItem * option,
                                   QWidget * widget) {
    QPoint p;
    QPen pendraw=QPen(ManageLinkColor::getInstance()->getColorForInterface(_type));
    pendraw.setWidth(CL_CONNECTOR_SIZE/5.0);
    QColor color_base=((ComponentGraphicItem *)parentItem())->getColor();                
    QColor color;
    //color.setHsv((color_base.hue()+85)%255,color_base.saturation(),color_base.value());
    color.setHsv(color_base.hue(),color_base.saturation(),(255-color_base.value()));
    QColor ccolor;
    //ccolor.setHsv((color_base.hue()+170)%255,color_base.saturation(),color_base.value());
    ccolor.setHsv((color_base.hue()+128)%255,color_base.saturation(),color_base.value());

    painter->setRenderHints(QPainter::Antialiasing);
    
    switch(_ctype) {
        case PROVIDER:
            //painter->setPen(QPen(QColor("#888888")));
            //painter->setBrush(QBrush(ccolor));
            //painter->drawEllipse(QRectF(-CL_CONNECTOR_SIZE/2.0,-CL_CONNECTOR_SIZE/2.0,CL_CONNECTOR_SIZE,CL_CONNECTOR_SIZE));
            //painter->setBrush(QColor(64,64,64,100));
            //painter->drawEllipse(QRectF(-CL_CONNECTOR_SIZE/4.0,-CL_CONNECTOR_SIZE/4.0,CL_CONNECTOR_SIZE/2.0,CL_CONNECTOR_SIZE/2.0));
            //painter->setPen(QPen(QColor("#888888")));
            painter->setPen(pendraw);
			painter->setBrush(QBrush(ManageLinkColor::getInstance()->getColorForInterface(_type)));
            painter->drawEllipse(QRectF(-CL_CONNECTOR_SIZE/2.0,-CL_CONNECTOR_SIZE/2.0,CL_CONNECTOR_SIZE,CL_CONNECTOR_SIZE));
            painter->setPen(pendraw);
            break;
        case CONSUMER:
            painter->setPen(QPen(QColor(ManageLinkColor::getInstance()->getColorForInterface(_type))));
            painter->setBrush(QBrush());
            painter->drawEllipse(QRectF(-CL_CONNECTOR_SIZE/2.0,-CL_CONNECTOR_SIZE/2.0,CL_CONNECTOR_SIZE,CL_CONNECTOR_SIZE));
            break;
        case PIN:
            painter->setPen(QPen(QColor("#888888")));
            painter->setBrush(QBrush(QColor("#5555FF")));
            painter->drawRect(QRectF(-CL_CONNECTOR_SIZE/2.0,-CL_CONNECTOR_SIZE/2.0,CL_CONNECTOR_SIZE,CL_CONNECTOR_SIZE));
            break;
    }
    QString texte;
    if (isSelected() || _showHover) {
        if (_cpos==LEFT) {
            texte=QString("[%2] %1").arg(_name).arg(_type);
        } else {
            texte=QString("%1 [%2]").arg(_name).arg(_type);
        }
        QPen pen;
        if (isSelected()) {
            pen=QPen(QColor("#FFFFFF"));
        } else {
            pen=QPen(QColor("#AAAAAA"));
        }
        pen.setWidth(2);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        if(_ctype==CONSUMER || _ctype==PROVIDER)
            painter->drawEllipse(QRectF(-CL_CONNECTOR_SIZE/2.0,-CL_CONNECTOR_SIZE/2.0,CL_CONNECTOR_SIZE,CL_CONNECTOR_SIZE));
        if(_ctype==PIN)
            painter->drawRect(QRectF(-CL_CONNECTOR_SIZE/2.0,-CL_CONNECTOR_SIZE/2.0,CL_CONNECTOR_SIZE,CL_CONNECTOR_SIZE));
    } else { //if (parentItem()->isSelected() || _highlightExtern || _showHover) {
        texte=QString("%1").arg(_name);
    }
    painter->setRenderHints(QPainter::Antialiasing,false);
    if (!texte.isEmpty()) {
        QFontMetrics fm(QApplication::fontMetrics());
        if (_cpos==RIGHT) {
            p.setX((int)CL_CONNECTOR_SIZE);
            p.setY(fm.height()/4);
            painter->drawText(p,texte);
        } else {
            p.setX((int)(-CL_CONNECTOR_SIZE-fm.width(texte)));
            p.setY(fm.height()/4);
            painter->drawText(p,texte);
        }
    }

}
/** @brief acces au nom */
QString ConnectorGraphicItem::getName() {
    return _name;
}
/** @brief acces au type modele*/
QString ConnectorGraphicItem::getModelType() {
    return _type;
}
/** @brief acces au type */
TConnectorType ConnectorGraphicItem::getConnectorType() {
    return _ctype;
}
/** @brief definition de la position dans le parent */
void ConnectorGraphicItem::setParentPosition(TConnectorPosition position) {
    _cpos=position;
}
/** @brief acces a la position dans le parent */
TConnectorPosition ConnectorGraphicItem::getParentPosition() {
    return _cpos;
}
/** @brief inversion la mise en evidence externe */
void ConnectorGraphicItem::toggleHighlightExtern() {
    _highlightExtern=!_highlightExtern;
}
/** @brief inversion la mise en evidence du hover */
void ConnectorGraphicItem::toggleHover() {
    _showHover=!_showHover;
    update();

}
/** @brief Ajouter un lien */
void ConnectorGraphicItem::addLink(LinkGraphicItem * link) {
    _links.push_back(link);
}
/** @brief Ajouter un lien */
void ConnectorGraphicItem::removeLink(LinkGraphicItem * link) {
    _links.removeOne(link);
}
/** @brief getLinks */
QList<LinkGraphicItem *> * ConnectorGraphicItem::getLinks() {
    return &_links;
}
/** @brief mise a jour des liens */
void ConnectorGraphicItem::updateLinks() {
    QList<LinkGraphicItem *>::iterator it=_links.begin();
    while(it!=_links.end()) {
        (*it)->updateLink();
        it++;
    }
}

/** @brief sur changement de l'item */
QVariant ConnectorGraphicItem::itemChange ( GraphicsItemChange change, const QVariant & value ) {
    QVariant result=QGraphicsItem::itemChange(change, value);
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
    QList<LinkGraphicItem *>::iterator it=_links.begin();
        if (isSelected()) {
            //Si selected
            while(it!=_links.end()) {
                (*it)->setExternalHighlight(true);
                it++;
            }
        } else {
            //Si non selected
            while(it!=_links.end()) {
                (*it)->setExternalHighlight(false);
                it++;
            }
        }
    }
    return result;
}
/** @brief sur enter hover */
void ConnectorGraphicItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) {
    _showHover=true;
    update();
}
/** @brief sur leave hover */
void ConnectorGraphicItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) {
    _showHover=false;
    update();
}
