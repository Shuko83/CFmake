/**
@file InterestArea.h
@brief Zone d'interet
@author F.Bighelli
 */

#ifndef _INTERESTAREA_H
#define _INTERESTAREA_H


#include <QtCore>
#include <QtGui>
#include "ComponentGraphicItem.h"

class StreamControler;
/**
@class InterestArea
@brief Zone d'interet
*/
class InterestArea : public QGraphicsItem {

public:
	/** @brief Constructor */
	InterestArea(StreamControler * controler);
	/** @brief Destructor */
	~InterestArea();
    /** @brief Definition du rectangle englobant */
    QRectF boundingRect() const;
    /** @brief Fonction de dessin */
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    /** @brief mise a jour des attributs de l'item */
    void updateAttributs();
    /** @brief getter et setter color*/
    QColor getColor();
    void setColor(QColor c);
    /** @brief getter et setter text*/
    QString getText() const;
    void setText(QString t);
    /** @brief getter et setter color*/
    void setBoundingRect(QRectF bbox);
    /** @brief renvoie tout les component graphic item qui touche la zone d'interet */
    QList<ComponentGraphicItem *> getRelatedComponents();
protected:
    /** @brief sur changement de l'item */
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant & value );
    /** @brief sur double click dans l'item */
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );
    /** @brief sur mouvement sourie */
    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    /** @brief sur press sourie */
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    /** @brief sur release sourie */
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /** @brief sur hover enter */
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    /** @brief sur hover leave */
    virtual void hoverLeaveEvent  ( QGraphicsSceneHoverEvent * event );

private:
    /** @brief Bounding rect */
    mutable QRectF _bbox;
    /** @brief Couleur */
    QColor _color;
    /** @brief Zone de texte */
    QGraphicsTextItem * _text;
    /** @brief resize */
    bool _vResize;
    bool _hResize;
    QPointF _startPos;
    QRectF _initialRect;
    /** @brief bool de hover */
    bool _hover;
    /** @breif acces au controleur */
    StreamControler * _controler;
};

#endif
