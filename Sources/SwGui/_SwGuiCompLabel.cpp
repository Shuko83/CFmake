/*!
 \file _SwGuiCompLabel.cpp
 \brief Implementation of the Class _SwGuiCompLabel generant un QWidget
 \version 1.0
 \date 26-octobre-2009 18:59:26
 \author PGA
*/

#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "_SwGuiCompLabel.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwGuiCompLabel::_SwGuiCompLabel(): SwComponent_Class()
{
    _provider_service = NULL;
    _consumer_service = NULL;
    _properties_service = NULL;
    _label = NULL;
}

/*! \brief Destructeur */
_SwGuiCompLabel::~_SwGuiCompLabel()
{
    //Desenregistrement des services
    this->UnregisterService( _consumer_service->GetServiceName() );
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _label;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompLabel::InitializeResources() throw( SwException )
{
    //Creation des service
    _consumer_service = new SwInterfaces_Consumer_Class( this ) ;
    _provider_service = new SwInterfaces_Provider_Class( this ) ;
    _properties_service = new SwProperties_Class( this );
    
    //Creation de l'interface principale
    _label = new QLabel();
    
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _consumer_service );
    this->RegisterService( _provider_service );
    
    //Exportation de l'interface QWidget
    _provider_service->RegisterProvidedInterface<QWidget>( "Widget", _label);
    
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver( this );
    
    //sortie des proprietes de la group box
    _properties_service->CreatePropertiesForQObject( _label, "QLabel" );
    
    if( SW_APP->IsVerbose() )
        SW_APP->Logger().Log( LogLvl_Info,
                              QString( "InitializeResources of SwLabel done\n" ) );
                              
}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompLabel::OnPropertyChange( ISwProperty * property )
{
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompLabel::BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompLabel::AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
}
