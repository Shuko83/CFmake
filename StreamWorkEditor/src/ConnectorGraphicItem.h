/**
@file ConnectorGraphicItem.h
@brief Item graphique correspondant a un connecteur
@author F.Bighelli
 */

#ifndef _CONNECTORGRAPHICITEM_H
#define _CONNECTORGRAPHICITEM_H

#include <QtCore>
#include <QtWidgets>

#define CL_CONNECTOR_BBSIZE 20.0
#define CL_CONNECTOR_SIZE 12.0

typedef enum {PROVIDER,CONSUMER,PIN} TConnectorType;
typedef enum {RIGHT,LEFT,UNDEFINED} TConnectorPosition;

class LinkGraphicItem;
/**
@class ConnectorGraphicItem
@brief Item graphique correspondant a un connecteur
*/
class ConnectorGraphicItem : public QGraphicsItem {

public:
	/** @brief Constructor */
	ConnectorGraphicItem(QGraphicsItem * parent,QString name,QString type,TConnectorType ctype);
    /** @brief Definition du rectangle englobant */
    QRectF boundingRect() const;
    /** @brief Fonction de dessin */
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    /** @brief acces au nom */
    QString getName();
    /** @brief acces au type modele */
    QString getModelType();
    /** @brief acces au type de connecteur*/
    TConnectorType getConnectorType();
    /** @brief definition de la position dans le parent */
    void setParentPosition(TConnectorPosition position);
    /** @brief acces a la position dans le parent */
    TConnectorPosition getParentPosition();
    /** @brief inversion la mise en evidence externe */
    void toggleHighlightExtern();
    /** @brief inversion la mise en evidence du hover */
    void toggleHover();
    /** @brief Ajouter un lien */
    void addLink(LinkGraphicItem * link);
    /** @brief Ajouter un lien */
    void removeLink(LinkGraphicItem * link);
    /** @brief getLinks */
    QList<LinkGraphicItem *> * getLinks();
    /** @brief mise a jour des liens */
    void updateLinks();
protected:
    /** @brief sur changement de l'item */
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant & value );
    /** @brief sur enter hover */
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    /** @brief sur leave hover */
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    /** @brief Nom de l'interface */
    QString _name;
    /** @brief Type de l'interface */
    QString _type;
    /** @brief Type de connexion de l'interface */
    TConnectorType _ctype;
    /** @brief Position */
    TConnectorPosition _cpos;
    /** @brief mise en evidence externe */
    bool _highlightExtern;
    /** @brief mise en evidence hover */
    bool _showHover;
    /** @brief liste des liens */
    QList<LinkGraphicItem *> _links;
};

#endif
