/*!
 \file _SwGuiCompPushButtonToQAction.cpp
 \brief Implementation of the Class _SwGuiCompPushButtonToQAction generant un QWidget
 \version 1.0
 \date 21-novembre-2011
 \author ATN
*/

#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompPushButtonToQAction.h"

/*! \brief Constructeur */
_SwGuiCompPushButtonToQAction::_SwGuiCompPushButtonToQAction(): SwComponent_Class()
{
    _provider_service = NULL;
    _consumer_service = NULL;
    _properties_service = NULL;
    pushButton = NULL;
    
    _handle_action = NULL;
    
    //  _i_Widget=(ISwWidget *)this; /* Remplacer null par le bon handle pour les interfaces fournies */
    
    
}

//*! \brief Destructeur */
_SwGuiCompPushButtonToQAction::~_SwGuiCompPushButtonToQAction()
{
    //Desenregistrement des services
    this->UnregisterService( _consumer_service->GetServiceName() );
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete pushButton;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompPushButtonToQAction::InitializeResources() throw( SwException )
{
    //Creation des service
    _consumer_service = new SwInterfaces_Consumer_Class( this ) ;
    _provider_service = new SwInterfaces_Provider_Class( this ) ;
    _properties_service = new SwProperties_Class( this );
    
    //Creation de l'interface principale
    pushButton = new QPushButton();
    
    bool res = true;
    if( !connect( pushButton, SIGNAL( clicked( bool ) ), ( QWidget * )this, SLOT( commandApply( bool ) ) ) )
        res = false;
        
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _consumer_service );
    this->RegisterService( _provider_service );
    
    _consumer_service->RegisterConsumedInterface<ISwAction>( "Action", &_handle_action );
    
    //Exportation de l'interface ISwWidget
    _provider_service->RegisterProvidedInterface<ISwWidget>( "Widget", this );
    //_provider_service->RegisterProvidedInterface<ISwWidget>("Widget", _i_Widget);
    
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver( this );
    
    //sortie des proprietes de la group box
    _properties_service->CreatePropertiesForQObject( pushButton, "QPushButton" );
    
    if( SW_APP->IsVerbose() )
        SW_APP->Logger().Log( LogLvl_Info,
                              QString( "InitializeResources of QPushButton done\n" ) );
                              
}
/*! \brief Callback sur les changements de propriétés*/
//void _SwGuiCompPushButtonToQAction::OnPropertyChange(ISwProperty * property) {
//}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompPushButtonToQAction::BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompPushButtonToQAction::AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host )
{
}
//---------------------------------------------------------------------
// Interface ISwMainWindow
//---------------------------------------------------------------------
/*! \brief Renvoie le menu
\return le menu */
QWidget * _SwGuiCompPushButtonToQAction::GetWidget()
{
    return pushButton;
}

void _SwGuiCompPushButtonToQAction::commandApply( bool state )
{
    _handle_action->GetAction().trigger();
}