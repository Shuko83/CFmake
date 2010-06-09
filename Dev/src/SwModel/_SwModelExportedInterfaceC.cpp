/*!
 \file _SwModelExportedInterfaceC.cpp
 \brief  Class implementant une entite exportable interface consommée
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "_SwModelExportedInterfaceC.h"
#include "_SwModel_Class.h"
#include "_SwModelHost_Class.h"

/*! \brief Constructeur */
_SwModelExportedInterfaceC::_SwModelExportedInterfaceC():_SwModelExportedEntity() {
	_type=Ent_InterfaceC;
    _internal_provider=NULL;
    _external_consumer=NULL;
    _handle=NULL;
    _host_consumer=NULL;
    _interface_created=false;
}
/*! \brief Destructeur */
_SwModelExportedInterfaceC::~_SwModelExportedInterfaceC() {

}
/*! \brief Specific bind */
void _SwModelExportedInterfaceC::SpecificBind(){
    _host_consumer=dynamic_cast<ISwInterfaces_Consumer *>(_host->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));    
    _internal_provider=dynamic_cast<SwInterfaces_Provider_Class *>(_model_host->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));    
    _external_consumer=dynamic_cast<SwInterfaces_Consumer_Class *>(_model->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER)); 
    if (_host_consumer==NULL)
        return;
    _host_consumer->AttachInterfacesServices_Listener(this);
    //Si l'interface existe deja on la cree
    if (!_host_consumer->GetInterfaceType(_name).isEmpty()) {
        OnAddInterface(_host_consumer,_name);    
    }
}
/*! \brief Defait la liaison */
void _SwModelExportedInterfaceC::SpecificUnbind(){
    if (_host_consumer==NULL)
        return;
    //Si l'interface a été crée on la detruit
    if (_interface_created) {
        OnRemoveInterface(_host_consumer,_name);    
    }
    _host_consumer->DetachInterfacesServices_Listener(this);
    _host_consumer=NULL;
    _internal_provider=NULL;
    _external_consumer=NULL;
    _handle=NULL;
}
//-------------------------------------------------------------------------
//Interface ISwInterfaces_ServicesListener
//-------------------------------------------------------------------------
/*! \brief Sur ajout d'une nouvelle interface */
void _SwModelExportedInterfaceC::OnAddInterface(ISwInterfaces_Service * source,QString interface_name) {
    if (source==(ISwInterfaces_Service *)_host_consumer && _name==interface_name) {
        //Sur creation de l'interface a propager on crée les deux interfaces locales
        //Creation du producteur interne
        _internal_provider->RegisterProvidedInterfaceWithType(_exported_name,_host_consumer->GetInterfaceType(_name),_handle);
        _internal_provider->SetInterfaceUnavailable(_exported_name);
        //Connexion a l'hote
        _host_consumer->AttachProvider(_internal_provider,_name,_exported_name);
        //Creation du consommateur externe
        _external_consumer->RegisterConsumedInterfaceWithType(_exported_name,_host_consumer->GetInterfaceType(_name),&_handle);
        //Observation du consommateur externe
        _external_consumer->AttachInterfacesConsumerObserver(this);
        //Flag d'info
        _interface_created=true;

    }
}
/*! \brief Sur suppression d'une  interface */
void _SwModelExportedInterfaceC::OnRemoveInterface(ISwInterfaces_Service * source,QString interface_name){
    if (source==(ISwInterfaces_Service *)_host_consumer && _name==interface_name) {
        //Desobservation du consommateur externe
        _external_consumer->DetachInterfacesConsumerObserver(this);
        //Destruction du consommateur externe
        _external_consumer->UnregisterConsumedInterface(_exported_name);
        //Destruction du producteur interne
        _internal_provider->UnregisterProvidedInterface(_exported_name);
        //Flag d'info
        _interface_created=false;
    }
}            
/*! \brief Sur connection d'une interface */
void _SwModelExportedInterfaceC::OnConnectInterface(ISwInterfaces_Service * source,QString interface_name,ISwInterfaces_Service * remote_source,QString remote_interface_name){
}           
/*! \brief Sur deconnection d'une interface */
void _SwModelExportedInterfaceC::OnDisconnectInterface(ISwInterfaces_Service * source,QString interface_name,ISwInterfaces_Service * remote_source,QString remote_interface_name){

}            
//-------------------------------------------------------------------------
//Interface ISwInterfaces_ConsumerObserver
//-------------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwModelExportedInterfaceC::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    if (interface_name==_exported_name &&  _handle!=NULL) {
        //Rendre l'interface non disponible
        _internal_provider->SetInterfaceUnavailable(_exported_name);
    }
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwModelExportedInterfaceC::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    if (interface_name==_exported_name &&  _handle!=NULL) {
        //Rendre l'interface non disponible
        _internal_provider->SetInterfaceAvailable(_exported_name,_handle);
    }
}
