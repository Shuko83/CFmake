/*!
 \file _SwGuiCompActionList.cpp
 \brief Implementation of the Class _SwGuiCompActionList generant une QMenu
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QMenuBar>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompActionList.h"
#include "..\src\gui\dialogs\qmessagebox.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

#define CL_ACTION_INTERFACE_NAME "Action_%1"

/*! \brief Constructeur */
_SwGuiCompActionList::_SwGuiCompActionList(): SwComponent_Class(){
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _actions_nb=0;
    _tmp_handle_action=NULL;
}
/*! \brief Destructeur */
_SwGuiCompActionList::~_SwGuiCompActionList(){
   _listeners.clear();
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
	_actionMap.clear();
	_actionList.clear();
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompActionList::InitializeResources() throw(SwException) {
    
	
	//Creation des service
	_consumer_service=new SwInterfaces_Consumer_Class(this) ;
	_provider_service=new SwInterfaces_Provider_Class(this) ;
	_properties_service=new SwProperties_Class(this);

	//Enregistrement des services
	this->RegisterService(_properties_service);
	this->RegisterService(_consumer_service);
	this->RegisterService(_provider_service);

	//Exportation de l'interface ISwMenu
	_provider_service->RegisterProvidedInterface<ISwActionList>("ActionList",(ISwActionList *)this);

	//S'enregistrer comme observer du consumer
	_consumer_service->AttachInterfacesConsumerObserver(this);

	//Gestion des actions
	_actions_nb_property=_properties_service->CreateProperty<uint>("nb_actions");
	if (_actions_nb_property==NULL) {
		if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_menus property\n"));
	}
	_actions_nb_property->SetDescription("Define how many ISwAction interfaces this component accept");  
	_actions_nb_property->SetValue(QVariant(_actions_nb));
	_actions_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompActionList::OnPropertyChange);


	if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiMenu done\n"));
	
}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompActionList::OnPropertyChange(ISwProperty * property) {
    uint val;
    QString interface_name;

    if (_actions_nb_property==property) {
        val=property->GetValue().toUInt();
        if (val==_actions_nb) return;
        if (val<_actions_nb) {
            for (uint i=val;i<_actions_nb;i++) {
                interface_name=QString(CL_ACTION_INTERFACE_NAME).arg(i);
                _consumer_service->UnregisterConsumedInterface(interface_name);
            }
        } else {
            for (uint i=_actions_nb;i<val;i++) {
                interface_name=QString(CL_ACTION_INTERFACE_NAME).arg(i);
                _actions.insert(interface_name,(ISwAction *)NULL);
                _consumer_service->RegisterConsumedInterface<ISwAction>(interface_name,&_tmp_handle_action);
            }
        }
        _actions_nb=val;
    }
    
    signalListChanged();
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompActionList::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {

    QMap<QString,ISwAction *>::iterator action_it;
    
    //Si c'est une action
    action_it=_actions.find(interface_name);
    if (action_it!=_actions.end()) {
        if (action_it.value()!=NULL) 
        {            
			bool ok = false;
			int indice = interface_name.split("_").at(1).toInt(&ok, 10);
			_actionMap.remove(indice);
            action_it.value()=NULL;
            
            signalListChanged();
        }
        return;
    } 
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompActionList::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,ISwAction *>::iterator action_it;
    

    //Si c'est une action
    action_it=_actions.find(interface_name);
    if (action_it!=_actions.end()) {
        if (action_it.value()==NULL && _tmp_handle_action!=NULL) {
            //Et qu'ellle etait non definie,on l'enregistre et l'attache a la menubar
            action_it.value()=_tmp_handle_action;
			bool ok = false;
			int indice = interface_name.split("_").at(1).toInt(&ok, 10);
			_actionMap.insert(indice, &(_tmp_handle_action->GetAction()));
            
            signalListChanged();
        }
        return;
    } 
    
}
//---------------------------------------------------------------------
// Interface ISwactionList
//---------------------------------------------------------------------
/*! \brief Renvoie les actions
\return le menu */
QList<QAction *> &  _SwGuiCompActionList::GetActions() 
{
	_actionList = _actionMap.values();
    return _actionList;
}

void _SwGuiCompActionList::addActionListListener(ISwActionListListener * observer) 
{
   _listeners.push_back(observer);
} 

void _SwGuiCompActionList::removeActionListListener(ISwActionListListener * observer) 
{
    _listeners.removeOne(observer);
}

void _SwGuiCompActionList::signalListChanged() 
{
    QList<ISwActionListListener *>::iterator it =  _listeners.begin();
    while (it != _listeners.end())
    {
      (*it)->listChanged(_actionMap.values());
      it++;
    }
}	