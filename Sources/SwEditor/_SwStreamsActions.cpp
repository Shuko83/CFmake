/*!
 \file _SwStreamsActions.cpp
 \brief Implementations des QActions relatives a un manager de stream
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QKeySequence>
#include <QApplication>
#include "_SwStreamsActions.h"

using namespace StreamWork::SwEditor;


_SwStreamsActions::_SwStreamsActions(QObject *parent,SwInterfaces_Provider_Class * provider_service)
	: QObject(parent)
	, _manager(nullptr)
	,_provider_service(provider_service)
{

    _new=new QAction(QIcon(":/SwEditor/new.png"),"&New",this);
    _new->setShortcut(QKeySequence("Ctrl+N"));
    _new->setStatusTip("Create new stream");
    connect(_new, &QAction::triggered, this, &_SwStreamsActions::OnNew);
    _provider_service->RegisterProvidedInterface<QAction>("ActionNew",_new);

    _open=new QAction(QIcon(":/SwEditor/open.png"),"&Open",this);
    _open->setShortcut(QKeySequence("Ctrl+O"));
    _open->setStatusTip("Open an existing stream");
    connect(_open, &QAction::triggered, this, &_SwStreamsActions::OnOpen);
    _provider_service->RegisterProvidedInterface<QAction>("ActionOpen",_open);

    _save=new QAction(QIcon(":/SwEditor/save.png"),"&Save",this);
    _save->setShortcut(QKeySequence("Ctrl+S"));
    _save->setStatusTip("Save current stream");
    connect(_save, &QAction::triggered, this, &_SwStreamsActions::OnSave);
    _provider_service->RegisterProvidedInterface<QAction>("ActionSave",_save);

    _saveas=new QAction(QIcon(":/SwEditor/save.png"),"&Save as ...",this);
    _saveas->setStatusTip("Save current stream as ...");
    connect(_saveas, &QAction::triggered, this, &_SwStreamsActions::OnSaveAs);
    _provider_service->RegisterProvidedInterface<QAction>("ActionSaveAs",_saveas);

    _close=new QAction(QIcon(":/SwEditor/close.png"),"Close",this);
    _close->setStatusTip("Close current stream");
    connect(_close, &QAction::triggered, this, &_SwStreamsActions::OnClose);
    _provider_service->RegisterProvidedInterface<QAction>("ActionClose",_close);

    _quit=new QAction("Quit",this);
    _quit->setStatusTip("Quit");
    connect(_quit, &QAction::triggered, this, &_SwStreamsActions::OnQuit);
    QApplication * gui_app=dynamic_cast<QApplication *>(qApp);
    gui_app->setQuitOnLastWindowClosed(false);
    gui_app->connect(gui_app, &QApplication::lastWindowClosed, this, &_SwStreamsActions::OnQuit);
    _provider_service->RegisterProvidedInterface<QAction>("ActionQuit",_quit);

    _new->setEnabled(false);
    _open->setEnabled(false);
    _save->setEnabled(false);
    _saveas->setEnabled(false);
    _close->setEnabled(false);
    _quit->setEnabled(false);
    _quit_in_progress=false;
}

_SwStreamsActions::~_SwStreamsActions()
{
    _provider_service->UnregisterProvidedInterface("ActionNew");
    _provider_service->UnregisterProvidedInterface("ActionOpen");
    _provider_service->UnregisterProvidedInterface("ActionSave");
    _provider_service->UnregisterProvidedInterface("ActionSaveAs");
    _provider_service->UnregisterProvidedInterface("ActionClose");
    _provider_service->UnregisterProvidedInterface("ActionQuit"); 

    delete _new;
    delete _open;
    delete _save;
    delete _saveas;
    delete _close;
    delete _quit;
}
/*! \brief Attach un stream manager */
void _SwStreamsActions::AttachStreamsManager(ISwEditorStreamsManager * manager) {
    _manager=manager;
    _manager->AttachStreamManagerObserver(this);
    _new->setEnabled(true);
    _open->setEnabled(true);
    Update();
}
/*! \briefdetach un stream manager */
void _SwStreamsActions::DetachStreamsManager() {
    _manager->DetachStreamManagerObserver(this); 
    _manager=nullptr;
    Update();
}
/*! \brief callback _new*/
void _SwStreamsActions::OnNew(){
    if (_manager) _manager->NewStream();
}
/*! \brief callback _open*/
void _SwStreamsActions::OnOpen(){
    if (_manager) _manager->OpenStream();
}
/*! \brief callback _save*/
void _SwStreamsActions::OnSave(){
    if (_manager) _manager->SaveStream();
}
/*! \brief callback _save*/
void _SwStreamsActions::OnSaveAs(){
    if (_manager) _manager->SaveStreamAs();
}
/*! \brief callback _close*/
void _SwStreamsActions::OnClose(){
    if (_manager) _manager->CloseStream();
}
/*! \brief callback _quit*/
void _SwStreamsActions::OnQuit() {
    if (_manager && !_quit_in_progress) {
        _quit_in_progress=true;    
        _manager->QuitEditor();
    }
}

//---------------------------------------------------------------------
// Interface ISwObserver
//---------------------------------------------------------------------
/*! \brief methode appelée par l'observable*/
void _SwStreamsActions::Update(StreamWork::SwCore::ISwObservable* sender) {
    if (!_manager) {
        _new->setEnabled(false);
        _open->setEnabled(false);
        _save->setEnabled(false);
        _saveas->setEnabled(false);
        _close->setEnabled(false);
        _quit->setEnabled(false);
        return;
    }
    if (_manager->GetCurrentStream()==-1 && _save->isEnabled()) _save->setEnabled(false);
    if (_manager->GetCurrentStream()==-1 && _saveas->isEnabled()) _saveas->setEnabled(false);
    if (_manager->GetCurrentStream()==-1 && _close->isEnabled()) _close->setEnabled(false);
    if (_manager->GetCurrentStream()!=-1 && !_save->isEnabled()) _save->setEnabled(true);
    if (_manager->GetCurrentStream()!=-1 && !_saveas->isEnabled()) _saveas->setEnabled(true);
    if (_manager->GetCurrentStream()!=-1 && !_close->isEnabled()) _close->setEnabled(true);
    _quit->setEnabled(true);

}
