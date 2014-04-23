/*!
\file SwPin.cpp
\date 12/04/2006
\brief Pin
\author  Big
\version 1.0
 */

#include <QtGlobal>
/*
  * INCLUDES LOCAUX
 */
#include "SwPin.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
SwPin::SwPin(ISwPins_Manager * manager,QString name,QString data_type) {
	_manager=manager;
	_pin_name=name;
	_data_type=data_type;
	_remote_pin=NULL;
    _connection_historic_date=0L;

}
/*! \brief Destructeur */
SwPin::~SwPin(){
	_listeners.clear();
	Q_ASSERT(_remote_pin==NULL);
}
/*! \brief Liste des listener */
void SwPin::ProcessReceivedData(SwData_Class * data){
    for (int i=0;i<_listeners.count();i++) {
		_listeners[i]->OnReceiveData(this,data);

    }


}
/*! \brief Acces au type de données pris en charge par le pin */
QString SwPin::GetType(){

	return _data_type;
}
/*! \brief Acces au nom du pin */
QString SwPin::GetName(){
	return _pin_name;
}
/*! \brief Permet au pin de recevoir une donnée */
void SwPin::PushData(SwData_Class * data){
	ProcessReceivedData(data);
}
/*! \brief Permet au pin d'envoyer une donnée */
void SwPin::SendData(SwData_Class * data) {
	if (_remote_pin!=NULL) {
		_remote_pin->PushData(data);
	}

}
/*! \brief Permet d'assigner un pin remote a ce pin */
void SwPin::AssignRemote(SwPin * remote_pin,quint64 historic_date){
	_remote_pin=remote_pin;
    _connection_historic_date=historic_date;
}
/*! \brief Permet de supprimer l'assignation du pin distant */
void SwPin::Desassign(){
	_remote_pin=NULL;
    _connection_historic_date=0L;

}
/*! \brief Permet de connaitre le pin connecte
\return NULL si aucun pin n'est connected*/
SwPin * SwPin::GetConnected(){
	return _remote_pin;
}
/*! \brief Permet d'acceder a la date de connection */
quint64 SwPin::GetHistoricConnectionDate() {
    return _connection_historic_date;
}
/*! \brief Permet de connaitre le manager du pin */
ISwPins_Manager * SwPin::GetManager(){
	return _manager;
}
/*! \brief Enregistre un listener*/
void SwPin::RegisterListener(ISwPin_Listener * listener){
	_listeners.push_back(listener);

}
/*! \brief Desenregistre  un listener*/
void SwPin::UnregisterListener(ISwPin_Listener * listener){
	int index=_listeners.indexOf(listener);
	if (index>=0)
        _listeners.removeAt(index);
}

