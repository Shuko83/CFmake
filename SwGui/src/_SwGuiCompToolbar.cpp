/*!
 \file _SwGuiCompToolBar.cpp
 \brief Implementation of the Class _SwGuiCompToolBar generant une QToolBar
 \version 1.0
 \date 23-aoét-2006 18:59:26
 \author F.Bighelli
*/

#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "_SwGuiCompToolbar.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

#define CL_ACTION_INTERFACE_NAME "Action_%1"
#define CL_WIDGET_INTERFACE_NAME "Widget_%1"

/*! \brief Constructeur */
_SwGuiCompToolBar::_SwGuiCompToolBar()
	: SwComponent_Class()
	, _provider_service(nullptr)
	, _consumer_service(nullptr)
	, _properties_service(nullptr)
	, _toolbar(nullptr)
	, _actions_nb(0)
	, _tmp_handle_action(nullptr)
{  
}

/*! \brief Destructeur */
_SwGuiCompToolBar::~_SwGuiCompToolBar(){
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _toolbar;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent étre déclarés dans cette methodes*/
void _SwGuiCompToolBar::InitializeResources() throw(SwException) {
    //Creation des service
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    //Creation de l'interface principale
    _toolbar=new QToolBar();
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    //Exportation de l'interface ISwToolBar
    _provider_service->RegisterProvidedInterface<ISwToolBar>("ToolBar",(ISwToolBar *)this);

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

    //Enregistrement des propriétés
    _properties_service->CreatePropertiesForQObject(_toolbar,"QToolBar");

    //Gestion des actions
    _actions_nb_property=_properties_service->CreateProperty<uint>("nb_actions");
    if (!_actions_nb_property) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_actions property\n"));
    }
    _actions_nb_property->SetDescription("Define how many QAction interfaces this component accept");
    _actions_nb_property->SetValue(QVariant(_actions_nb));
    _actions_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompToolBar::OnPropertyChange);

    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiToolBar done\n"));

}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompToolBar::OnPropertyChange(ISwProperty * property) {
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
                _actions.insert(interface_name,nullptr);
                _consumer_service->RegisterConsumedInterface<QAction>(interface_name,&_tmp_handle_action);
            }
        }
        _actions_nb=val;
    }
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompToolBar::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,QAction *>::iterator action_it;
    QMap<QString, QWidget *>::iterator widget_it;

    //Si c'est une action
    action_it=_actions.find(interface_name);
    if (action_it!=_actions.end()) {
        if (action_it.value()) {
            //Et qu'ellle etait definie, on la detache de la toolbar
            //_toolbar->removeAction(&action_it.value()->GetAction());
            //action_it.value()=NULL;
        }
    }
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompToolBar::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,QAction *>::iterator action_it;
    QMap<QString, QWidget *>::iterator widget_it;

    //Si c'est une action
    action_it=_actions.find(interface_name);
    if (action_it!=_actions.end()) {
        if (!action_it.value() && _tmp_handle_action) {
            //Et qu'ellle etait non definie,on l'enregistre et l'attache a la toolbar
            action_it.value()=_tmp_handle_action;
            _toolbar->addAction(_tmp_handle_action);
        }
        return;
    }
}
//---------------------------------------------------------------------
// Interface ISwMainWindow
//---------------------------------------------------------------------
/*! \brief Renvoie le menu
\return le menu */
QToolBar & _SwGuiCompToolBar::GetToolBar() {
    return *_toolbar;
}
