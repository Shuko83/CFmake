/**
@file Arranger.h
@brief arrangeur de stream
@author F.Bighelli
 */

#ifndef _ARRANGER_H
#define _ARRANGER_H

#include <QList>
#include "ComponentGraphicItem.h"
/**
@class Arranger
@brief arrangeur de stream
*/
class Arranger {
public:
    class Container {
    public:
        Container(ConnectorGraphicItem * item,QPointF ponderation);
        ConnectorGraphicItem * _item;
        QPointF _ponderation;
    };
	/** @brief Constructor */
	Arranger();
    /** @brief Arrange */
    void arrange(QList<ComponentGraphicItem *> *lcitems);
    /** @brief Arrange connectors */
    void arrangeConnectors(QList<ComponentGraphicItem *> *lcitems);
private:
    /** @brief Arrange connectors for a components */
    void arrangeConnectorsForAComponent(ComponentGraphicItem * citem);
    /** @brief calcul du point de ponderation */
    QPointF processPonderationPoint(ConnectorGraphicItem * con);
};

#endif
