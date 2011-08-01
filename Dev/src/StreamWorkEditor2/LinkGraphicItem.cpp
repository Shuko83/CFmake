/**
@file LinkGraphicItem.cpp
@brief Item pour la liaison
@author F.Bighelli
 */

#include "LinkGraphicItem.h"
#include <Qdebug>
#define LINK_WIDTH 2.0
#define SELECTED_LINK_WIDTH 3.0

#include "ManageLinkColor.h"

/** @brief Constructor */
LinkGraphicItem::LinkGraphicItem(ConnectorGraphicItem * source):QGraphicsItem() 
{
    setFlag(ItemIsSelectable);
    _source=source;
    _tmpTarget=source->mapToScene(QPointF(0.0,0.0));
    _target=0;
    _externalHighlight=false;
    setZValue(-100.0);
    _source->addLink(this);
    updateLink();
}

/** @brief Constructor */
LinkGraphicItem::LinkGraphicItem(ConnectorGraphicItem * source,ConnectorGraphicItem * target)
{
    setFlag(ItemIsSelectable);
    _source=source;
    _target=target;
    _externalHighlight=false;
    _source->addLink(this);
    _target->addLink(this);
    updateLink();
}

/** @brief Constructor */
LinkGraphicItem::~LinkGraphicItem()
{
    _source->removeLink(this);
    if (_target!=0)
        _target->removeLink(this);
}

/** @brief acces a la source */
ConnectorGraphicItem * LinkGraphicItem::getSource() 
{
    return _source;
}

/** @brief acces a la target */
ConnectorGraphicItem * LinkGraphicItem::getTarget() 
{
    return _target;
}
/** @brief definition du point temporaire */
void LinkGraphicItem::setTarget(ConnectorGraphicItem * target) {
    _target=target;
    updateLink();
}
/** @brief definition du point temporaire */
void LinkGraphicItem::setTemporaryTarget(QPointF & tmpTarget) {
    _tmpTarget=mapFromScene(tmpTarget);
    if (_target!=0)
        _target->removeLink(this);
    _target=0;
    updateLink();
}
/** @brief set highlignt */
void LinkGraphicItem::setExternalHighlight(bool highlight) {
    _externalHighlight=highlight;
}
/** @brief Fonction de dessin */
void LinkGraphicItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
    painter->setBrush(QBrush(Qt::NoBrush));
    painter->setRenderHints(QPainter::Antialiasing);
    QPen basePen;
    bool isPin=(_source->getConnectorType()==PIN);

	
    if (isSelected()) 
	{
        QPen pen=QPen(QPen(QColor("#FFFF00")));
        pen.setWidthF(SELECTED_LINK_WIDTH);
        painter->setPen(pen);
        painter->drawPath(_path);
        if (isPin) 
		{
            basePen=QPen(QColor("#5555FF"));
        } 
		else 
		{
			basePen=QPen(ManageLinkColor::getInstance()->getColorForInterface(_source->getModelType()));
        }
    } 
	else 
	{
        if (_externalHighlight) 
		{
            //basePen=QPen(QPen(QColor("#FF9900")));
            QPen pen=QPen(QPen(QColor("#FFFF00")));
            pen.setWidthF(SELECTED_LINK_WIDTH);
            painter->setPen(pen);
            painter->drawPath(_path);
            if (isPin) 
			{
                basePen=QPen(QColor("#5555FF"));
            } 
			else
			{
				basePen=QPen(ManageLinkColor::getInstance()->getColorForInterface(_source->getModelType()));
            }
        }
		else 
		{
            if (isPin) 
			{
                basePen=QPen(QColor(0x55,0x55,0xFF,200));
            }
			else 
			{
				QColor tmp = ManageLinkColor::getInstance()->getColorForInterface(_source->getModelType());
				tmp.setAlpha(128);
				basePen=QPen(tmp);
            }
        }
    }
    basePen.setWidthF(LINK_WIDTH);
    painter->setPen(basePen);
    painter->drawPath(_path);
    painter->setRenderHints(QPainter::Antialiasing,false);
}
/** @brief renvoie la forme */
QPainterPath LinkGraphicItem::shape () const {
    return _shapePath;
}
/** @brief Definition du rectangle englobant */
QRectF LinkGraphicItem::boundingRect() const {
    return _shapePath.controlPointRect ();
}
/** @brief mise a jour */
void LinkGraphicItem::updateLink() {
    /** @brief path */
    QPainterPath path;
    prepareGeometryChange(); 
    QPointF ssource=QPointF(0.0,0.0);
    setPos(_source->mapToScene(ssource));
    path.moveTo(ssource);
    if (_target==0) {
        QPointF cpoint=buildControlPoint(ssource,_source->getParentPosition(),100.0);
        path.quadTo(cpoint,_tmpTarget);
    } else {
        QPointF starget=mapFromItem(_target,QPointF(0.0,0.0));
        qreal decalage=sqrt((starget.x()-ssource.x())*(starget.x()-ssource.x())+(starget.y()-ssource.y())*(starget.y()-ssource.y()));
        decalage=decalage/1.618;
        //QPointF c1=buildControlPoint(ssource,_source->getParentPosition(),decalage);
        //QPointF c2=buildControlPoint(starget,_target->getParentPosition(),decalage);
        //path.cubicTo(c1,c2,starget);
        QPointF c1((ssource.x()+starget.x())/2.0,ssource.y());
        QPointF c2((ssource.x()+starget.x())/2.0,starget.y());
        if (_source->getParentPosition()==_target->getParentPosition()) {
            if (_source->getParentPosition()==RIGHT) {
                c1.setX(c1.x()+decalage);
                c2.setX(c2.x()+decalage);
            } else {
                c1.setX(c1.x()-decalage);
                c2.setX(c2.x()-decalage);
            }
        } 
        path.lineTo(c1);
        path.lineTo(c2);
        path.lineTo(starget);
    }
    _path=path;
    
    QPainterPathStroker ps;
    ps.setWidth(SELECTED_LINK_WIDTH);
    _shapePath = ps.createStroke(path);
    QRectF irect = mapToScene(boundingRect()).boundingRect();
    if (scene()!=0) {
        QRectF srect = scene()->sceneRect();
        if (!srect.contains(irect)) {
            srect = srect | irect;
            scene()->setSceneRect(srect);
        }
    }
    update();
}
/** @brief construction du point de controle */
QPointF LinkGraphicItem::buildControlPoint(QPointF & source,TConnectorPosition cpos,qreal decalage) {
    if (cpos==RIGHT) {
        return source+QPointF(decalage,0.0);
    } else {
        return source+QPointF(-decalage,0.0);
    }
}
