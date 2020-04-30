/*!
 \file _SwGuiCompWidget.cpp
 \brief Implementation of the Class _SwGuiCompWidget generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
 */
#include <QDesktopWidget>
#include <QApplication>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompWidget.h"

using namespace StreamWork::SwCore;

#define CL_WIDGET_INTERFACE_NAME "Widget_%1"
#define SW_SHOW_NORMAL 0
#define SW_SHOW_CENTERED 1
#define SW_SHOW_FULLSCREEN 2
#define SW_SHOW_MAXIMIZED 3
#define SW_SHOW_MINIMIZED 4

//-----------------------------------------------------------------------
_SwGuiCompWidget::_SwGuiCompWidget() : SwComponent_Class()
{
    _provider_service = NULL;
    _consumer_service = NULL;
    _properties_service = NULL;
    _widget = NULL;
    _widgets_nb = 0;
    _tmp_handle_widget = NULL;
    _registered_widgets_nb = 0;
    _isBackgroundTransparent = false;
    _show_mode.AddKey( SW_SHOW_NORMAL, "None" );
    _show_mode.AddKey( SW_SHOW_CENTERED, "Centered" );
    _show_mode.AddKey( SW_SHOW_FULLSCREEN, "FullScreen" );
    _show_mode.AddKey( SW_SHOW_MAXIMIZED, "Maximized" );
    _show_mode.AddKey( SW_SHOW_MINIMIZED, "Minimized" );
    _show_mode.FromInt( SW_SHOW_NORMAL );
    
    _flags_mode.AddKey( Qt::Widget, "Widget" );
    _flags_mode.AddKey( Qt::Window, "Window" );
    _flags_mode.AddKey( Qt::Dialog, "Dialog" );
    _flags_mode.AddKey( Qt::Sheet, "Sheet" );
    _flags_mode.AddKey( Qt::Drawer, "Drawer" );
    _flags_mode.AddKey( Qt::Popup, "Popup" );
    _flags_mode.AddKey( Qt::Tool, "Tool" );
    _flags_mode.AddKey( Qt::ToolTip, "ToolTip" );
    _flags_mode.AddKey( Qt::SplashScreen, "SplashScreen" );
    _flags_mode.AddKey( Qt::FramelessWindowHint, "FramelessWindowHint" );
    _flags_mode.FromInt( Qt::Widget );
}

//-----------------------------------------------------------------------
_SwGuiCompWidget::~_SwGuiCompWidget()
{

    QString interface_name;
    for( uint i = 0; i < _widgets_nb; i++ )
    {
        interface_name = QString( CL_WIDGET_INTERFACE_NAME ).arg( i );
        _consumer_service->UnregisterConsumedInterface( interface_name );
    }
    
    _provider_service->UnregisterProvidedInterface( "Widget" );
    
    //Desenregistrement des services
    this->UnregisterService( _consumer_service->GetServiceName() );
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _widget;
}

//-----------------------------------------------------------------------
void _SwGuiCompWidget::InitializeResources() throw( SwException )
{
    //Creation des service
    _consumer_service = new SwInterfaces_Consumer_Class( this );
    _provider_service = new SwInterfaces_Provider_Class( this );
    _properties_service = new SwProperties_Class( this );
    //Creation de l'interface principale
    _widget = new QWidget();
    //_widget->setStyleSheet("QWidget {background: red}");
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _consumer_service );
    this->RegisterService( _provider_service );
    
    //Exportation de l'interface QWidget
    _provider_service->RegisterProvidedInterface<QWidget>( "Widget", _widget );
    
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver( this );
    
    _properties_service->CreatePropertiesForQObject( _widget, "QWidget" );
    _properties_service->CreatePropertiesForQObject( this, "", true );
    
    //Gestion des widgets
    _widgets_nb_property = _properties_service->CreateProperty<uint>( "nb_widgets" );
    if( _widgets_nb_property == NULL )
    {
        if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Warning, QString( "Fail to register nb_widgets property\n" ) );
    }
    _widgets_nb_property->SetDescription( "Define how many QWidget interfaces this component accept" );
    _widgets_nb_property->SetValue( QVariant( _widgets_nb ) );
    _widgets_nb_property->GetOnChangeSignal().iconnect( *this, &_SwGuiCompWidget::OnPropertyChange );
    
    _show_property = _properties_service->CreateProperty<SwEnum>( "ShowMode" );
    if( _show_property != NULL )
    {
        QVariant variant;
        variant.setValue( _show_mode );
        _show_property->SetValue( variant );
        _show_property->SetDescription( "Show Mode" );
        _show_property->GetOnChangeSignal().iconnect( *this, &_SwGuiCompWidget::OnPropertyChange );
    }
    
    
    _flags_property = _properties_service->CreateProperty<SwEnum>( "FlagMode" );
    if( _flags_property != NULL )
    {
        QVariant variant;
        variant.setValue( _flags_mode );
        _flags_property->SetValue( variant );
        _flags_property->SetDescription( "Flags Mode" );
        _flags_property->GetOnChangeSignal().iconnect( *this, &_SwGuiCompWidget::OnPropertyChange );
    }
    
    _backgroundTransparency_property = _properties_service->CreateProperty<bool>( "TranslucentBackground" );
    if( _backgroundTransparency_property != NULL )
    {
        QVariant variant;
        variant.setValue( _isBackgroundTransparent );
        _backgroundTransparency_property->SetValue( variant );
        _backgroundTransparency_property->SetDescription( "Set the background of the widget transparent" );
        _backgroundTransparency_property->GetOnChangeSignal().iconnect( *this, &_SwGuiCompWidget::OnPropertyChange );
    }
    
    if( SW_APP->IsVerbose() )
        SW_APP->Logger().Log( LogLvl_Info, QString( "InitializeResources of SwGuiWidget done\n" ) );
        
}

//-----------------------------------------------------------------------
void _SwGuiCompWidget::OnPropertyChange( ISwProperty * property )
{
    uint val;
    QString interface_name;
    
    if( _widgets_nb_property == property )
    {
        val = property->GetValue().toUInt();
        if( val == _widgets_nb ) return;
        if( val < _widgets_nb )
        {
            for( uint i = val; i < _widgets_nb; i++ )
            {
                interface_name = QString( CL_WIDGET_INTERFACE_NAME ).arg( i );
                _consumer_service->UnregisterConsumedInterface( interface_name );
            }
        }
        else
        {
            for( uint i = _widgets_nb; i < val; i++ )
            {
                interface_name = QString( CL_WIDGET_INTERFACE_NAME ).arg( i );
                _widgets.insert( interface_name, nullptr );
                _consumer_service->RegisterConsumedInterface<QWidget>( interface_name, &_tmp_handle_widget );
            }
        }
        _widgets_nb = val;
    }
    if( _show_property == property )
    {
        SwEnum showmode = _show_property->GetValue().value<SwEnum>();
        _show_mode = showmode;
        showChanged();
    }
    if( _flags_property == property )
    {
        SwEnum flags_mode = _flags_property->GetValue().value<SwEnum>();
        _flags_mode = flags_mode;
        bool isVisible = _widget->isVisible();
        _widget->setWindowFlags( Qt::WindowFlags( _flags_mode.ToInt() ) );
        _widget->setVisible( isVisible );
    }
    if( _backgroundTransparency_property == property )
    {
        bool transparency = _backgroundTransparency_property->GetValue().toBool();
        _isBackgroundTransparent = transparency;
        bool isVisible = _widget->isVisible();
        if( _isBackgroundTransparent )
        {
            _widget->setAttribute( Qt::WA_TranslucentBackground, true );
            _flags_mode.FromInt( Qt::FramelessWindowHint );
            _widget->setWindowFlags( Qt::FramelessWindowHint );
        }
        else
        {
            _widget->setAttribute( Qt::WA_TranslucentBackground, false );
        }
        
        _widget->setVisible( isVisible );
    }
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------

//-----------------------------------------------------------------------
void _SwGuiCompWidget::BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
    QMap<QString, QWidget *>::iterator widget_it;
    //Si c'est un menu
    widget_it = _widgets.find( interface_name );
    if( widget_it != _widgets.end() )
    {
        if( widget_it.value() )
        {
            //Et qu'il etait defini, on le detache de la widgetbar
            widget_it.value()->setParent( nullptr );
            widget_it.value() = nullptr;
            _registered_widgets_nb--;
        }
    }
}

//-----------------------------------------------------------------------
void _SwGuiCompWidget::AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
    QMap<QString, QWidget *>::iterator widget_it;
        
    //Si c'est un widget
    widget_it = _widgets.find( interface_name );
    if( widget_it != _widgets.end() )
    {
        if( widget_it.value() == nullptr && _tmp_handle_widget )
        {
            //Et qu'il etait non defini, on l'enregistre et l'attache a la widgetbar
            widget_it.value() = _tmp_handle_widget;
            _tmp_handle_widget->setParent( _widget );
            _registered_widgets_nb++;
        }
    }
}

//-----------------------------------------------------------------------
void _SwGuiCompWidget::showChanged()
{
    switch( _show_mode.ToInt() )
    {
        case SW_SHOW_CENTERED:
        {
            _widget->showNormal();
            QDesktopWidget * desktop = QApplication::desktop();
            QRect screensize = desktop->screenGeometry( desktop->primaryScreen() );
            QRect windowSize = _widget->frameGeometry();
            QPoint slefttop( screensize.left(), screensize.top() );
            QPoint wlefttop;
            slefttop.setX( slefttop.x() + ( screensize.width() - windowSize.width() ) / 2 );
            slefttop.setY( slefttop.y() + ( screensize.height() - windowSize.height() ) / 2 );
            _widget->move( slefttop );
            
        }
        break;
        case SW_SHOW_FULLSCREEN:
            _widget->showFullScreen();
            break;
        case SW_SHOW_MAXIMIZED:
            _widget->showMaximized();
            break;
        case SW_SHOW_MINIMIZED:
            _widget->showMinimized();
            break;
        case SW_SHOW_NORMAL:
        default:
            break;
    }
}

//-----------------------------------------------------------------------
Qt::WindowFlags _SwGuiCompWidget::windowFlags()
{
    return _widget->windowFlags();
}

//-----------------------------------------------------------------------
void _SwGuiCompWidget::setWindowFlags( Qt::WindowFlags flag )
{
    _widget->setWindowFlags( flag );
}
