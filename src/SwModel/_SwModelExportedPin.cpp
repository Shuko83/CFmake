/*!
 \file _SwModelExportedPin.cpp
 \brief Class implementant une entite exportable pin
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "_SwModelExportedPin.h"
#include "_SwModel_Class.h"
#include "_SwModelHost_Class.h"

/*! \brief Constructeur */
_SwModelExportedPin::_SwModelExportedPin():_SwModelExportedEntity() {
	_type=Ent_Pin;
	_host_pin_manager=NULL;
	_host_pin=NULL;
	_internal_pins_manager=NULL;
	_internal_pin=NULL;
	_external_pins_manager=NULL;
	_external_pin=NULL;
    _link_done=false;
}
/*! \brief Destructeur */
_SwModelExportedPin::~_SwModelExportedPin() {

}
/*! \brief Specific bind */
void _SwModelExportedPin::SpecificBind(){
    //Recuperation des managers
    _host_pin_manager=dynamic_cast<ISwPins_Manager *>(_host->QueryService(CG_SW_SERVICE_PINS_MANAGER));    
    _internal_pins_manager=dynamic_cast<SwPins_Manager_Class *>(_model_host->QueryService(CG_SW_SERVICE_PINS_MANAGER)); 
    _external_pins_manager=dynamic_cast<SwPins_Manager_Class *>(_model->QueryService(CG_SW_SERVICE_PINS_MANAGER)); 
    if (_host_pin_manager==NULL) {
        return;
    }
    //Connection des signaux
    _host_pin_manager->RegisterListener(this);
    //Creation du pin s'il existe
    if (_host_pin_manager->GetPinByName(_name)!=NULL)
        OnAddPin(_host_pin_manager->GetPinByName(_name));
}
/*! \brief Defait la liaison */
void _SwModelExportedPin::SpecificUnbind(){
    if (_host_pin_manager==NULL) {
        return;
    }
    //Destruction du pin s'il existe
    if (_host_pin!=NULL)
        OnRemovePin(_host_pin);
    //Deconnection des signaux
    _host_pin_manager->UnregisterListener(this);
	_host_pin_manager=NULL;
	_host_pin=NULL;
	_internal_pins_manager=NULL;
	_internal_pin=NULL;
	_external_pins_manager=NULL;
	_external_pin=NULL;
}
//-------------------------------------------------------------------------
//Interface ISwPins_ManagerListener
//-------------------------------------------------------------------------
/*! \brief Sur ajout d'un nouveau pin*/
void _SwModelExportedPin::OnAddPin(SwPin *pin) {
    //Creation du pin s'il correspond
    if (pin->GetName()!=_name)
        return;
    _host_pin=pin;
    //Creation du pin interne
    _internal_pin=new SwPin(_internal_pins_manager,_exported_name+"_internal",_host_pin->GetType());
    _internal_pin->RegisterListener(this);
    _internal_pins_manager->RegisterPin(_internal_pin);
    //Connection au pin de l'hote
    _internal_pins_manager->ConnectRemotePinToLocalPin(_exported_name+"_internal",_name,_host_pin_manager);
    //Creation du pin externe
    _external_pin=new SwPin(_external_pins_manager,_exported_name,_host_pin->GetType());
    _external_pin->RegisterListener(this);
    _external_pins_manager->RegisterPin(_external_pin);
    _link_done=true;
}
/*! \brief Sur suppression d'un pin existant*/
void _SwModelExportedPin::OnRemovePin(SwPin *pin){
    //Destruction du pin
    if (_host_pin!=pin)
        return;
    //Suppression du pin interne
    _internal_pins_manager->UnregisterPin(_internal_pin);
    delete _internal_pin;
    _internal_pin=NULL;
    //Suppression du pin externe
    _external_pins_manager->UnregisterPin(_external_pin);
    delete _external_pin;
    _external_pin=NULL;
    _link_done=false;
}            
/*! \brief Sur connexion d'un pin*/
void _SwModelExportedPin::OnConnectPin(SwPin * local_pin,SwPin * remote_pin){

}            
/*! \brief Sur deconnexion d'un pin*/
void _SwModelExportedPin::OnDisconnectPin(SwPin * local_pin,SwPin * remote_pin){

}           
//-------------------------------------------------------------------------
//Interface ISwPin_Listener
//-------------------------------------------------------------------------
/*! \brief Sur reception d'une donnée*/
void _SwModelExportedPin::OnReceiveData(SwPin *p,SwData_Class *data){
    if (p==_internal_pin && _external_pin!=NULL) {
        _external_pin->SendData(data);       
    }
    if (p==_external_pin && _internal_pin!=NULL) {
        _internal_pin->SendData(data);       
    }
}            
