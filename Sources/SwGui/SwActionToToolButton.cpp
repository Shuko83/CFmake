#include "SwActionToToolButton.h"
#include "ISwAction.h"

using namespace StreamWork::SwGui;
using namespace StreamWork::SwCore;

//---------------------------------------------------
SwActionToToolButton::SwActionToToolButton() : SwAssistedComponent()
{
    //SwAssistedComponent
    setConsumerServiceAvaibility( true );
    setProviderServiceAvaibility( true );
    
    //tool button
	_toolButton = new QToolButton();
    _toolButton->setVisible( false );
}

//---------------------------------------------------
SwActionToToolButton::~SwActionToToolButton()
{
    unconsumeInterface( "ISwAction" );
    unprovideInterface( "Widget" );

	delete _toolButton;
}

//---------------------------------------------------
void SwActionToToolButton::initializeComponent() throw( SwException )
{
    consummeInterface<ISwAction>( "ISwAction" );
    provideInterface<QWidget>( "Widget", _toolButton);
    
    createPropertiesForThisObject( QString(), true );
}

//---------------------------------------------------
void SwActionToToolButton::interfaceAvailable( QString interface_name )
{
    if( interface_name == "ISwAction" )
    {
        _toolButton->setDefaultAction( &getInterface<ISwAction>( "ISwAction" )->GetAction() );
        connect( _toolButton->defaultAction(), &QAction::changed, this, &SwActionToToolButton::updateVisibilityAction );
        
        updateVisibilityAction();
    }
}

//---------------------------------------------------
void SwActionToToolButton::interfaceUnavailable( QString interface_name )
{
    if( interface_name == "ISwAction" )
    {
        disconnect( _toolButton->defaultAction(), &QAction::changed, this, &SwActionToToolButton::updateVisibilityAction );
        _toolButton->setDefaultAction( NULL );
        
        updateVisibilityAction();
    }
}

//---------------------------------------------------
void SwActionToToolButton::updateVisibilityAction()
{
    QAction * tmp = _toolButton->defaultAction();
    if( tmp )
        _toolButton->setVisible( tmp->isVisible() );
    else
        _toolButton->setVisible( false );
}
