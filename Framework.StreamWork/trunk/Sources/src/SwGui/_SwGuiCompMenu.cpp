/*!
 \file _SwGuiCompMenu.cpp
 \brief Implementation of the Class _SwGuiCompMenu generant une QMenu
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QMenuBar>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompMenu.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


#define CL_MENU_INTERFACE_NAME "Menu_%1"
#define CL_ACTION_INTERFACE_NAME "Action_%1"

/*! \brief Constructeur */
_SwGuiCompMenu::_SwGuiCompMenu(): SwComponent_Class(){
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _menu=NULL;
    _menus_nb=0;
    _actions_nb=0;
    _tmp_handle_menu=NULL;
    _tmp_handle_action=NULL;
}
/*! \brief Destructeur */
_SwGuiCompMenu::~_SwGuiCompMenu(){
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _menu;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompMenu::InitializeResources() throw(SwException) {
    //Creation des service
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    //Creation de l'interface principale
    _menu=new QMenu();
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    //Exportation de l'interface ISwMenu
    _provider_service->RegisterProvidedInterface<ISwMenu>("Menu",(ISwMenu *)this);
   
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

    //Enregistrement des propriétés
    _properties_service->CreatePropertiesForQObject(_menu,"QMenu");

    //Gestion des menus
    _menus_nb_property=_properties_service->CreateProperty<uint>("nb_menus");
    if (_menus_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_menus property\n"));
    }
    _menus_nb_property->SetDescription("Define how many ISwMenu interfaces this component accept");  
    _menus_nb_property->SetValue(QVariant(_menus_nb));
    _menus_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompMenu::OnPropertyChange);
    //Gestion des actions
    _actions_nb_property=_properties_service->CreateProperty<uint>("nb_actions");
    if (_actions_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_menus property\n"));
    }
    _actions_nb_property->SetDescription("Define how many ISwAction interfaces this component accept");  
    _actions_nb_property->SetValue(QVariant(_actions_nb));
    _actions_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompMenu::OnPropertyChange);


    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiMenu done\n"));

}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompMenu::OnPropertyChange(ISwProperty * property) {
    uint val;
    QString interface_name;

    if (_menus_nb_property==property) {
        val=property->GetValue().toUInt();
        if (val==_menus_nb) return;
        if (val<_menus_nb) {
            for (uint i=val;i<_menus_nb;i++) {
                interface_name=QString(CL_MENU_INTERFACE_NAME).arg(i);
                _consumer_service->UnregisterConsumedInterface(interface_name);
            }
        } else {
            for (uint i=_menus_nb;i<val;i++) {
                interface_name=QString(CL_MENU_INTERFACE_NAME).arg(i);
                _menus.insert(interface_name,(ISwMenu *)NULL);
                _consumer_service->RegisterConsumedInterface<ISwMenu>(interface_name,&_tmp_handle_menu);
            }
        }
        _menus_nb=val;
    }
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
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompMenu::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,ISwMenu *>::iterator menu_it;
    QMap<QString,ISwAction *>::iterator action_it;
    
    //Si c'est un menu
    menu_it=_menus.find(interface_name);
    if (menu_it!=_menus.end()) {
        if (menu_it.value()!=NULL) {
            //Et qu'il etait defini, on le detache de la menubar
            menu_it.value()->GetMenu().setParent(NULL);  
            menu_it.value()=NULL;
        }
        return;
    } 
    //Si c'est une action
    action_it=_actions.find(interface_name);
    if (action_it!=_actions.end()) {
        if (action_it.value()!=NULL) {
            //Et qu'ellle etait definie, on la detache de la menubar
            _menu->removeAction(&action_it.value()->GetAction());
            action_it.value()=NULL;
        }
        return;
    } 
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompMenu::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,ISwMenu *>::iterator menu_it;
    QMap<QString,ISwAction *>::iterator action_it;
    
    //Si c'est un menu
    menu_it=_menus.find(interface_name);
    if (menu_it!=_menus.end()) {
        if (menu_it.value()==NULL  && _tmp_handle_menu!=NULL) {
            //Et qu'il etait non defini, on l'enregistre et l'attache a la menubar
            menu_it.value()=_tmp_handle_menu;  
            _menu->addMenu(&(_tmp_handle_menu->GetMenu()));
        }
        return;
    } 
    //Si c'est une action
    action_it=_actions.find(interface_name);
    if (action_it!=_actions.end()) {
        if (action_it.value()==NULL && _tmp_handle_action!=NULL) {
            //Et qu'ellle etait non definie,on l'enregistre et l'attache a la menubar
            action_it.value()=_tmp_handle_action;
            _menu->addAction(&(_tmp_handle_action->GetAction()));
        }
        return;
    } 

}
//---------------------------------------------------------------------
// Interface ISwMenu
//---------------------------------------------------------------------
/*! \brief Renvoie le menu
\return le menu */
QMenu & _SwGuiCompMenu::GetMenu() {
    return *_menu;
}

