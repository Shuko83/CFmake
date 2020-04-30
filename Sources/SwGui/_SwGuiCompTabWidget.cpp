/*!
 \file _SwGuiCompTabWidget.cpp
 \brief Implementation of the Class _SwGuiCompTabWidget generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompTabWidget.h"

using namespace StreamWork::SwCore;

#define CL_WIDGET_INTERFACE_NAME QStringLiteral("Widget_%1")

/*! \brief Constructeur */
_SwGuiCompTabWidget::_SwGuiCompTabWidget() : SwComponent_Class(),
    _provider_service( nullptr ),
    _consumer_service( nullptr ),
    _properties_service( nullptr ),
    _tabWidget( nullptr ),
    _widgets_nb( 0 ),
    _tmp_handle_widget( nullptr )
{

}
/*! \brief Destructeur */
_SwGuiCompTabWidget::~_SwGuiCompTabWidget()
{
    //Desenregistrement des services
    this->UnregisterService( _consumer_service->GetServiceName() );
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    _tabWidget->clear();
    delete _tabWidget;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompTabWidget::InitializeResources() throw( SwException )
{
    //Creation des service
    _consumer_service = new SwInterfaces_Consumer_Class( this ) ;
    _provider_service = new SwInterfaces_Provider_Class( this ) ;
    _properties_service = new SwProperties_Class( this );
    //Creation de l'interface principale
    _tabWidget = new QTabWidget();
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _consumer_service );
    this->RegisterService( _provider_service );
    //Exportation de l'interface QWidget
    _provider_service->RegisterProvidedInterface<QWidget>( "Widget", _tabWidget );
    
    
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver( this );
    
    _properties_service->CreatePropertiesForQObject( _tabWidget, QStringLiteral( "QTabWidget" ) );
    
    //Gestion des widgets
    _widgets_nb_property = _properties_service->CreateProperty<uint>( QStringLiteral( "nb_widgets" ) );
    if( _widgets_nb_property == NULL )
    {
        if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Warning, QStringLiteral( "Fail to register nb_widgets property\n" ) );
    }
    _widgets_nb_property->SetDescription( QStringLiteral( "Define how many QWidget interfaces this component accept" ) );
    _widgets_nb_property->SetValue( QVariant( _widgets_nb ) );
    _widgets_nb_property->GetOnChangeSignal().iconnect( *this, &_SwGuiCompTabWidget::OnPropertyChange );
    
    
    if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Info, QStringLiteral( "InitializeResources of SwFrameWidget done\n" ) );
    
}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompTabWidget::OnPropertyChange( ISwProperty * property )
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
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompTabWidget::BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
    QMap<QString, QWidget *>::iterator widget_it;
    //Si c'est un menu
    widget_it = _widgets.find( interface_name );
    if( widget_it != _widgets.end() )
    {
        if( widget_it.value() )
        {
            //Et qu'il etait defini, on le detache de la widgetbar
            //            int index=_tabWidget->indexOf(&widget_it.value());
            //if(index>=0) {
            //_tabWidget->removeTab(index);
            //} else {
            widget_it.value()->setParent( nullptr );
            //}
            //Fin
            widget_it.value() = nullptr;
        }
    }
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompTabWidget::AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
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
            _tabWidget->addTab( _tmp_handle_widget, _tmp_handle_widget->windowTitle() );
        }
    }
}
