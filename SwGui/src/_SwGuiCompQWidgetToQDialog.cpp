/**
  * Includes Globaux
  */
#include <QObject>
#include <QString>
#include <QDir>
#include <QFile>
#include <QWidget>
#include <QAction>
/**
  * Includes Locaux
  */
#include "_SwGuiCompQWidgetToQDialog.h"
#include "Main/SwMacros.h"

#define WIDGET_INTERFACE_NAME "Widget"
#define WIDGETP_INTERFACE_NAME "WidgetP"
#define QACTION_INTERFACE_NAME "QAction"

using namespace StreamWork::SwCore;

//-------------------------------------------------------------------------
_SwGuiCompQWidgetToQDialog::_SwGuiCompQWidgetToQDialog()
    : SwAssistedComponent()
    , _widget(nullptr)
    , _enableMaximize(false)
    , _saveDialogPosition(false)
    , _helper(CG_SW_SERVICE_MAINWINDOW)
    , _action(new QAction(this))
{
    connect( _action, &QAction::toggled, [ = ]( bool )
    {
        //Close est important car il permet de filtrer l'event en cas
        // de volonté de ne pas fermer la fenetre dans le widget
        if( _container.isVisible() )
            _container.close();
        else
            _container.show();
    } );
    
    connect( _action, &QAction::triggered, [ = ]( bool )
    {
        if( !_container.isVisible() )
            _container.show();
    } );
}

//-------------------------------------------------------------------------
_SwGuiCompQWidgetToQDialog::~_SwGuiCompQWidgetToQDialog()
{
    _helper.setCallback(nullptr);

    unconsumeInterface( WIDGET_INTERFACE_NAME );
    unprovideInterface( QACTION_INTERFACE_NAME );
    unprovideInterface( WIDGETP_INTERFACE_NAME );

    delete _action;
}

//-------------------------------------------------------------------------
void _SwGuiCompQWidgetToQDialog::initializeComponent() throw( SwException )
{
    _helper.setCallback(this, &_SwGuiCompQWidgetToQDialog::onService);
    
    consumeInterface<QWidget>( WIDGET_INTERFACE_NAME );
    
    provideInterface<QWidget>( WIDGETP_INTERFACE_NAME, &_container );
    provideInterface<QAction>( QACTION_INTERFACE_NAME, _action );
    
    getPropertiesService().CreatePropertyForQObject( this, "EnableMaximize" );
    getPropertiesService().CreatePropertyForQObject( this, "SaveDialogPosition" );
    createPropertiesForQObject( &_container, "Widget", true );
    createPropertiesForQObject( _action, "Action", true );
}


//---------------------------------------------------------------------------------
void _SwGuiCompQWidgetToQDialog::interfaceAvailable( QString interfaceName )
{
    if( interfaceName == WIDGET_INTERFACE_NAME )
    {
		_widget = getInterface<QWidget>( interfaceName );
        _container.setContentWidget( _widget );
    }
}

//---------------------------------------------------------------------------------
void _SwGuiCompQWidgetToQDialog::interfaceUnavailable( QString interfaceName )
{
    if( interfaceName == WIDGET_INTERFACE_NAME )
    {
        if( _widget )
            _widget->setParent( nullptr );
            
        _container.setContentWidget( nullptr );
    }
}

//-----------------------------------------------------------------------
void _SwGuiCompQWidgetToQDialog::onService( bool available )
{
    if( available )
    {
        _container.setParent( _helper.getService()->getMainWindow() );
        if( _enableMaximize )
            _container.setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint );
        else
            _container.setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint );
            
    }
    else
        _container.setParent( nullptr );
}
