/*!
 \file _SwGuiCompGroupBox.cpp
 \brief Implementation of the Class _SwGuiCompGroupBox generant un QWidget
 \version 1.0
 \date 26-octobre-2009 18:59:26
 \author PGA
*/

#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompGroupBox.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*! \brief Constructeur */
_SwGuiCompGroupBox::_SwGuiCompGroupBox(): SwComponent_Class()
{
    _provider_service = NULL;
    _consumer_service = NULL;
    _properties_service = NULL;
    _groupBox = NULL;
    _layout = NULL;
}

/*! \brief Destructeur */
_SwGuiCompGroupBox::~_SwGuiCompGroupBox()
{
    //Desenregistrement des services
    this->UnregisterService( _consumer_service->GetServiceName() );
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _groupBox;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompGroupBox::InitializeResources() throw( SwException )
{
    //Creation des service
    _consumer_service = new SwInterfaces_Consumer_Class( this ) ;
    _provider_service = new SwInterfaces_Provider_Class( this ) ;
    _properties_service = new SwProperties_Class( this );
    
    //Creation de l'interface principale
    _groupBox = new QGroupBox();
    
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _consumer_service );
    this->RegisterService( _provider_service );
    
    //Exportation de l'interface ISwWidget
    _provider_service->RegisterProvidedInterface<ISwWidget>( "Widget", this );
    
    //Consomme un layout
    _consumer_service->RegisterConsumedInterface<ISwLayout>( "Layout",
            &_layout );
            
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver( this );
    
    //sortie des proprietes de la group box
    _properties_service->CreatePropertiesForQObject( _groupBox, "QGroupBox" );
    
    if( SW_APP->IsVerbose() )
        SW_APP->Logger().Log( LogLvl_Info,
                              QString( "InitializeResources of SwGroupBox done\n" ) );
                              
}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompGroupBox::OnPropertyChange( ISwProperty * property )
{
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompGroupBox::BeforeInterfaceAvailabilityChange( QString interface_name,
        SwComponent_Class * provider_host )
{
    //Si c'est un layout
    if( _layout != NULL )
    {
        _layout->LiberateLayout();
    }
    
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompGroupBox::AfterInterfaceAvailabilityChange( QString interface_name,
        SwComponent_Class * provider_host )
{
    if( interface_name == "Layout" && _layout != 0 )
    {
        _groupBox->setLayout( &_layout->GetLayout() );
    }
}
//---------------------------------------------------------------------
// Interface ISwMainWindow
//---------------------------------------------------------------------
/*! \brief Renvoie le menu
\return le menu */
QWidget * _SwGuiCompGroupBox::GetWidget()
{
    return _groupBox;
}

