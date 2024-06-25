/*!
 \file _SwEditorPinsLayerManager.cpp
 \brief  Gestionnaire des pins dessinés
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QtGlobal>
#include <QAction>
#include <QRegularExpression>

#include "_SwEditorPinsLayerManager.h"
#include "_SwEditorPinGraphicItem.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

/*! \brief constructeur*/
_SwEditorPinsLayerManager::_SwEditorPinsLayerManager( QObject * parent):QObject(parent) {

    /*! \brief menu_principal*/
    _pins_display_main_menu=new QMenu("Pins",NULL);
    _pins_display_main_menu->setWindowOpacity(0.9);
    _pins_display_main_menu->setStyleSheet("* { border: 1px solid gray; background: #ccc; font-size:10px }");

    show_all_action=new QAction("Show all pins",_pins_display_main_menu);
    //connect(action, SIGNAL(triggered()), this, SLOT(GoToParent()));
    _pins_display_main_menu->addAction(show_all_action);
    hide_all_action=new QAction("Hide all pins",_pins_display_main_menu);
    //connect(action, SIGNAL(triggered()), this, SLOT(GoToParent()));
    _pins_display_main_menu->addAction(hide_all_action);

    _pins_display_main_menu->addSeparator();
    connect(_pins_display_main_menu, SIGNAL(triggered(QAction *)), this, SLOT(ActionTriggered(QAction *)));

}
/*! \brief destructeur*/
_SwEditorPinsLayerManager::~_SwEditorPinsLayerManager() {
    delete _pins_display_main_menu;
}
/*! \brief Initialisation */
void _SwEditorPinsLayerManager::AddCompToSpy(SwComponent_Class * host_comp){
    SwCore::ISwPins_Manager * pin_manager;
    QString si;

    pin_manager=dynamic_cast<ISwPins_Manager *>(host_comp->QueryService(CG_SW_SERVICE_PINS_MANAGER));

    if (pin_manager==NULL) {
        return;
    }
    pin_manager->RegisterListener(this);

    QList<SwPin *> liste_pin=pin_manager->GetPinList();

    for (int i=0;i<liste_pin.count();i++) {
        OnAddPin(liste_pin[i]);
    }
}
/*! \brief Acces a l'etat d'un type de pin */
bool _SwEditorPinsLayerManager::GetPinDisplayState(const QString & pin_type) {
    QMap<QString,QAction *>::iterator it;

    it=_pins_display.find(pin_type);
    if (it!=_pins_display.end()) {
        return it.value()->isChecked();
    }
    return true;
}
/*! \brief Acces au menu pour la gestion des pins */
QMenu * _SwEditorPinsLayerManager::GetPinDisplayStateMenu() {
    return _pins_display_main_menu;
}
/*! \brief Definition des pins a mettre en evidence */
void _SwEditorPinsLayerManager::SetPinToHighlight(const QString & pin_type, bool set_highlight) {
    if (set_highlight) _highlighted_pins.clear();
    OnPinHighlightChange(&pin_type,set_highlight);
    if (!set_highlight) _highlighted_pins.clear();
}
/*! \brief Enregistrer les interfaces mises en evidence */
void _SwEditorPinsLayerManager::RegisterHighlightedPinsItem(_SwEditorPinGraphicItem * h_pin) {
    _highlighted_pins.push_back(h_pin);
}
/*! \brief Acces a la liste des pins mis en evidence */
QList<_SwEditorPinGraphicItem *> * _SwEditorPinsLayerManager::GetHighlightedPinsItems() {
    return &_highlighted_pins;
}
//---------------------------------------------------------------------
// Interface ISwPins_ManagerListener
//---------------------------------------------------------------------
/*! \brief Sur ajout d'un nouveau pin*/
void _SwEditorPinsLayerManager::OnAddPin(SwPin * pin){
    QString type_pin;

    type_pin=pin->GetType();
    if (!type_pin.isEmpty()) {
        QMap<QString,QAction *>::iterator it;

        it=_pins_display.find(type_pin);
        if (it==_pins_display.end()) {
            QAction * action=_pins_display_main_menu->addAction(type_pin);
            action->setToolTip(type_pin);
            action->setCheckable(true);
            action->setChecked(true);
            _pins_display.insert(type_pin,action);
        }
    }
}
/*! \brief Sur suppression d'un pin existant*/
void _SwEditorPinsLayerManager::OnRemovePin(SwPin * pin) {

}
/*! \brief Sur connexion d'un pin*/
void _SwEditorPinsLayerManager::OnConnectPin(SwPin * local_pin,SwPin * remote_pin){

}
/*! \brief Sur deconnexion d'un pin*/
void _SwEditorPinsLayerManager::OnDisconnectPin(SwPin * local_pin,SwPin * remote_pin){

}
/*! \brief Sur action triggered */
void _SwEditorPinsLayerManager::ActionTriggered(QAction * action) {
    QMap<QString,QAction *>::iterator it;

    if (action==show_all_action) {
        for(it=_pins_display.begin();it!=_pins_display.end();it++) {
            it.value()->setChecked(true);
            OnPinVisibilityChange(&(it.key()),it.value()->isChecked());
        }
        return;
    }
    if (action==hide_all_action) {
        for(it=_pins_display.begin();it!=_pins_display.end();it++) {
            it.value()->setChecked(false);
            OnPinVisibilityChange(&(it.key()),it.value()->isChecked());
        }
        return;
    }
    it=_pins_display.find(action->toolTip());
    if (it!=_pins_display.end()) {
        OnPinVisibilityChange(&(it.key()),it.value()->isChecked());
    }
}
