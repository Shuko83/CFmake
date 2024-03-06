/**
@file GraphicsLoaderV1.cpp
@brief Recuperateur des informations graphiques de l'editeur V1
@author F.Bighelli
 */

#include "GraphicsLoaderV1.h"
#include "SwCoreConstantes.h"

/** @brief Constructeur */
GraphicsLoaderV1::GraphicsLoaderV1() {

}

/** @brief chargement des informations graphiques issues de l'editeur V1 */
void GraphicsLoaderV1::loadGraphicsInfoFromDom(QDomDocument & doc,QList<ComponentGraphicItem *> gcomponents,QGraphicsScene * scene) {
    GraphicsLoaderV1 loader;
    QDomElement rootCompNode=doc.documentElement().firstChildElement(QString(CG_SW_XML_COMPONENT_NODE));

    //Recherche du service EditorGraphicScene
    QDomElement cEltService=rootCompNode.firstChildElement(QString(CG_SW_XML_SERVICE_NODE));
    while (!cEltService.isNull() && cEltService.attribute(CG_SW_XML_SERVICE_ATT_NAME)!=QString("EditorGraphicScene")) {
        cEltService=cEltService.nextSiblingElement(CG_SW_XML_SERVICE_NODE);
    }
    //Si trouve
    if (!cEltService.isNull()) {
        //Si node gitem trouve
        QDomElement cEltGItem=cEltService.firstChildElement(QString("gscene"));
        if (!cEltGItem.isNull()) {
            //Recuperation des positions
            double x=2.0*cEltGItem.attribute("x").toDouble();
            double y=2.0*cEltGItem.attribute("y").toDouble();
            double width=2.0*cEltGItem.attribute("width").toDouble();
            double height=2.0*cEltGItem.attribute("height").toDouble();
            scene->setSceneRect(x,y,width,height);
        }
    }

    //Traitement des enfants
    QList<ComponentGraphicItem *>::iterator it=gcomponents.begin();
    while (it!=gcomponents.end()) {
        loader.loadGraphicData(*it,rootCompNode);
        it++;
    }
}

/** @brief Constructeur */
void GraphicsLoaderV1::loadGraphicData(ComponentGraphicItem * gitem,QDomElement parentNode) {
    //Recherche du dom node associé
    QDomElement cElt=parentNode.firstChildElement(QString(CG_SW_XML_COMPONENT_NODE));
    while (!cElt.isNull() && cElt.attribute(CG_SW_XML_COMPONENT_NODE_ATT_NAME)!=gitem->getComponent()->GetName()) {
        cElt=cElt.nextSiblingElement(CG_SW_XML_COMPONENT_NODE);
    }
    //Si le neoud n'est pas trouvé
    if (cElt.isNull()) {
        return; //Fin
    }
    //Recherche du service EditorGraphicItem
    QDomElement cEltService=cElt.firstChildElement(QString(CG_SW_XML_SERVICE_NODE));
    while (!cEltService.isNull() && cEltService.attribute(CG_SW_XML_SERVICE_ATT_NAME)!=QString("EditorGraphicItem")) {
        cEltService=cEltService.nextSiblingElement(CG_SW_XML_SERVICE_NODE);
    }
    //Si trouve
    if (!cEltService.isNull()) {
        //Si node gitem trouve
        QDomElement cEltGItem=cEltService.firstChildElement(QString("gitem"));
        if (!cEltGItem.isNull()) {
            //Recuperation des positions
            double px=2.0*floor(cEltGItem.attribute("px").toDouble());
            double py=2.0*floor(cEltGItem.attribute("py").toDouble());
            gitem->setPos(px,py);
        }
    }
    //Recursif sur les enfants
    QList<ComponentGraphicItem *> cgitems=gitem->getChildsComponents();
    QList<ComponentGraphicItem *>::iterator it=cgitems.begin(); 
    while(it!=cgitems.end()) {
        loadGraphicData(*it,cElt);
        it++;
    }
}
