/*!
 \file _SwStreamOperationsActions.cpp
 \brief Implementations des QActions relatives a un manager de stream
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QKeySequence>
#include "_SwStreamOperationsActions.h"

using namespace StreamWork::SwEditor;


_SwStreamOperationsActions::_SwStreamOperationsActions(QObject *parent,SwInterfaces_Provider_Class * provider_service)
	: QObject(parent)
{
    _selection=NULL;
    _provider_service=provider_service;

    _select_all=new QAction("Select &All",this);
    _select_all->setShortcut(QKeySequence("Ctrl+A"));
    _select_all->setStatusTip("Select All");
    connect(_select_all, SIGNAL(triggered()), this, SLOT(OnSelectAll()));
    _select_all_c=new _ActionContainer(_select_all);
    _provider_service->RegisterProvidedInterface<ISwAction>("ActionSelectAll",(ISwAction *)_select_all_c);

    _select_none=new QAction("Select None",this);
    _select_none->setStatusTip("Select None");
    connect(_select_none, SIGNAL(triggered()), this, SLOT(OnSelectNone()));
    _select_none_c=new _ActionContainer(_select_none);
    _provider_service->RegisterProvidedInterface<ISwAction>("ActionSelectNone",(ISwAction *)_select_none_c);

    _select_invert=new QAction("Invert Selection",this);
    _select_invert->setShortcut(QKeySequence("Ctrl+Shift+I"));
    _select_invert->setStatusTip("Invert Selection");
    connect(_select_invert, SIGNAL(triggered()), this, SLOT(OnSelectInvert()));
    _select_invert_c=new _ActionContainer(_select_invert);
    _provider_service->RegisterProvidedInterface<ISwAction>("ActionSelectInvert",(ISwAction *)_select_invert_c);

    _remove_selection=new QAction(QIcon(":/SwEditor/editdelete.png"),"Remove",this);
    _remove_selection->setShortcut(QKeySequence(QKeySequence::Delete));
    _remove_selection->setStatusTip("Remove Selection");
    connect(_remove_selection, SIGNAL(triggered()), this, SLOT(OnRemoveSelection()));
    _remove_selection_c=new _ActionContainer(_remove_selection);
    _provider_service->RegisterProvidedInterface<ISwAction>("ActionRemoveSelection",(ISwAction *)_remove_selection_c);

    _copy_selection=new QAction(QIcon(":/SwEditor/editcopy.png"),"Copy",this);
    _copy_selection->setShortcut(QKeySequence(QKeySequence::Copy));
    _copy_selection->setStatusTip("Copy Selection");
    connect(_copy_selection, SIGNAL(triggered()), this, SLOT(OnCopySelection()));
    _copy_selection_c=new _ActionContainer(_copy_selection);
    _provider_service->RegisterProvidedInterface<ISwAction>("ActionCopySelection",(ISwAction *)_copy_selection_c);

    _paste_selection=new QAction(QIcon(":/SwEditor/editpaste.png"),"Paste",this);
    _paste_selection->setShortcut(QKeySequence(QKeySequence::Paste));
    _paste_selection->setStatusTip("Paste Selection");
    connect(_paste_selection, SIGNAL(triggered()), this, SLOT(OnPasteSelection()));
    _paste_selection_c=new _ActionContainer(_paste_selection);
    _provider_service->RegisterProvidedInterface<ISwAction>("ActionPasteSelection",(ISwAction *)_paste_selection_c);

    _cut_selection=new QAction(QIcon(":/SwEditor/editcut.png"),"Cut",this);
    _cut_selection->setShortcut(QKeySequence(QKeySequence::Cut));
    _cut_selection->setStatusTip("Cut Selection");
    connect(_cut_selection, SIGNAL(triggered()), this, SLOT(OnCutSelection()));
    _cut_selection_c=new _ActionContainer(_cut_selection);
    _provider_service->RegisterProvidedInterface<ISwAction>("ActionCutSelection",(ISwAction *)_cut_selection_c);


    _select_all->setEnabled(false);
    _select_none->setEnabled(false);
    _select_invert->setEnabled(false);
    _remove_selection->setEnabled(false);
    _copy_selection->setEnabled(false);
    _paste_selection->setEnabled(false);
    _cut_selection->setEnabled(false);
}

_SwStreamOperationsActions::~_SwStreamOperationsActions()
{
    _provider_service->UnregisterProvidedInterface("ActionSelectAll");
    _provider_service->UnregisterProvidedInterface("ActionSelectNone");
    _provider_service->UnregisterProvidedInterface("ActionSelectInvert");
    _provider_service->UnregisterProvidedInterface("ActionRemoveSelection");
    _provider_service->UnregisterProvidedInterface("ActionCopySelection");
    _provider_service->UnregisterProvidedInterface("ActionPasteSelection");
    _provider_service->UnregisterProvidedInterface("ActionCutSelection");
    delete _select_all_c;
    delete _select_all;
    delete _select_none_c;
    delete _select_none;
    delete _select_invert_c;
    delete _select_invert;
    delete _remove_selection_c;
    delete _remove_selection;
    delete _copy_selection_c;
    delete _copy_selection;
    delete _paste_selection_c;
    delete _paste_selection;
    delete _cut_selection_c;
    delete _cut_selection;
}
/*! \brief Attach un stream manager */
void _SwStreamOperationsActions::AttachStreamOperations(ISwEditorStreamOperations * selection){
    _selection=selection;
    _selection->AttachSelectionObserver(this);
    Update();
}
/*! \briefdetach un stream manager */
void _SwStreamOperationsActions::DetachStreamOperations() {
    _selection->DetachSelectionObserver(this);   
}
/*! \brief callback sur tout selectionner*/
void _SwStreamOperationsActions::OnSelectAll(){
    if (_selection!=NULL) _selection->SelectAll();
}
/*! \brief callback sur select none*/
void _SwStreamOperationsActions::OnSelectNone() {
    if (_selection!=NULL) _selection->SelectNone();
}
/*! \brief callback sur inversion de selection*/
void _SwStreamOperationsActions::OnSelectInvert(){
    if (_selection!=NULL) _selection->SelectInvert();
}
/*! \brief callback sur suppression de la selection*/
void _SwStreamOperationsActions::OnRemoveSelection(){
    if (_selection!=NULL) _selection->RemoveSelection();
}
/*! \brief callback sur copy de la selection*/
void _SwStreamOperationsActions::OnCopySelection(){
    if (_selection!=NULL) _selection->CopySelection();
}
/*! \brief callback sur coller la selection*/
void _SwStreamOperationsActions::OnPasteSelection(){
    if (_selection!=NULL) _selection->PasteSelection();
}
/*! \brief callback sur couper la selection*/
void _SwStreamOperationsActions::OnCutSelection(){
    if (_selection!=NULL) _selection->CutSelection();
}
/*! \brief Construction*/
_SwStreamOperationsActions::_ActionContainer::_ActionContainer(QAction * action) {
    _action=action;
}/*! \brief Construction*/
QAction & _SwStreamOperationsActions::_ActionContainer::GetAction() {
    return *_action;
}
//---------------------------------------------------------------------
// Interface ISwObserver
//---------------------------------------------------------------------
/*! \brief methode appelée par l'observable*/
void _SwStreamOperationsActions::Update() {
    if (_selection==NULL) {
        _select_all->setEnabled(false);
        _select_none->setEnabled(false);
        _select_invert->setEnabled(false);
        _remove_selection->setEnabled(false);
        _copy_selection->setEnabled(false);
        _paste_selection->setEnabled(false);
        _cut_selection->setEnabled(false);
        return;
    }
    if (_selection->GetUnselectedComponentNumber()==0) _select_all->setEnabled(false); 
    else _select_all->setEnabled(true); 
    if (_selection->GetSelectedComponentNumber()==0) _select_none->setEnabled(false); 
    else _select_none->setEnabled(true); 
    if (_selection->GetSelectedComponentNumber()==0) {
        _remove_selection->setEnabled(false); 
        _copy_selection->setEnabled(false); 
        _cut_selection->setEnabled(false); 
    } else {
        _remove_selection->setEnabled(true); 
        _copy_selection->setEnabled(true); 
        _cut_selection->setEnabled(true); 
   }
    _select_invert->setEnabled(true);
    _paste_selection->setEnabled(true);
}
