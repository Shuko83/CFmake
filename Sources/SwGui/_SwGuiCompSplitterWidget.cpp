/*!
 \file _SwGuiCompSplitterWidget.h
 \brief Implementation of the Class _SwGuiCompSplitterWidget generant un QSplitter
 \version 1.0
 \date 14-mai-2009
 \author PGA
*/

#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "_SwGuiCompSplitterWidget.h"

using namespace StreamWork::SwCore;

#define CL_WIDGET_INTERFACE_NAME "Widget_%1"

/*! \brief Constructeur */
_SwGuiCompSplitterWidget::_SwGuiCompSplitterWidget(): SwComponent_Class()
{
    _provider_service = NULL;
    _consumer_service = NULL;
    _properties_service = NULL;
    _splitterWidget = NULL;
    _widgets_nb = 0;
    _tmp_handle_widget = NULL;
}
/*! \brief Destructeur */
_SwGuiCompSplitterWidget::~_SwGuiCompSplitterWidget()
{
    //Desenregistrement des services
    this->UnregisterService( _consumer_service->GetServiceName() );
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    
    //destruction du widget principal
    delete _splitterWidget;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompSplitterWidget::InitializeResources() throw( SwException )
{
    //Creation des service
    _consumer_service = new SwInterfaces_Consumer_Class( this ) ;
    _provider_service = new SwInterfaces_Provider_Class( this ) ;
    _properties_service = new SwProperties_Class( this );
    
    //Creation de l'interface principale
    _splitterWidget = new QSplitter();
    
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _consumer_service );
    this->RegisterService( _provider_service );
    //Exportation de l'interface QWidget
    _provider_service->RegisterProvidedInterface<QWidget>( "Widget", _splitterWidget );
    
    
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver( this );
    
    _properties_service->CreatePropertiesForQObject( _splitterWidget, "QSplitter" );
    
    //Gestion des widgets
    _widgets_nb_property = _properties_service->CreateProperty<uint>( "nb_widgets" );
    if( _widgets_nb_property == NULL )
    {
        if( SW_APP->IsVerbose() )
            SW_APP->Logger().Log( LogLvl_Warning, QString( "Fail to register nb_widgets property\n" ) );
    }
    else
    {
        _widgets_nb_property->SetDescription( "Define how many QWidget interfaces this component accept" );
        _widgets_nb_property->SetValue( QVariant( _widgets_nb ) );
        _widgets_nb_property->GetOnChangeSignal().iconnect( *this, &_SwGuiCompSplitterWidget::OnPropertyChange );
    }
    
    if( SW_APP->IsVerbose() )
        SW_APP->Logger().Log( LogLvl_Info, QString( "InitializeResources of SwSplittableWidget done\n" ) );
        
}

/*! \brief reset de la taille des widgets  */
void _SwGuiCompSplitterWidget::resetWidgetSizes()
{
    QList<int> sizes;
    for( WidgetSizes::iterator it = _widgetSizes.begin(); it != _widgetSizes.end(); ++it )
    {
        sizes.append( it.value()->GetValue().toInt() );
    }
    _splitterWidget->setSizes( sizes );
}


/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompSplitterWidget::OnPropertyChange( ISwProperty * property )
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
                
                //suppression de la propriete taille
                WidgetSizes::iterator it = _widgetSizes.find( interface_name );
                if( it != _widgetSizes.end() )
                {
                    _properties_service->DestroyProperty( it.value()->GetName() );
                    _widgetSizes.remove( interface_name );
                }
            }
        }
        else
        {
            for( uint i = _widgets_nb; i < val; i++ )
            {
                interface_name = QString( CL_WIDGET_INTERFACE_NAME ).arg( i );
                _widgets.insert( interface_name, 0 );
                _consumer_service->RegisterConsumedInterface<QWidget>( interface_name,
                        &_tmp_handle_widget );
                        
                //creation d'une propriete pour la taille du widget dans le splitter
                ISwProperty * sizeProperty =
                    _properties_service->CreateProperty<int>( QString( "Sizes_" ) + interface_name );
                if( sizeProperty == NULL )
                {
                    if( SW_APP->IsVerbose() )
                        SW_APP->Logger().Log( LogLvl_Warning,
                                              QString( "Fail to register Initial Sizes property\n" ) );
                }
                else
                {
                    _widgetSizes.insert( interface_name, sizeProperty );
                    sizeProperty->SetDescription( "Initial size for this widget" );
                    sizeProperty->SetValue( 0 );
                    sizeProperty->GetOnChangeSignal().iconnect( *this, &_SwGuiCompSplitterWidget::OnPropertyChange );
                }
            }
        }
        _widgets_nb = val;
    }
    else
    {
        //parcours des proprietes
        WidgetSizes::iterator it = _widgetSizes.begin();
        while( it != _widgetSizes.end() && it.value() != property )
        {
            it++;
        }
        if( it != _widgetSizes.end() )  //c'est une propriete de la taille d'un des widgets
        {
            resetWidgetSizes();
        }
    }
    
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompSplitterWidget::BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
    QMap<QString, QWidget *>::iterator widget_it;
    //Si c'est un menu
    widget_it = _widgets.find( interface_name );
    if( widget_it != _widgets.end() )
    {
        if( widget_it.value() )
        {
            widget_it.value()->setParent( nullptr );
            //Fin
            widget_it.value() = nullptr;
        }
    }
}

/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompSplitterWidget::AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
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
            
            bool convertOk = false;
            int index = interface_name.section( '_', -1 ).toInt( &convertOk ); //index d'insertion
            if( convertOk && index >= 0 )
            {
                //ajout du nouveau widget a l'index specifie
                _splitterWidget->insertWidget( index, _tmp_handle_widget );
                //on reaffecte les tailles de chaque widget
                resetWidgetSizes();
            }
        }
    }
}
