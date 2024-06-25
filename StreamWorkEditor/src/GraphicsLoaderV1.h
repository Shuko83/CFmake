/**
@file GraphicsLoaderV1.h
@brief Recuperateur des informations graphiques de l'editeur V1
@author F.Bighelli
 */

#ifndef _GRAPHICSLOADERV1_H
#define _GRAPHICSLOADERV1_H

#include <QtCore>
#include <QtXml>
#include "ComponentGraphicItem.h"

/**
@class GraphicsLoaderV1
@brief Recuperateur des informations graphiques de l'editeur V1
*/
class GraphicsLoaderV1 {
private:
    /** @brief Constructeur */
    GraphicsLoaderV1();
    /** @brief Constructeur */
    void loadGraphicData(ComponentGraphicItem * gitem,QDomElement parentNode);
public:
    /** @brief chargement des informations graphiques issues de l'editeur V1 */
    static void loadGraphicsInfoFromDom(QDomDocument & doc,QList<ComponentGraphicItem *> gcomponents,QGraphicsScene * scene);
};

#endif
