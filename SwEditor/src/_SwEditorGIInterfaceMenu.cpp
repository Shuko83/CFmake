/*!
 \file _SwEditorGIInterfaceMenu.cpp
 \brief menu pour un graphics item interface composant
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/


#include "_SwEditorGIInterfaceMenu.h"
#include "Component/Services/ISwInterfaces_Consumer.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

/*! \brief constructeur*/
_SwEditorGIInterfaceMenu::_SwEditorGIInterfaceMenu(_SwEditorInterfaceGraphicItem *interface_item,SwComponent_Class * host_comp,QWidget *parent):QMenu(parent) {
    QAction *action;
    _interface_item=interface_item;
    _host_comp=host_comp;

    setWindowOpacity(0.9);
    setStyleSheet("* { border: 1px solid gray; background: #ccc; font-size:10px}");


    action=new QAction("Disconnect",this);
    connect(action, SIGNAL(triggered()), this, SLOT(Disconnect()));
    addAction(action);

}
/*! \brief destructeur*/
_SwEditorGIInterfaceMenu::~_SwEditorGIInterfaceMenu() {
}
/*! \brief Deconnection interface*/
void _SwEditorGIInterfaceMenu::Disconnect() {
    QString name=_interface_item->GetInterfaceName();
    ISwInterfaces_Consumer * consumer=dynamic_cast<ISwInterfaces_Consumer *>(_host_comp->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));

    consumer->DetachProvider(name);
    _interface_item->ResetEndPosition();
}
