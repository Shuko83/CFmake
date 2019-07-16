/*!
\file _SwEditorNavigator_Class.cpp
\date 12/04/2006
\brief implementation de la selection
\author  Big
\version 1.0
 */
#include <SwApplication.h>
#include <SwMacros.h>
#include <QMessageBox>
#include <SwAddress_ToolBox.h>
#include "_SwEditorNavigator_Class.h"
#include "_SwEditorSelection_Class.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

/*! \brief Construsteur */
_SwEditorNavigator_Class::_SwEditorNavigator_Class(SwComponent_Class * host) {
    _host_component=host;
    _observers.clear();
    //Definition de la selection par defaut - racine
    host->RegisterService(new _SwEditorSelection_Class(_host_component));
    _current_selection=host;
}
/*! \brief Destrusteur */
_SwEditorNavigator_Class::~_SwEditorNavigator_Class(){
    _observers.clear();
}
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
	lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwEditorNavigator_Class::Liberate() {

}
//-------------------------------------------------------------
// Navigation dans l'arbre du stream
//-------------------------------------------------------------
/*! \brief Remonter au parent*/
void _SwEditorNavigator_Class::GoToParent(){
    if (_current_selection->GetParent()!=NULL) {
        _current_selection=_current_selection->GetParent();
        //Check si le parent a un service selection
        if (_current_selection->QueryService(CG_SW_SERVICE_SELECTION)==NULL) {
            //non, on en crée un
            _current_selection->RegisterService(new _SwEditorSelection_Class(_current_selection));
        }
        //On signale le changement
        SignalChangeToObservers();
    }
}
/*! \brief Descendre sur un des enfants*/
void _SwEditorNavigator_Class::GoToChild(QString child_name){
    SwComponent_Class * child;

    //Recherche de l'enfant
    child=_current_selection->GetChild(child_name);
    if (child!=NULL) {
        //trouve, on le met en selection courante
        _current_selection=child;
        //Check si l'enfant a un service selection
        if (_current_selection->QueryService(CG_SW_SERVICE_SELECTION)==NULL) {
            //non, on en crée un
            _current_selection->RegisterService(new _SwEditorSelection_Class(_current_selection));
        }
        //On signale le changement
        SignalChangeToObservers();
    }
}
/*! \brief Acces au composant courant*/
SwComponent_Class * _SwEditorNavigator_Class::GetCurrentComponent() {
    return _current_selection;
}
/*! \brief Definition au composant courant*/
void _SwEditorNavigator_Class::SetCurrentComponent(SwComponent_Class * component) {
    SwComponent_Class * parent;
    //Test si la racine est parente
    parent=component;
    while(parent!=_host_component && parent!=NULL) parent=parent->GetParent();
    //Si pas de racine commune
    if (parent!=_host_component) {
        //Exception
        LAUNCH_SWEXCEPTION("SwEditor","Try to define a current component which isn't in the stream");
    }
    //Ok, il devient la selection courante
    _current_selection=component;
    //Check si un service selection existe
    if (_current_selection->QueryService(CG_SW_SERVICE_SELECTION)==NULL) {
        //non, on en crée un
        _current_selection->RegisterService(new _SwEditorSelection_Class(_current_selection));
    }
    //On signale le changement
    SignalChangeToObservers();
}
/*! \brief Acces au path courant*/
QString _SwEditorNavigator_Class::GetCurrentPath() {
    return SwAddress_ToolBox::BuildAbsolutePath(_current_selection);
}
//-------------------------------------------------------------
// Observations
//-------------------------------------------------------------
/*! \brief Attachement d'un observateur sur cette interface*/
void _SwEditorNavigator_Class::AttachNavigationObserver(ISwObserver * observer){

    if (_observers.find(observer)==_observers.end()) {
        _observers.insert(observer);
    }
}
/*! \brief Detachement d'un observateur sur cette interface*/
void _SwEditorNavigator_Class::DetachNavigationObserver(ISwObserver * observer){
    QSet<ISwObserver *>::iterator it;

    it=_observers.find(observer);
    if (it!=_observers.end()) {
        _observers.erase(it);
    }
}
/*! \brief Signale un changement aux observers*/
void _SwEditorNavigator_Class::SignalChangeToObservers() {
    for (QSet<ISwObserver *>::iterator it=_observers.begin();it!=_observers.end();it++)
        (*it)->Update();
}
