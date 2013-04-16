/*!
 \file __SwEditorStreamOperationsActions.cpp
 \brief Implementation of the Class __SwEditorStreamOperationsActions une tabbar des streams editer
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwEditorStreamOperationsActions.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
using namespace StreamWork::SwEditor;

/*! \brief Constructeur */
_SwEditorStreamOperationsActions::_SwEditorStreamOperationsActions(): SwComponent_Class(){
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _actions_object=NULL;
    _selection=NULL;
}
/*! \brief Destructeur */
_SwEditorStreamOperationsActions::~_SwEditorStreamOperationsActions(){
    _consumer_service->UnregisterConsumedInterface("StreamOperations");
    delete _actions_object;
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwEditorStreamOperationsActions::InitializeResources() throw(SwException) {
    //Creation des service
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);
    //Creation de l'interface principale
    _actions_object=new _SwStreamOperationsActions(NULL,_provider_service);

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);
    _consumer_service->RegisterConsumedInterface<ISwEditorStreamOperations>("StreamOperations",&_selection);
    //Fin
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwEditorStreamOperationsActions done\n"));

}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwEditorStreamOperationsActions::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    if (_selection!=NULL) {
        _actions_object->DetachStreamOperations();
    }
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwEditorStreamOperationsActions::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    if (_selection!=NULL) {
        _actions_object->AttachStreamOperations(_selection);
    }
}
