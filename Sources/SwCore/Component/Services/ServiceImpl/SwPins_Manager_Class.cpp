/*!
\file SwPins_Manager_Class.cpp
\date 12/04/2006
\brief service  de gestion des pins
\author  Big
\version 1.0
*/

/*
  * INCLUDES LOCAUX
  */
#include "SwPins_Manager_Class.h"
#include "SwApplication.h"
#include "SwCoreConstantes.h"
#include "SwException.h"
#include "SwMacros.h"
#include "SwAddress_ToolBox.h"

using namespace StreamWork::SwCore;

#define CL_SW_XML_CONNECTION_NODE    "connection"
#define CL_SW_XML_CONNECTION_NODE_ATT_SOURCE    "source"
#define CL_SW_XML_CONNECTION_NODE_ATT_TARGET    "target"
#define CL_SW_XML_CONNECTION_NODE_ATT_IDX     "idx"
#define CL_SW_XML_CONNECTION_NODE_ATT_TPATH    "target_path"

/*! \brief Construsteur */
SwPins_Manager_Class::SwPins_Manager_Class(SwComponent_Class * host) {
	_host_component = host;
	_observers.clear();
	_historic_index = 0;
}
/*! \brief Destrusteur */
SwPins_Manager_Class::~SwPins_Manager_Class(){
	Liberate();
	_observers.clear();
}
/*! \brief Enregistre un nouveau pin*/
void SwPins_Manager_Class::RegisterPin(SwPin * pin){
	QMap<QString, SwPin *>::iterator it;
	QSet<ISwPins_ManagerListener *>::const_iterator ito;

	it = _pins.find(pin->GetName());
	if (it != _pins.end()) {
		QString msg = QString("In component %1\nFailed to register because a pin is registered with the same name %2").arg(_host_component->GetName()).arg(pin->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	_pins.insert(pin->GetName(), pin);
	//Signalisation au niveau des observeurs
	for (ito = _observers.begin(); ito != _observers.end(); ito++) {
		(*ito)->OnAddPin(pin);
	}
}
/*! \brief Desenregistre un pin existant*/
void SwPins_Manager_Class::UnregisterPin(SwPin *pin){
	QMap<QString, SwPin *>::iterator it;
	QSet<ISwPins_ManagerListener *>::const_iterator ito;

	it = _pins.find(pin->GetName());
	if (it == _pins.end()) {
		QString msg = QString("In component %1\nFailed to unregister because pin(%2) is unknown").arg(_host_component->GetName()).arg(pin->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	//Si le pin est connecte, deconnexion
	if (pin->GetConnected() != NULL) {
		DisconnectPin(pin->GetName());
	}
	//Signalisation au niveau des observeurs
	for (ito = _observers.begin(); ito != _observers.end(); ito++) {
		(*ito)->OnRemovePin(pin);
	}
	//Suppression
	_pins.erase(it);
}
/*! \brief Connect un pin externe a un pin local*/
void SwPins_Manager_Class::ConnectRemotePinToLocalPin(QString local_pin, QString remote_pin, ISwPins_Manager * remote_manager){
	QMap<QString, SwPin *>::iterator it;
	QSet<ISwPins_ManagerListener *>::const_iterator ito;
	SwPin * r_pin;
	SwPin * l_pin;
	SwPins_Manager_Class * r_manager = dynamic_cast<SwPins_Manager_Class *>(remote_manager);

	if (r_manager == NULL) {
		QString msg = QString("In component %1\nFailed to make connection because remote manager is undefined").arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	it = _pins.find(local_pin);
	if (it == _pins.end()) {
		QString msg = QString("In component %1\nFailed to connect because pin(%2) is unknown").arg(_host_component->GetName()).arg(local_pin);
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	if (it.value()->GetConnected() != NULL) {
		QString msg = QString("In component %1\nFailed to connect because pin(%2) is already connected").arg(_host_component->GetName()).arg(local_pin);
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	l_pin = it.value();
	//Recuperation du pin distant
	r_pin = remote_manager->GetPinByName(remote_pin);
	if (r_pin == NULL) {
		QString msg = QString("In component %1\nFailed to connect because remote pin(%2) is unknown").arg(_host_component->GetName()).arg(remote_pin);
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	//On regarde que le pin distant n'est pas connecte
	if (r_pin->GetConnected() != NULL) {
		return;
	}
	//Effectuer la comparison de type
	if (r_pin->GetType() != l_pin->GetType()) {
		QString msg = QString("In component %1\nFailed to make connection because types are different (%2!=%3)").arg(_host_component->GetName()).arg(l_pin->GetType()).arg(r_pin->GetType());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	//Effectuer la connection
	if (_historic_index)
	{
		l_pin->AssignRemote(r_pin, _historic_index);
		r_pin->AssignRemote(l_pin, _historic_index + 1);
		SW_APP->SetHistoricCpt(_historic_index + 1);
	}
	else
	{
		l_pin->AssignRemote(r_pin, SW_APP->GetHistoricCpt());
		r_pin->AssignRemote(l_pin, SW_APP->GetHistoricCpt());
	}
	//Signaler la connection au observers locaux
	for (ito = _observers.begin(); ito != _observers.end(); ito++) {
		(*ito)->OnConnectPin(l_pin, r_pin);
	}
	//Signaler la connection au observers distant
	for (ito = r_manager->_observers.begin(); ito != r_manager->_observers.end(); ito++) {
		(*ito)->OnConnectPin(r_pin, l_pin);
	}
}
/*! \brief Deconnecte un pin existant*/
void SwPins_Manager_Class::DisconnectPin(QString local_pin){
	QMap<QString, SwPin *>::iterator it;
	QSet<ISwPins_ManagerListener *>::const_iterator ito;
	SwPin * r_pin;
	SwPin * l_pin;

	it = _pins.find(local_pin);
	if (it == _pins.end()) {
		QString msg = QString("In component %1\nFailed to disconnect because pin(%2) is unknown").arg(_host_component->GetName()).arg(local_pin);
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	if (it.value()->GetConnected() == NULL) {
		QString msg = QString("In component %1\nFailed to disconnect because pin(%2) is not connected").arg(_host_component->GetName()).arg(local_pin);
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	l_pin = it.value();
	r_pin = it.value()->GetConnected();
	//Recuperation du manager distant
	SwPins_Manager_Class * r_manager = dynamic_cast<SwPins_Manager_Class *>(r_pin->GetManager());
	if (r_manager == NULL) {
		QString msg = QString("In component %1\nFailed to make deconnection because remote manager is undefined").arg(_host_component->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	//Signaler la deconnection au observers
	for (ito = _observers.begin(); ito != _observers.end(); ito++) {
		(*ito)->OnDisconnectPin(l_pin, r_pin);
	}
	//Signaler la deconnection au observers distants
	for (ito = r_manager->_observers.begin(); ito != r_manager->_observers.end(); ito++) {
		(*ito)->OnDisconnectPin(r_pin, l_pin);
	}
	//Deconnecte
	r_pin->Desassign();
	l_pin->Desassign();
}
/*! \brief Enregistre un listener*/
void SwPins_Manager_Class::RegisterListener(ISwPins_ManagerListener *observer){
	QSet<ISwPins_ManagerListener *>::const_iterator it;

	it = _observers.find(observer);
	if (it != _observers.end()) {
        QString msg=QString("In component %1\nFailed to attach an already attached pin manager observer").arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)
    }
    _observers.insert(observer);
}
/*! \brief Desenregistre  un listener*/
void SwPins_Manager_Class::UnregisterListener(ISwPins_ManagerListener *observer){
    QSet<ISwPins_ManagerListener *>::iterator it;

    it=_observers.find(observer);
    if (it==_observers.end()) {
        return;
    }
    _observers.erase(it);
}
/*! \brief Acces a la liste des plugins*/
QList<SwPin *> SwPins_Manager_Class::GetPinList(){
    return _pins.values();
}
/*! \brief Acces a la liste des pins par nom*/
SwPin * SwPins_Manager_Class::GetPinByName(QString name){
    QMap<QString,SwPin *>::iterator it;

    it=_pins.find(name);
    if (it==_pins.end())
        return NULL;
    return it.value();
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistr�
lorsque ce premier se detruit ou une operation de desenregistrement du service est r�alis�e*/
void SwPins_Manager_Class::Liberate(){
    QMap<QString,SwPin *>::iterator it;
    //Deconnection de tous les pins connectes
    for (it=_pins.begin();it!=_pins.end();it++) {
        if (it.value()->GetConnected()!=NULL) {
            DisconnectPin(it.value()->GetName());
        }
    }
    //Destruction des pins restants
    it=_pins.begin();
    while (it!=_pins.end()) {
        SwPin * pin=it.value();
        UnregisterPin(pin);
        it=_pins.begin();
    }
}
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * SwPins_Manager_Class::GetHostComponent(){
    return _host_component;
}
//---------------------------------------------------------------------
// Interface ISwpersistent
//---------------------------------------------------------------------
/*! \brief methode permettant de charger des donnees */
void SwPins_Manager_Class::Load(QDomElement &elt,ISwFinalizerManager & finalizer_manager){
	QMap<quint64, _SwPinDesc *>::iterator itidx;
	quint64 index;
	bool result;

	//RAZ de la table de finalization (load multiples)
	_finalize_connections.clear();

	//recuperation des interfaces
	for (QDomElement connection_node = elt.firstChildElement(QString(CL_SW_XML_CONNECTION_NODE)); !connection_node.isNull(); connection_node = connection_node.nextSiblingElement(QString(CL_SW_XML_CONNECTION_NODE)))
	{
		if (connection_node.hasAttribute(CL_SW_XML_CONNECTION_NODE_ATT_SOURCE) &&
			connection_node.hasAttribute(CL_SW_XML_CONNECTION_NODE_ATT_TARGET) &&
			connection_node.hasAttribute(CL_SW_XML_CONNECTION_NODE_ATT_IDX) &&
			connection_node.hasAttribute(CL_SW_XML_CONNECTION_NODE_ATT_TPATH)) {
			//On recupere l'index
			index = connection_node.attribute(CL_SW_XML_CONNECTION_NODE_ATT_IDX).toULongLong(&result);
			if (result == false) {
				QString msg = QString("In component %1\nFail to load index of connection description because it's not a valid index %2").arg(_host_component->GetName()).arg(connection_node.attribute(CL_SW_XML_CONNECTION_NODE_ATT_IDX));
				LAUNCH_SWEXCEPTION("SwCore", msg)
			}
			//On verifie que l'index n'est pas deja utilise (ne doit JAMAIS ARRIVE)
			itidx = _finalize_connections.find(index);
			if (itidx != _finalize_connections.end()) {
				QString msg = QString("In component %1\nFail to load connection description because two index are equal to %2").arg(_host_component->GetName()).arg(index);
				LAUNCH_SWEXCEPTION("SwCore", msg)
			}
			//Creation d'une description
			_SwPinDesc * desc = new _SwPinDesc;
			desc->_src = connection_node.attribute(CL_SW_XML_CONNECTION_NODE_ATT_SOURCE);
			desc->_tgt = connection_node.attribute(CL_SW_XML_CONNECTION_NODE_ATT_TARGET);
			desc->_path = connection_node.attribute(CL_SW_XML_CONNECTION_NODE_ATT_TPATH);
			_finalize_connections.insert(index, desc);
			finalizer_manager.RegisterFinalization(index, this);
		}
	}
}

/*! \brief methode permettant de sauver des donnees */
void SwPins_Manager_Class::Save(QXmlStreamWriter& writer)
{
	for (auto it = _pins.constBegin(); it != _pins.constEnd(); ++it)
	{
		if (it.value()->GetConnected() != NULL)
		{
			//Creation du neoud
            writer.writeStartElement(CL_SW_XML_CONNECTION_NODE);
			//Enregistrement de la source
           	writer.writeAttribute(CL_SW_XML_CONNECTION_NODE_ATT_SOURCE, it.key());
			//Enregistrement de la cible
            writer.writeAttribute(CL_SW_XML_CONNECTION_NODE_ATT_TARGET, it.value()->GetConnected()->GetName());
			//Enregistrement de l'index pour la reconstruction
            writer.writeAttribute(CL_SW_XML_CONNECTION_NODE_ATT_IDX, QString::number(it.value()->GetHistoricConnectionDate()));
			//Enregistrement du path pour l'acces au parent
            QString path;
			try
			{
				//On tente un lien relatif
				path = SwAddress_ToolBox::BuildRelativePath(_host_component, it.value()->GetConnected()->GetManager()->GetHostComponent());
			}
			catch (SwException &)
			{
				//Impossible (pas le meme parent)
				//On recupere un lien universel
				path = SwAddress_ToolBox::BuildUniversalPath(it.value()->GetConnected()->GetManager()->GetHostComponent());
			}
			writer.writeAttribute(CL_SW_XML_CONNECTION_NODE_ATT_TPATH, path);

			writer.writeEndElement();
		}
	}
}
//---------------------------------------------------------------------
// Interface ISwFinalizer
//---------------------------------------------------------------------
/*! \brief finalize l'operation correspondant a l'index d'historique
\return false si la finalisation n'a pas eu lieu et true si ok*/
bool SwPins_Manager_Class::Finalize(quint64 historic_index){
	QMap<quint64, _SwPinDesc *>::iterator itidx;
	QMap<QString, SwPin *>::iterator itp;
	SwComponent_Class * host_provider;
	SwPins_Manager_Class * r_manager;
	SwPin * l_pin;
	SwPin * r_pin;
	_SwPinDesc * pin_desc;

	itidx = _finalize_connections.find(historic_index);
	if (itidx != _finalize_connections.end()) {
		//On a trouve le pin source concern�
		itp = _pins.find(itidx.value()->_src);
		if (itp == _pins.end()) {
			//Pin inconnu - on arrete
			return true;
		}
		l_pin = itp.value();
		//S'il est deja connect�, on ne fait rien
		if (l_pin->GetConnected() != NULL) {
			return true;
		}
		//Acces au fournisseur
		host_provider = SwAddress_ToolBox::FindTarget(itidx.value()->_path, _host_component);
		//S'il est defini et qu'il est producteur d'interfaces et qu'il produit l'interface qui nous interesse au bon type
		if (host_provider != NULL &&
			(r_manager = dynamic_cast<SwPins_Manager_Class *>(host_provider->QueryService(CG_SW_SERVICE_PINS_MANAGER))) != NULL &&
			(r_pin = r_manager->GetPinByName(itidx.value()->_tgt)) != NULL) {
			try {
				//On effectue la connection
				_historic_index = historic_index;
				ConnectRemotePinToLocalPin(l_pin->GetName(), r_pin->GetName(), r_manager);
			}
			catch (SwException & se) {
				SW_APP->Logger().Log(LogLvl_Critical, QString("Unable to connect %1 to %2:%s").arg(l_pin->GetName()).arg(r_pin->GetName()).arg(se.GetReason()));
			}
		}
		else {
			SW_APP->Logger().Log(LogLvl_Debug, QString("Unable to connect pin %1 of %2 (target not found)").arg(itidx.value()->_src).arg(_host_component->GetName()));
		}
		pin_desc = itidx.value();
		//Ok la finalization a ete effectue on libere l'index
		_finalize_connections.erase(itidx);
		delete pin_desc;
		//Et on renvoie ok
		return true;
		//Fin sans finalisation possible
	}
	return false;
}