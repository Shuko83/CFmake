/**
@file GraphicsResources.h
@brief Resources Graphique
@author F.Bighelli
 */

#ifndef _GRAPHICSRESOURCES_H
#define _GRAPHICSRESOURCES_H

#include <QtGui>
#include <SwComponent_Class.h>

class ConnectorGraphItem;
/**
@class GraphicsResources
@brief Resources Graphique
*/
class GraphicsResources {

public:
    /** @brief acces singleton */
    static GraphicsResources * getInstance();

    /** @brief renvoie l'icone d'un composant */
    QPixmap getComponentIcon(StreamWork::SwCore::SwComponent_Class * component);
    /** @brief renvoie l'icone de run */
    QPixmap getRunIcon();
    /** @brief renvoie l'icone de replay */
    QPixmap getReplayIcon();
    /** @brief renvoie l'icone de run disabled */
    QPixmap getRunIconDisabled();
    /** @brief couleur interface par type */
    QColor getInterfaceColor(QString type);
    
private:
	/** @brief Constructor */
	GraphicsResources();
	/** @brief ~Destructor */
	~GraphicsResources();

private:
    /** @brief id icon_run */
    QPixmap _icone_run;
    /** @brief id icon_run_disable */
    QPixmap _icone_run_disable;
    /** @brief id icon_replay */
    QPixmap _icone_replay;
    /** @brief id icon_run_disable */
    QGraphicsView * _view;


};

#endif
