/**
@file Arranger.cpp
@brief arrangeur de stream
@author F.Bighelli
 */

#include "Arranger.h"
#include "LinkGraphicItem.h"

 bool LessThan(const Arranger::Container  *c1, const Arranger::Container * c2)
 {
     return c1->_ponderation.y()<c2->_ponderation.y();
 }


/** @brief Constructor */
Arranger::Arranger() {
	//A Completer
}
/** @brief Arrange */
void Arranger::arrange(QList<ComponentGraphicItem *> *lcitems) {

}
Arranger::Container::Container(ConnectorGraphicItem * citem,QPointF ponderation) {
    _item=citem;
    _ponderation=ponderation;
}
/** @brief Arrange connectors */
void Arranger::arrangeConnectors(QList<ComponentGraphicItem *> *lcitems) {
    QList<ComponentGraphicItem *>::iterator it=lcitems->begin();
    while(it!=lcitems->end()) {
        arrangeConnectorsForAComponent(*it);
        it++;
    }
}
/** @brief Arrange connectors for a components */
void Arranger::arrangeConnectorsForAComponent(ComponentGraphicItem * citem) {
    QList<ConnectorGraphicItem *> *connectors=citem->getConnectors();
    QList<Container *> containers;
    if (connectors->isEmpty())
        return;
    QList<ConnectorGraphicItem *>::iterator it=connectors->begin();
    while (it!=connectors->end()) {
        containers.push_back(new Container(*it,processPonderationPoint(*it)));
        it++;
    }
    qSort(containers.begin(),containers.end(),LessThan);
    connectors->clear();
    QList<Container *>::iterator itc=containers.begin();
    while(itc!=containers.end()) {
        connectors->push_back((*itc)->_item);
        if ((*itc)->_ponderation.x()<0.0)
            (*itc)->_item->setParentPosition(LEFT);
        else 
            (*itc)->_item->setParentPosition(RIGHT);
        delete *itc;
        itc++;
    }
    citem->updateAttributs();
}
/** @brief calcul du point de ponderation */
QPointF Arranger::processPonderationPoint(ConnectorGraphicItem * con) {
    QList<LinkGraphicItem *> * lks=con->getLinks();
    QList<LinkGraphicItem *>::iterator it=lks->begin();
    QPointF average;
    QGraphicsItem * parent=con->parentItem();
    ConnectorGraphicItem * rcon;
    while (it!=lks->end()) {
        rcon=(*it)->getSource();
        if (rcon==con) 
            rcon=(*it)->getTarget();
        average+=rcon->mapToItem(parent,rcon->pos());
        it++;
    }
    if (lks->isEmpty())
        return average;
    average/=lks->count();
    return average;
}
