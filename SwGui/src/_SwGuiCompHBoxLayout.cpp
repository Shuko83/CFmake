/*!
 \file _SwGuiCompHBoxLayout.cpp
 \brief Implementation of the Class _SwGuiCompHBoxLayout generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
 */

#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "_SwGuiCompHBoxLayout.h"

#include <QWidget>

using namespace StreamWork::SwCore;

#define CL_WIDGET_INTERFACE_NAME "Widget_%1"
#define CL_LAYOUT_INTERFACE_NAME "Layout_%1"

//-----------------------------------------------------------------------
_SwGuiCompHBoxLayout::_SwGuiCompHBoxLayout() : SwComponent_Class()
{
    _provider_service = NULL;
    _consumer_service = NULL;
    _properties_service = NULL;
	_mainWidget = NULL;
    _layout = NULL;
    _widgets_nb = 0;
    _widgets_nb_property = NULL;
    _nb_childs = 0;
    _margin = 0;
    _spacing = 6;
    _enableSpacer = false;
    _spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding );
	_mainWidget = new QWidget();
	_layout = new QHBoxLayout(_mainWidget);
	_layout->setMargin(_margin);
	_layout->setSpacing(_spacing);
}

//-----------------------------------------------------------------------
_SwGuiCompHBoxLayout::~_SwGuiCompHBoxLayout()
{

    for( uint i = 0; i < _widgets_nb; i++ )
    {
        QString interface_name = QString( CL_WIDGET_INTERFACE_NAME ).arg( i );
        _consumer_service->UnregisterConsumedInterface( interface_name );
    }
    
    _provider_service->UnregisterProvidedInterface( "Layout" );
    
    //Desenregistrement des services
    this->UnregisterService( _consumer_service->GetServiceName() );
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
	delete _layout;
	delete _mainWidget;
    delete _spacer;
}

//-----------------------------------------------------------------------
void _SwGuiCompHBoxLayout::InitializeResources() throw( SwException )
{
    //Creation des service
    _consumer_service = new SwInterfaces_Consumer_Class( this );
    _provider_service = new SwInterfaces_Provider_Class( this );
    _properties_service = new SwProperties_Class( this );
    //Creation de l'interface principale
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _consumer_service );
    this->RegisterService( _provider_service );
    
    //Exportation de l'interface QWidget
    _provider_service->RegisterProvidedInterface<QWidget>( "Layout", _mainWidget);
    
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver( this );
    
    //Enregistrement des layouts
    _margin_property = _properties_service->CreateProperty<int>( "QHBoxLayout.margin" );
    _margin_property->SetDescription( "Define margin for layout" );
    _margin_property->SetValue( QVariant( _margin ) );
    _margin_property->GetOnChangeSignal().iconnect( *this, &_SwGuiCompHBoxLayout::OnPropertyChange );
    
    _spacing_property = _properties_service->CreateProperty<int>( "QHBoxLayout.spacing" );
    _spacing_property->SetDescription( "Define spacing for layout" );
    _spacing_property->SetValue( QVariant( _spacing ) );
    _spacing_property->GetOnChangeSignal().iconnect( *this, &_SwGuiCompHBoxLayout::OnPropertyChange );
    
    //Gestion des widgets
    _widgets_nb_property = _properties_service->CreateProperty<uint>( "nb_widgets" );
    if( _widgets_nb_property == NULL )
    {
        if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Warning, QString( "Fail to register nb_widgets property\n" ) );
    }
    _widgets_nb_property->SetDescription( "Define how many QWidget interfaces this component accept" );
    _widgets_nb_property->SetValue( QVariant( _widgets_nb ) );
    _widgets_nb_property->GetOnChangeSignal().iconnect( *this, &_SwGuiCompHBoxLayout::OnPropertyChange );
    
    if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Info, QString( "InitializeResources of SwGuiHBoxLayout done\n" ) );
    //Spacer
    _properties_service->CreatePropertiesForQObject( this, QString(), true );
    
}

//-----------------------------------------------------------------------
void _SwGuiCompHBoxLayout::OnPropertyChange( ISwProperty * property )
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
				interface_name = QString(CL_WIDGET_INTERFACE_NAME).arg(i);
                _consumer_service->UnregisterConsumedInterface( interface_name );
            }
        }
        else
        {
            for( uint i = _widgets_nb; i < val; i++ )
            {
				interface_name = QString(CL_WIDGET_INTERFACE_NAME).arg(i);
                _consumer_service->RegisterConsumedInterface<QWidget>( interface_name, &_tmp_handle_widget );
            }
        }
        _widgets_nb = val;
    }
    if( _margin_property == property )
    {
        _margin = property->GetValue().toInt();
        _layout->setMargin( _margin );
    }
    if( _spacing_property == property )
    {
        _spacing = property->GetValue().toInt();
        _layout->setSpacing( _spacing );
    }
    
}

//-----------------------------------------------------------------------
bool _SwGuiCompHBoxLayout::getEnableSpacer()
{
    return _enableSpacer;
}

//-----------------------------------------------------------------------
void _SwGuiCompHBoxLayout::setEnableSpacer( bool enable )
{
    if( _enableSpacer == enable )
        return;
    _enableSpacer = enable;
    if( _layout )
    {
        if( _enableSpacer )
        {
            _layout->addSpacerItem( _spacer );
            _layout->setStretch( _layout->count() - 1, 1 );
        }
        else
        {
            _layout->removeItem( _spacer );
        }
    }
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------

//-----------------------------------------------------------------------
void _SwGuiCompHBoxLayout::BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
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
            _widgets.erase( widget_it );
            _nb_childs--;
            int index = _ordered_childrens.indexOf( interface_name );
            if( index != -1 ) _ordered_childrens.removeAt( index );
        }
    }
}

//-----------------------------------------------------------------------
void _SwGuiCompHBoxLayout::AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
    QMap<QString, QWidget *>::iterator widget_it;
    QString interface_header = interface_name;
    
    interface_header.truncate( 1 );
    //Si c'est un widget
    widget_it = _widgets.find( interface_name );
    if( interface_header == QString( "W" ) && widget_it == _widgets.end() && _tmp_handle_widget )
    {
        _widgets.insert( interface_name, _tmp_handle_widget );
        if( _enableSpacer )
        {
            _layout->insertWidget( _layout->count() - 1, _tmp_handle_widget );
        }
        else
        {
            _layout->addWidget( _tmp_handle_widget );
        }
        _nb_childs++;
        _ordered_childrens.push_back( interface_name );
        return;
    }
}
