/*!
 \file _SwEditorCompStreamTabBar.cpp
 \brief Implementation of the Class _SwEditorCompStreamTabBar une tabbar des streams editer
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QTabBar>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwEditorCompStreamTabBar.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
using namespace StreamWork::SwEditor;

/*! \brief Constructeur */
_SwEditorCompStreamTabBar::_SwEditorCompStreamTabBar(): SwComponent_Class()
{
    _provider_service = NULL;
    _consumer_service = NULL;
    _properties_service = NULL;
    _tabbar = NULL;
    _manager = NULL;
}
/*! \brief Destructeur */
_SwEditorCompStreamTabBar::~_SwEditorCompStreamTabBar()
{
    //Desenregistrement des services
    this->UnregisterService( _consumer_service->GetServiceName() );
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _tabbar;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwEditorCompStreamTabBar::InitializeResources() throw( SwException )
{
    //Creation des service
    _consumer_service = new SwInterfaces_Consumer_Class( this ) ;
    _provider_service = new SwInterfaces_Provider_Class( this ) ;
    _properties_service = new SwProperties_Class( this );
    //Creation de l'interface principale
    _tabbar = new _SwStreamsTabBar( NULL );
    _tabbar->setVisible( true );
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _consumer_service );
    this->RegisterService( _provider_service );
    
    //Exportation de l'interface ISwWidget
    _provider_service->RegisterProvidedInterface<ISwWidget>( "Widget", ( ISwWidget * )this );
    _properties_service->CreatePropertiesForQObject( _tabbar, "QTabBar" );
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver( this );
    _consumer_service->RegisterConsumedInterface<ISwEditorStreamsManager>( "StreamsManager", &_manager );
    //Fin
    if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Info, QString( "InitializeResources of SwEditorCompToolBox done\n" ) );
    
}
//---------------------------------------------------------------------
// Interface ISwWidget
//---------------------------------------------------------------------
/*! \brief Renvoie le menu
\return le menu */
QWidget * _SwEditorCompStreamTabBar::GetWidget()
{
    return _tabbar;
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwEditorCompStreamTabBar::BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
    if( _manager != NULL )
    {
        _tabbar->DetachStreamsManager();
    }
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwEditorCompStreamTabBar::AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
    if( _manager != NULL )
    {
        _tabbar->AttachStreamsManager( _manager );
    }
}
