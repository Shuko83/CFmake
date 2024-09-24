/*!
 \file _SwEditorInterfacesLayerManager.cpp
 \brief Gestionnaire des interfaces dessinés
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QtGlobal>
#include <QAction>
#include <QRegExp>

#include "_SwEditorInterfacesLayerManager.h"
#include "_SwEditorInterfaceGraphicItem.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

/*! \brief constructeur*/
_SwEditorInterfacesLayerManager::_SwEditorInterfacesLayerManager( QObject * parent):QObject(parent) {

    /*! \brief menu_principal*/
    _interfaces_display_main_menu=new QMenu("Interfaces",NULL);
    _interfaces_display_main_menu->setWindowOpacity(0.9);
    _interfaces_display_main_menu->setStyleSheet("* { border: 1px solid gray; background: #ccc; font-size:10px }");

    show_all_action=new QAction("Show all interfaces",_interfaces_display_main_menu);
    //connect(action, SIGNAL(triggered()), this, SLOT(GoToParent()));
    _interfaces_display_main_menu->addAction(show_all_action);
    hide_all_action=new QAction("Hide all interfaces",_interfaces_display_main_menu);
    //connect(action, SIGNAL(triggered()), this, SLOT(GoToParent()));
    _interfaces_display_main_menu->addAction(hide_all_action);

    _interfaces_display_main_menu->addSeparator();
    connect(_interfaces_display_main_menu, SIGNAL(triggered(QAction *)), this, SLOT(ActionTriggered(QAction *)));

}
/*! \brief destructeur*/
_SwEditorInterfacesLayerManager::~_SwEditorInterfacesLayerManager() {
    delete _interfaces_display_main_menu;
}
/*! \brief Initialisation */
void _SwEditorInterfacesLayerManager::AddCompToSpy(SwComponent_Class * host_comp){
    SwCore::ISwInterfaces_Provider * provider;
    SwCore::ISwInterfaces_Consumer * consumer;
    QString si;
	provider=dynamic_cast<ISwInterfaces_Provider *>(host_comp->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
	consumer=dynamic_cast<ISwInterfaces_Consumer *>(host_comp->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));

    if (provider!=NULL) {
        si=provider->GetFirstInterface();
        while (!si.isEmpty()) {
            OnAddInterface(provider,si);
            si=provider->GetNextInterface();
        }
        provider->AttachInterfacesServices_Listener(this);
    }
    if (consumer!=NULL) {
        si=consumer->GetFirstInterface(NULL,NULL,NULL);
        while (!si.isEmpty()) {
            OnAddInterface(consumer,si);
            si=consumer->GetNextInterface(NULL,NULL,NULL);
        }
        consumer->AttachInterfacesServices_Listener(this);
    }
}
/*! \brief Acces a l'etat d'un type d'interface */
bool _SwEditorInterfacesLayerManager::GetInterfaceDisplayState(const QString & interface_type) {
    QMap<QString,QAction *>::iterator it;

    it=_interfaces_display.find(interface_type);
    if (it!=_interfaces_display.end()) {
        return it.value()->isChecked();
    }
    return true;
}
/*! \brief Acces au menu pour la gestion des interfaces */
QMenu * _SwEditorInterfacesLayerManager::GetInterfaceDisplayStateMenu() {
    return _interfaces_display_main_menu;
}
/*! \brief Definition de les interfaces produites a mettre en evidence */
void _SwEditorInterfacesLayerManager::SetInterfaceToHighlight(const QString & interface_type, bool set_highlight) {
    OnInterfaceHighlightChange(&interface_type,set_highlight);
    if (!set_highlight) _highlighted_interfaces.clear();
}
/*! \brief Enregistrer les interfaces mises en evidence */
void _SwEditorInterfacesLayerManager::RegisterHighlightedInterfaceItem(_SwEditorInterfaceGraphicItem * h_interface) {
    _highlighted_interfaces.push_back(h_interface);
}
/*! \brief Acces a la liste des les interfaces mises en evidence */
QList<_SwEditorInterfaceGraphicItem *> * _SwEditorInterfacesLayerManager::GetHighlightedInterfaceItems() {
    return &_highlighted_interfaces;
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ServicesListener
//---------------------------------------------------------------------
/*! \brief Sur ajout d'une nouvelle interface */
void _SwEditorInterfacesLayerManager::OnAddInterface(ISwInterfaces_Service * source,QString interface_name){
    SwCore::ISwInterfaces_Provider * provider;
    SwCore::ISwInterfaces_Consumer * consumer;
    QString type_interface;
    QString simple_type_interface;
    provider=dynamic_cast<ISwInterfaces_Provider *>(source);
	consumer=dynamic_cast<ISwInterfaces_Consumer *>(source);

    if (provider!=NULL) {
        type_interface=provider->GetInterfaceType(interface_name);
    }
    if (consumer!=NULL) {
        type_interface=consumer->GetInterfaceType(interface_name);
    }
    if (!type_interface.isEmpty()) {
        QMap<QString,QAction *>::iterator it;

//Nom simple depend de l'os
#ifdef Q_WS_WIN
        QRegExp r("class ([A-Za-z_0-9]*::)*([A-Za-z_0-9]*) *\\*");
        if (r.exactMatch(type_interface)) {
            QStringList l=r.capturedTexts();
            simple_type_interface=l[l.count()-1];
        } else
            simple_type_interface=type_interface;
#else
        simple_type_interface=type_interface;
#endif
        it=_interfaces_display.find(type_interface);
        if (it==_interfaces_display.end()) {
            QAction * action=_interfaces_display_main_menu->addAction(simple_type_interface);
            action->setToolTip(type_interface);
            action->setCheckable(true);
            action->setChecked(true);
            _interfaces_display.insert(type_interface,action);
        }
    }
}
/*! \brief Sur suppression d'une  interface */
void _SwEditorInterfacesLayerManager::OnRemoveInterface(ISwInterfaces_Service * source,QString interface_name){

}
/*! \brief Sur connection d'une interface */
void _SwEditorInterfacesLayerManager::OnConnectInterface(ISwInterfaces_Service * source,QString interface_name,ISwInterfaces_Service * remote_source,QString remote_interface_name){

}
/*! \brief Sur deconnection d'une interface */
void _SwEditorInterfacesLayerManager::OnDisconnectInterface(ISwInterfaces_Service * source,QString interface_name,ISwInterfaces_Service * remote_source,QString remote_interface_name) {

}
/*! \brief Sur action triggered */
void _SwEditorInterfacesLayerManager::ActionTriggered(QAction * action) {
    QMap<QString,QAction *>::iterator it;

    if (action==show_all_action) {
        for(it=_interfaces_display.begin();it!=_interfaces_display.end();it++) {
            it.value()->setChecked(true);
            OnInterfaceVisibilityChange(&(it.key()),it.value()->isChecked());
        }
        return;
    }
    if (action==hide_all_action) {
        for(it=_interfaces_display.begin();it!=_interfaces_display.end();it++) {
            it.value()->setChecked(false);
            OnInterfaceVisibilityChange(&(it.key()),it.value()->isChecked());
        }
        return;
    }
    it=_interfaces_display.find(action->toolTip());
    if (it!=_interfaces_display.end()) {
        OnInterfaceVisibilityChange(&(it.key()),it.value()->isChecked());
    }
}
