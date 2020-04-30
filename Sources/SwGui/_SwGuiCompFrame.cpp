/*!
 \file _SwGuiCompFrame.cpp
 \brief Implementation of the Class _SwGuiCompFrame generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
 */

#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompFrame.h"

using namespace StreamWork::SwCore;

#define CL_WIDGET_INTERFACE_NAME "Widget_%1"

//-----------------------------------------------------------------------
_SwGuiCompFrame::_SwGuiCompFrame() : SwComponent_Class()
{
    _provider_service = NULL;
    _consumer_service = NULL;
    _properties_service = NULL;
    _frame = NULL;
    _widgets_nb = 0;
    _tmp_handle_widget = NULL;
    _registered_widgets_nb = 0;
}

//-----------------------------------------------------------------------
_SwGuiCompFrame::~_SwGuiCompFrame()
{
    for( uint i = 0; i < _widgets_nb; i++ )
    {
        QString interface_name = QString( CL_WIDGET_INTERFACE_NAME ).arg( i );
        _consumer_service->UnregisterConsumedInterface( interface_name );
    }
    
    _consumer_service->UnregisterConsumedInterface( "Layout" );
    _provider_service->UnregisterProvidedInterface( "Widget" );
    
    //Desenregistrement des services
    this->UnregisterService( _consumer_service->GetServiceName() );
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _frame;
}


//-----------------------------------------------------------------------
void _SwGuiCompFrame::InitializeResources() throw( SwException )
{
    //Creation des service
    _consumer_service = new SwInterfaces_Consumer_Class( this );
    _provider_service = new SwInterfaces_Provider_Class( this );
    _properties_service = new SwProperties_Class( this );
    //Creation de l'interface principale
    _frame = new QFrame();
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _consumer_service );
    this->RegisterService( _provider_service );
    //Exportation de l'interface QWidget
    _provider_service->RegisterProvidedInterface<QWidget>( "Widget", _frame );
    
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver( this );
    
    _properties_service->CreatePropertiesForQObject( _frame, "QFrame" );
    
    //Gestion des widgets
    _widgets_nb_property = _properties_service->CreateProperty<uint>( "nb_widgets" );
    if( _widgets_nb_property == NULL )
		if ( SW_APP->IsVerbose() ) SW_APP->Logger().Log(LogLvl_Warning, QString("Fail to register nb_widgets property\n"));
        
    _widgets_nb_property->SetDescription( "Define how many QWidget interfaces this component accept" );
    _widgets_nb_property->SetValue( QVariant( _widgets_nb ) );
    _widgets_nb_property->GetOnChangeSignal().iconnect( *this, &_SwGuiCompFrame::OnPropertyChange );
    
    
    if( SW_APP->IsVerbose() )
		SW_APP->Logger().Log(LogLvl_Info, QString("InitializeResources of SwFrameWidget done\n"));
        
}

//-----------------------------------------------------------------------
void _SwGuiCompFrame::OnPropertyChange( ISwProperty * property )
{
    uint val;
    QString interface_name;
    
    if( _widgets_nb_property == property)
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
                interface_name = QString( CL_WIDGET_INTERFACE_NAME ).arg( i );
                _widgets.insert( interface_name, nullptr );
                _consumer_service->RegisterConsumedInterface<QWidget>( interface_name, &_tmp_handle_widget );
            }
        }
        _widgets_nb = val;
    }
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------

//-----------------------------------------------------------------------
void _SwGuiCompFrame::BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
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
void _SwGuiCompFrame::AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
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
            _tmp_handle_widget->setParent( _frame );
            _registered_widgets_nb++;
        }
    }
}
