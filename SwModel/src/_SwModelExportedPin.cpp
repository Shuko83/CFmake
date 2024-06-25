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
/*! \brief Effectue la construction */
void _SwModelExportedPin::SpecificBuild() {
    _internal_pins_manager=dynamic_cast<SwPins_Manager_Class *>(_model_host->QueryService(CG_SW_SERVICE_PINS_MANAGER)); 
    //Creation du pin interne
    _internal_pin=new SwPin(_internal_pins_manager,_exported_name,_itype);
    _internal_pin->RegisterListener(this);
    _internal_pins_manager->RegisterPin(_internal_pin);

}
/*! \brief Effectue la destruction */
void _SwModelExportedPin::SpecificDestroy(){
    //Suppression du pin interne
    _internal_pin->UnregisterListener(this);
    _internal_pins_manager->UnregisterPin(_internal_pin);
    delete _internal_pin;

    _internal_pin=NULL;
	_internal_pins_manager=NULL;

}
/*! \brief Specific bind */
void _SwModelExportedPin::SpecificBind(){
    //Recuperation des managers
    _external_pins_manager=dynamic_cast<SwPins_Manager_Class *>(_model->QueryService(CG_SW_SERVICE_PINS_MANAGER)); 
    OnAddPin();
}
/*! \brief Defait la liaison */
void _SwModelExportedPin::SpecificUnbind(){
     //Destruction du pin s'il existe
    OnRemovePin();
    //Deconnection des signaux
	_external_pins_manager=NULL;

}
//-------------------------------------------------------------------------
//Interface ISwPins_ManagerListener
//-------------------------------------------------------------------------
/*! \brief Sur ajout d'un nouveau pin*/
void _SwModelExportedPin::OnAddPin() {
    //Creation du pin externe
    _external_pin=new SwPin(_external_pins_manager,_exported_name,_itype);
    _external_pin->RegisterListener(this);
    _external_pins_manager->RegisterPin(_external_pin);
}
/*! \brief Sur suppression d'un pin existant*/
void _SwModelExportedPin::OnRemovePin(){
    //Suppression du pin externe
    _external_pin->UnregisterListener(this);
    _external_pins_manager->UnregisterPin(_external_pin);
    delete _external_pin;
    _external_pin=NULL;
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
