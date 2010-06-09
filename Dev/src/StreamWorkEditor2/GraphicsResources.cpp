/**
@file GraphicsResources.cpp
@brief Resources Graphique
@author F.Bighelli
 */

#include "GraphicsResources.h"

#include <SwApplication.h>
#include <SwMacros.h>

using namespace StreamWork::SwCore;


static GraphicsResources * _instance=0;

/** @brief Constructor */
GraphicsResources::GraphicsResources() {
	_icone_run=QIcon(":/StreamWorkEditor2/mod_run.png").pixmap(11,14);
    _icone_run_disable=QIcon(":/StreamWorkEditor2/mod_run_disable.png").pixmap(11,14);
    _icone_replay=QIcon(":/StreamWorkEditor2/mod_replay.png").pixmap(11,14);
    _view=0;
}
/** @brief ~Destructor */
GraphicsResources::~GraphicsResources() {

}
/** @brief acces singleton */
GraphicsResources * GraphicsResources::getInstance() {
    if (_instance==0) {
        _instance=new GraphicsResources();
    }
    return _instance;
}
/** @brief renvoie l'icone d'un composant */
QPixmap GraphicsResources::getComponentIcon(StreamWork::SwCore::SwComponent_Class * component) {
    QIcon ico=SW_APP->ComponentsBank().GetComponentIcon(component->GetFactoryComponentName());
    return ico.pixmap(16,16);
}

/** @brief renvoie l'icone de run */
QPixmap GraphicsResources::getRunIcon() {
    return _icone_run;
}
/** @brief renvoie l'icone de run disabled */
QPixmap GraphicsResources::getRunIconDisabled() {
    return _icone_run_disable;
}
/** @brief renvoie l'icone de replay */
QPixmap GraphicsResources::getReplayIcon() {
    return _icone_replay;
}

/** @brief couleur interface par type */
QColor GraphicsResources::getInterfaceColor(QString type) {
    return QColor("#330066");
}
