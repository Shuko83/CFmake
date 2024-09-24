#include "QActionToToolButton.h"
#include <QAction>;

using namespace StreamWork::SwGui;
using namespace StreamWork::SwCore;

const QString QACTION = QStringLiteral("QAction");
const QString WIDGET = QStringLiteral("Widget");

//---------------------------------------------------
QActionToToolButton::QActionToToolButton() 
	: SwAssistedComponent()
	, _toolButton (new QToolButton)
{
    //SwAssistedComponent
    setConsumerServiceAvaibility( true );
    setProviderServiceAvaibility( true );
    
    //tool button
    _toolButton->setVisible( false );
}

//---------------------------------------------------
QActionToToolButton::~QActionToToolButton()
{
    unconsumeInterface(QACTION);
    unprovideInterface(WIDGET);

	delete _toolButton;
}

//---------------------------------------------------
void QActionToToolButton::initializeComponent() throw( SwException )
{
    consumeInterface<QAction>(QACTION);
    provideInterface<QWidget>(WIDGET, _toolButton);
    
    createPropertiesForThisObject( QString(), true );
}

//---------------------------------------------------
void QActionToToolButton::interfaceAvailable( QString interface_name )
{
    if( interface_name == QACTION )
    {
        _toolButton->setDefaultAction( getInterface<QAction>(QACTION) );
        connect( _toolButton->defaultAction(), &QAction::changed, this, &QActionToToolButton::updateVisibilityAction );
        
        updateVisibilityAction();
    }
}

//---------------------------------------------------
void QActionToToolButton::interfaceUnavailable( QString interface_name )
{
    if( interface_name == QACTION )
    {
        disconnect( _toolButton->defaultAction(), &QAction::changed, this, &QActionToToolButton::updateVisibilityAction );
        _toolButton->setDefaultAction(nullptr);
        
        updateVisibilityAction();
    }
}

//---------------------------------------------------
void QActionToToolButton::updateVisibilityAction()
{
    QAction * tmp = _toolButton->defaultAction();
    if( tmp )
        _toolButton->setVisible( tmp->isVisible() );
    else
        _toolButton->setVisible( false );
}
