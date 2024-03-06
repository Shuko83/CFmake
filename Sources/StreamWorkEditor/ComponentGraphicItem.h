/**
@file ComponentGraphicItem.h
@brief Item Graphique correspondant à un composant
@author F.Bighelli
 */

#ifndef _COMPONENTGRAPHICITEM_H
#define _COMPONENTGRAPHICITEM_H

#include <QtCore>
#include <QtWidgets>
#include "Component/Base/SwComponent_Class.h"
#include "ISwExecutable_Service.h"
#include "ISwExecution_Service.h"
#include "ConnectorGraphicItem.h"

#define HEADER_COLOR QColor(0,0,128)
#define TEXT_COLOR QColor(255,255,255)


class StreamControler;
/**
@class ComponentGraphicItem
@brief Item Graphique correspondant à un composant
*/
class ComponentGraphicItem : public QGraphicsItem {

public:
	/** @brief Constructor */
    ComponentGraphicItem(StreamWork::SwCore::SwComponent_Class * component,StreamControler * controler);
    
    /** @brief Acces au composant */
    StreamWork::SwCore::SwComponent_Class * getComponent() const;

    /** @brief Definition du rectangle englobant */
    QRectF boundingRect() const;
    /** @brief Fonction de dessin */
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    /** @brief mise a jour des attributs de l'item */
    void updateAttributs();
    /** @brief acces au connecteurs */
    QList<ConnectorGraphicItem *> * getConnectors();
    /** @brief acces au composants enfants */
    QList<ComponentGraphicItem *> getChildsComponents();
    /** @brief acces a un connecteur */
    ConnectorGraphicItem * getConnector(QString name);
    /** @brief definition de la couleur */
    QColor getColor();
    /** @brief acces a la couleur */
    void setColor(QColor c);
    /** @brief definition de la couleur de texte */
    QColor getTextColor();
    /** @brief acces a la couleur de texte*/
    void setTextColor(QColor c);
    /** @brief show connector insertion */
    void showConnectorInsertion(QPointF pci);
    /** @brief hide connector insertion */
    void hideConnectorInsertion();
    /** @brief move connector  */
    void moveConnector(ConnectorGraphicItem *citem);
    //--------------------------------------------------------------------------
    // Methodes pour le rangement 
    //--------------------------------------------------------------------------
    /** @brief est Producteur*/
    bool isProducer();
    /** @brief est Producteur */
    bool isProducerButIsntConsumed();
	/** @brief Align sur la grille */
	QPointF computeTopLeftGridPoint(const QPointF& pointP);

protected:
    /** @brief sur changement de l'item */
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant & value );
    /** @brief sur release sourie */
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    /** @brief sur double click */
    virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    /** @brief arrange des connecteurs */
    void arrangeConnectors();
private:
    /** @brief updateLinks */
    void updateLinks();
private:
    /** @brief Composant */
    StreamWork::SwCore::SwComponent_Class * _component;
    /** @brief interface d'executable */
    StreamWork::SwExecution::ISwExecutable_Service * _executable;
    /** @brief interface d'executeur */
    StreamWork::SwExecution::ISwExecution_Service * _executor;
    /** @brief StreamControler */
    StreamControler * _controler;
    /** @brief Bounding rect */
    mutable QRectF _bbox;
    /** @brief id icon */
    QPixmap _icone;
    /** @brief liste des connecteurs */
    QList<ConnectorGraphicItem *> _connectors;
    /** @brief QPainterPath header */
    QPainterPath _header_path;
    /** @brief QPainterPath body */
    QPainterPath _body_path;
    /** @brief Brush header */
    QBrush _headerBrush;
    /** @brief Brush body */
    QBrush _bodyBrush;
    /** @brief Pen  */
    QPen _pen;
    /** @brief Pen en selection  */
    QPen _selected_pen;
    /** @brief Pen pour le texte  */
    QPen _text_pen;
    /** @brief Hauteur de l'entete */
    int _header_height;
    /** @brief Compteur de connecteurs gauche */
    int _leftCount;
    /** @brief Compteur de connecteurs droit */
    int _rightCount;
    /** @brief connector insertion position display*/
    bool _connectionInsertionPositionDisplay;
    /** @brief connector insertion position */
    QPointF _connectionInsertionPosition;
    /** @brief connector insertion index */
    int _connectionInsertionIndex;
    /** @brief Test */
    QString _dbg_cname;
	/** @brief Gradient */
	QLinearGradient _grad;
};

#endif
