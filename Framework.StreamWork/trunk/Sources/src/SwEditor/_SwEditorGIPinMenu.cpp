/*!
 \file _SwEditorGIPinMenu.cpp
 \brief menu pour un graphics item pin composant
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/


#include "_SwEditorGIPinMenu.h"
#include <ISwPins_Manager.h>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

/*! \brief constructeur*/
_SwEditorGIPinMenu::_SwEditorGIPinMenu(_SwEditorPinGraphicItem *pin_item,SwComponent_Class * host_comp,QWidget *parent):QMenu(parent) {
    QAction *action;
    _pin_item=pin_item;
    _host_comp=host_comp;

    setWindowOpacity(0.9);
    setStyleSheet("* { border: 1px solid gray; background: #ccc; font-size:10px}");


    action=new QAction("Disconnect",this);
    connect(action, SIGNAL(triggered()), this, SLOT(Disconnect()));
    addAction(action);

}
/*! \brief destructeur*/
_SwEditorGIPinMenu::~_SwEditorGIPinMenu() {
}
/*! \brief Deconnection pin*/
void _SwEditorGIPinMenu::Disconnect() {
    QString name=_pin_item->GetPinName();
    ISwPins_Manager * manager=_pin_item->GetPinManager();


    manager->DisconnectPin(name);
    _pin_item->ResetEndPosition();
}
