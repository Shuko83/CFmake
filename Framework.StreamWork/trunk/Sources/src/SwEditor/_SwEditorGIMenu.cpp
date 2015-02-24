/*!
 \file _SwEditorGIMenu.cpp
 \brief menu pour un graphics item composant
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <ISwAdminSetup.h>
#include <SwTime_ToolBox.h>
#include "_SwEditorGIMenu.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;
using namespace StreamWork::SwEditor;

/*! \brief constructeur*/
_SwEditorGIMenu::_SwEditorGIMenu(ISwEditorStreamOperations *op,ISwEditorStreamNavigation * nav,QWidget *parent):QMenu(parent) {
    QAction *action;
    _op=op;
    _nav=nav;
    _exe_serv=NULL;

    setWindowOpacity(0.9);
    setStyleSheet("* { border: 1px solid gray; background: #ccc; font-size:10px }");


    action=new QAction(QIcon(":/SwEditor/up.png"),"Go up",this);
    connect(action, SIGNAL(triggered()), this, SLOT(GoToParent()));
    addAction(action);

    addSeparator();

    action=new QAction("Add empty component",this);
    connect(action, SIGNAL(triggered()), this, SLOT(AddEmptyChild()));
    addAction(action);

    addSeparator();

    action=new QAction("Select all",this);
    connect(action, SIGNAL(triggered()), this, SLOT(SelectAll()));
    addAction(action);

    action=new QAction("Select none",this);
    connect(action, SIGNAL(triggered()), this, SLOT(SelectNone()));
    addAction(action);

    action=new QAction("Invert selection",this);
    connect(action, SIGNAL(triggered()), this, SLOT(SelectInvert()));
    addAction(action);

    addSeparator();

    action=new QAction(QIcon(":/SwEditor/editdelete.png"),"Remove",this);
    connect(action, SIGNAL(triggered()), this, SLOT(RemoveSelection()));
    addAction(action);
    if (_op->GetSelectedComponentNumber()==0) action->setEnabled(false);

    action=new QAction(QIcon(":/SwEditor/editcopy.png"),"Copy",this);
    connect(action, SIGNAL(triggered()), this, SLOT(CopySelection()));
    addAction(action);
    if (_op->GetSelectedComponentNumber()==0) action->setEnabled(false);

    action=new QAction(QIcon(":/SwEditor/editpaste.png"),"Paste",this);
    connect(action, SIGNAL(triggered()), this, SLOT(PasteSelection()));
    addAction(action);

    action=new QAction(QIcon(":/SwEditor/editcut.png"),"Cut",this);
    connect(action, SIGNAL(triggered()), this, SLOT(CutSelection()));
    addAction(action);
    if (_op->GetSelectedComponentNumber()==0) action->setEnabled(false);

    if (_op->GetSelectedComponentNumber()==1 && dynamic_cast<ISwAdminSetup *>(_op->GetSelectedComponent(0))!=NULL ) {
        addSeparator();
    
        action=new QAction("Setup",this);
        connect(action, SIGNAL(triggered()), this, SLOT(AdminSetup()));
        addAction(action);
    }

    if (_op->GetSelectedComponentNumber()==1 && 
        _op->GetSelectedComponent(0)!=NULL &&
        _op->GetSelectedComponent(0)->QueryService(CG_SW_SERVICE_EXECUTION)!=NULL) {
        _exe_serv=dynamic_cast<ISwExecution_Service *>(_op->GetSelectedComponent(0)->QueryService(CG_SW_SERVICE_EXECUTION));
        addSeparator();
        if (_exe_serv->IsExecutionStopped()) {
            action=new QAction(QIcon(":/SwEditor/b_start.png"),"Start",this);
            connect(action, SIGNAL(triggered()), this, SLOT(StartExecution()));
            addAction(action);
        } else {
            action=new QAction(QIcon(":/SwEditor/b_stop.png"),"Stop",this);
            connect(action, SIGNAL(triggered()), this, SLOT(StopExecution()));
            addAction(action);
        }
    }
}
/*! \brief destructeur*/
_SwEditorGIMenu::~_SwEditorGIMenu() {
}
//-------------------------------------------------------------
// Navigation dans l'arbre du stream
//-------------------------------------------------------------
    /*! \brief Remonter au parent*/
void _SwEditorGIMenu::GoToParent() {
    _nav->GoToParent();
}
//-------------------------------------------------------------
// Operations de la selection 
//-------------------------------------------------------------
/*! \brief Tout selectionner*/
void _SwEditorGIMenu::SelectAll() {
    _op->SelectAll();
}
/*! \brief Tout Deselectionner*/
void _SwEditorGIMenu::SelectNone(){
    _op->SelectNone();
}
/*! \brief Inverser la selection*/
void _SwEditorGIMenu::SelectInvert(){
    _op->SelectInvert();
}
//-------------------------------------------------------------
// Operations sur la selection
//-------------------------------------------------------------
/*! \brief Supprimer la selection courante*/
void _SwEditorGIMenu::RemoveSelection(){
    _op->RemoveSelection();
}
/*! \brief Copie la selection courante*/
void _SwEditorGIMenu::CopySelection(){
    _op->CopySelection();
}
/*! \brief Coller la selection courante*/
void _SwEditorGIMenu::PasteSelection(){
    _op->PasteSelection();
}
/*! \brief Couper la selection courante*/
void _SwEditorGIMenu::CutSelection(){
    _op->CutSelection();
}
//-------------------------------------------------------------
// Ajout composant
//-------------------------------------------------------------
/*! \brief Ajoute un component enfant au composant courant*/
void _SwEditorGIMenu::AddEmptyChild(){
    _op->AddChild();
}
//-------------------------------------------------------------
// AdminSetup
//-------------------------------------------------------------
/*! \brief Acces au setup du premier composant selectionné s'il en est capable*/
void _SwEditorGIMenu::AdminSetup() {
    ISwAdminSetup * admin_setup=dynamic_cast<ISwAdminSetup *>(_op->GetSelectedComponent(0));

    if (admin_setup!=NULL) {
        admin_setup->AdminSetup();    
    }
}
//-------------------------------------------------------------
// Execution
//-------------------------------------------------------------
/*! \brief Demarrage de l'execution*/
void  _SwEditorGIMenu::StartExecution() {
    _exe_serv->StartExecution();
}
/*! \brief Arret de l'execution*/
void  _SwEditorGIMenu::StopExecution() {
    _exe_serv->AskForStopExecution();
    while (!_exe_serv->IsExecutionStopped()) {
        SwTime_ToolBox::InternalSleep(10);    
    }
    _exe_serv->StopExecution();
}
