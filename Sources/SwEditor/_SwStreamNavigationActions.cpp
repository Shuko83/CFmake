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

_SwStreamNavigationActions::_SwStreamNavigationActions( QObject * parent, SwInterfaces_Provider_Class * provider_service )
    : QObject( parent )
	, _navigator(nullptr)
	, _provider_service (provider_service)
{  
    _go_to_parent = new QAction( QIcon( ":/SwEditor/up.png" ), "Go &Up", this );
    _go_to_parent->setShortcut( QKeySequence( "Ctrl+U" ) );
    _go_to_parent->setStatusTip( "Go to parent component" );
    connect( _go_to_parent, &QAction::triggered, this, &_SwStreamNavigationActions::OnGoToParent);
    _provider_service->RegisterProvidedInterface<QAction>( "ActionGoToParent",_go_to_parent );
    
    _current_path = new QLineEdit( nullptr );
    _current_path->setStatusTip( "Current path in current stream" );
    _current_path->setToolTip( "Current path in current stream" );
    _current_path->setReadOnly( true );
    _provider_service->RegisterProvidedInterface<QWidget>( "CurrentPath", _current_path );
    
    _current_path->setEnabled( false );
    _go_to_parent->setEnabled( false );
}

_SwStreamNavigationActions::~_SwStreamNavigationActions()
{
    _provider_service->UnregisterProvidedInterface( "CurrentPath" );
    _provider_service->UnregisterProvidedInterface( "ActionGoToParent" );
    delete _current_path;
    delete _go_to_parent;
}
/*! \brief Attach un stream manager */
void _SwStreamNavigationActions::AttachStreamNavigator( ISwEditorStreamNavigation * navigator )
{
    _navigator = navigator;
    _navigator->AttachNavigationObserver( this );
    Update();
}
/*! \briefdetach un stream manager */
void _SwStreamNavigationActions::DetachStreamNavigator()
{
    _navigator->DetachNavigationObserver( this );
    _navigator = nullptr;
    Update();
}
/*! \brief callback OnGoToParent*/
void _SwStreamNavigationActions::OnGoToParent()
{
    if( _navigator ) _navigator->GoToParent();
}
//---------------------------------------------------------------------
// Interface ISwObserver
//---------------------------------------------------------------------
/*! \brief methode appelée par l'observable*/
void _SwStreamNavigationActions::Update( StreamWork::SwCore::ISwObservable * sender )
{
    if( !_navigator )
    {
        _go_to_parent->setEnabled( false );
        _current_path->setEnabled( false );
        return;
    }
    if( _navigator->GetCurrentComponent() )
    {
        _current_path->setEnabled( true );
        _current_path->setText( _navigator->GetCurrentPath() );
    }
    else
    {
        _current_path->setEnabled( false );
    }
    if( _navigator->GetCurrentComponent() && _navigator->GetCurrentComponent()->GetParent() ) _go_to_parent->setEnabled( true );
    else _go_to_parent->setEnabled( false );
}
