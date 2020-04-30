/*!
 \file _SwGuiCompPluginsTree.cpp
 \brief Implementation of the Class _SwGuiCompPluginsTree generant un QTreeView contenant la liste des plugins
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <SwApplication.h>
#include <SwMacros.h>
#include <QMessageBox>
#include "_SwGuiCompPluginsTree.h"
#include "_SwGuiPluginsFrontEnd.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


#define CL_WIDGET_INTERFACE_NAME "Widget_%1"


/*! \brief Constructeur */
_SwGuiCompPluginsTree::_SwGuiCompPluginsTree(): SwComponent_Class(){
    _provider_service=NULL;
	_window_title = "Plugins List";
}
/*! \brief Destructeur */
_SwGuiCompPluginsTree::~_SwGuiCompPluginsTree(){
    //Desenregistrement des services
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _provider_service;
    delete _properties_service;
    delete _action;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompPluginsTree::InitializeResources() throw(SwException) {
    //Creation des service
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    //Creation de l'interface principale
    _action=new QAction(QIcon(":/SwGui/plugins.png"),QString("Plugins list"),NULL);
    connect(_action, SIGNAL(triggered()), this, SLOT(showplugins()));
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_provider_service);

	//Gestion du window title
    _window_title_property=_properties_service->CreateProperty<QString>("WindowTitle");
    if (_window_title_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register WindowTitle property\n"));
    }
    _window_title_property->SetDescription("Define window title");  
    _window_title_property->SetValue(QVariant(_window_title));
    _window_title_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompPluginsTree::OnPropertyChange);
	
    //Exportation de l'interface QWidget
    _provider_service->RegisterProvidedInterface<ISwAction>("Action",(ISwAction *)this);

    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiPluginsTree done\n"));

}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompPluginsTree::OnPropertyChange(ISwProperty * property) {

    if (_window_title_property==property) {
        _window_title=property->GetValue().toString();		
    }
}
//---------------------------------------------------------------------
// Interface ISwMainWindow
//---------------------------------------------------------------------
/*! \brief Renvoie le Action
\return le Action */
QAction & _SwGuiCompPluginsTree::GetAction() {
    return *_action;
}
/*! \brief Affiche les plugins */
void _SwGuiCompPluginsTree::showplugins() {
    _SwGuiPluginsFrontEnd * dialog=new _SwGuiPluginsFrontEnd(NULL);
	dialog->setWindowTitle(_window_title);
    dialog->exec();
}

