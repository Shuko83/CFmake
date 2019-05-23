/**
@file LinkGraphicItem.h
@brief Item pour la liaison
@author F.Bighelli
 */

#ifndef _LINKGRAPHICITEM_H
#define _LINKGRAPHICITEM_H

#include <QtCore>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "ConnectorGraphicItem.h"

/**
@class LinkGraphicItem
@brief Item pour la liaison
*/
class LinkGraphicItem : public QGraphicsItem{

public:
	/** @brief Constructor */
	LinkGraphicItem(ConnectorGraphicItem * source);
	/** @brief Constructor */
	LinkGraphicItem(ConnectorGraphicItem * source,ConnectorGraphicItem * target);
	/** @brief Constructor */
	~LinkGraphicItem();
    /** @brief Fonction de dessin */
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    /** @brief renvoie la forme */
    QPainterPath shape () const;
    /** @brief Definition du rectangle englobant */
    QRectF boundingRect() const;
    /** @brief mise a jour */
    void updateLink();
    /** @brief acces a la source */
    ConnectorGraphicItem * getSource();
    /** @brief acces a la target */
    ConnectorGraphicItem * getTarget();
    /** @brief definition du point temporaire */
    void setTarget(ConnectorGraphicItem * target);
    /** @brief definition du point temporaire */
    void setTemporaryTarget(const QPointF & tmpTarget);
    /** @brief set highlignt */
    void setExternalHighlight(bool highlight);
private:
    /** @brief construction du point de controle */
    QPointF buildControlPoint(QPointF & source,TConnectorPosition cpos,qreal decalage);
private:
    /** @brief source */
    ConnectorGraphicItem * _source;
    /** @brief target */
    ConnectorGraphicItem * _target;
    /** @brief temporary target */
    QPointF _tmpTarget;
    /** @brief path */
    QPainterPath _path;
    /** @brief path */
    QPainterPath _shapePath;
    /** @brief externalHighlight */
    bool _externalHighlight;

};

#endif
