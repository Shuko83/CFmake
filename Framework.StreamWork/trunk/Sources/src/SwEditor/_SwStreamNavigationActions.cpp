/*!
 \file _SwStreamsNavigationActions.cpp
 \brief Implementations des QActions relatives a un manager de stream
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QKeySequence>
#include "_SwStreamNavigationActions.h"

using namespace StreamWork::SwEditor;


_SwStreamNavigationActions::_SwStreamNavigationActions(QObject *parent,SwInterfaces_Provider_Class * provider_service)
	: QObject(parent)
{
    _navigator=NULL;
    _provider_service=provider_service;

    _go_to_parent=new QAction(QIcon(":/SwEditor/up.png"),"Go &Up",this);
    _go_to_parent->setShortcut(QKeySequence("Ctrl+U"));
    _go_to_parent->setStatusTip("Go to parent component");
    connect(_go_to_parent, SIGNAL(triggered()), this, SLOT(OnGoToParent()));
    _go_to_parent_c=new _ActionContainer(_go_to_parent);
    _provider_service->RegisterProvidedInterface<ISwAction>("ActionGoToParent",(ISwAction *)_go_to_parent_c);

	_current_path=new QLineEdit(NULL);
    _current_path->setStatusTip("Current path in current stream");
    _current_path->setToolTip("Current path in current stream");
	_current_path->setReadOnly(true);
    _current_path_c=new _WidgetContainer(_current_path);
    _provider_service->RegisterProvidedInterface<ISwWidget>("CurrentPath",(ISwWidget *)_current_path_c);
	
    _current_path->setEnabled(false);
    _go_to_parent->setEnabled(false);
}

_SwStreamNavigationActions::~_SwStreamNavigationActions()
{
    _provider_service->UnregisterProvidedInterface("CurrentPath");
    _provider_service->UnregisterProvidedInterface("ActionGoToParent");
    delete _current_path_c;
    delete _current_path;
    delete _go_to_parent_c;
    delete _go_to_parent;
}
/*! \brief Attach un stream manager */
void _SwStreamNavigationActions::AttachStreamNavigator(ISwEditorStreamNavigation * navigator) {
    _navigator=navigator;
    _navigator->AttachNavigationObserver(this);
    Update();
}
/*! \briefdetach un stream manager */
void _SwStreamNavigationActions::DetachStreamNavigator() {
    _navigator->DetachNavigationObserver(this);   
	_navigator=NULL;
    Update();
}
/*! \brief callback OnGoToParent*/
void _SwStreamNavigationActions::OnGoToParent(){
    if (_navigator!=NULL) _navigator->GoToParent();
}
//---------------------------------------------------------------------
// internal class _ActionContainer
//---------------------------------------------------------------------
/*! \brief Construction*/
_SwStreamNavigationActions::_ActionContainer::_ActionContainer(QAction * action) {
    _action=action;
}/*! \brief Construction*/
QAction & _SwStreamNavigationActions::_ActionContainer::GetAction() {
    return *_action;
}
//---------------------------------------------------------------------
// internal class _WidgetContainer
//---------------------------------------------------------------------
/*! \brief Construction*/
_SwStreamNavigationActions::_WidgetContainer::_WidgetContainer(QWidget * widget) {
    _widget=widget;
}/*! \brief Construction*/
QWidget & _SwStreamNavigationActions::_WidgetContainer::GetWidget() {
    return *_widget;
}
//---------------------------------------------------------------------
// Interface ISwObserver
//---------------------------------------------------------------------
/*! \brief methode appelée par l'observable*/
void _SwStreamNavigationActions::Update() {
    if (_navigator==NULL) {
		_go_to_parent->setEnabled(false);
		_current_path->setEnabled(false);
		return;
	}
	if (_navigator->GetCurrentComponent()!=NULL) {
		_current_path->setEnabled(true);
        _current_path->setText(_navigator->GetCurrentPath());
	} else {
		_current_path->setEnabled(false);	
	}
	if (_navigator->GetCurrentComponent()!=NULL && _navigator->GetCurrentComponent()->GetParent()!=NULL) _go_to_parent->setEnabled(true);
	else _go_to_parent->setEnabled(false);
}
