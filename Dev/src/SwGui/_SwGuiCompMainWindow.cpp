/*!
 \file _SwGuiMainWindow.cpp
 \brief Implementation of the Class _SwGuiMainWindow generant une QMainWindow
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QMenuBar>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompMainWindow.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


#define CL_MENU_INTERFACE_NAME "Menu_%1"
#define CL_ACTION_INTERFACE_NAME "Action_%1"
#define CL_TOOLBAR_INTERFACE_NAME "ToolBar_%1"
#define CL_DOCKWIDGET_INTERFACE_NAME "DockWidget_%1"
#define CL_CENTRALWIDGET_INTERFACE_NAME "CentralWidget"


/*! \brief Constructeur */
_SwGuiCompMainWindow::_SwGuiCompMainWindow(): SwComponent_Class(){
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _main_window=NULL;
    _use_aswidget_property = 0;
    _menus_nb=0;
    _actions_nb=0;
    _toolbars_nb=0;
    _dockwidgets_nb=0;
    _tmp_handle_menu=NULL;
    _tmp_handle_action=NULL;
    _tmp_handle_toolbar=NULL;
    _tmp_handle_dockwidget=NULL;
    _handle_central_widget=NULL;
    _default_toolbar_position.AddKey(Qt::LeftToolBarArea,"Left");
    _default_toolbar_position.AddKey(Qt::RightToolBarArea,"Right");
    _default_toolbar_position.AddKey(Qt::TopToolBarArea,"Top");
    _default_toolbar_position.AddKey(Qt::BottomToolBarArea,"Bottom");
    _default_toolbar_position.FromInt(Qt::TopToolBarArea);
    _default_dockwidget_position.AddKey(Qt::LeftDockWidgetArea,"Left");
    _default_dockwidget_position.AddKey(Qt::RightDockWidgetArea,"Right");
    _default_dockwidget_position.AddKey(Qt::TopDockWidgetArea,"Top");
    _default_dockwidget_position.AddKey(Qt::BottomDockWidgetArea,"Bottom");
    _default_dockwidget_position.FromInt(Qt::LeftDockWidgetArea);
    _useAsWidget = false;
}
/*! \brief Destructeur */
_SwGuiCompMainWindow::~_SwGuiCompMainWindow(){
    //Desenregistrement des services
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _provider_service;
    delete _consumer_service;
    delete _properties_service;
    delete _main_window;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompMainWindow::InitializeResources() throw(SwException) {
    //Creation des service
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _consumer_service=new SwInterfaces_Consumer_Class(this);
    _properties_service=new SwProperties_Class(this);
    //Creation de l'interface principale
    _main_window=new QMainWindow();
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    if (!_useAsWidget)
    {
      //Exportation de l'interface ISwMainWindow
      _provider_service->RegisterProvidedInterface<ISwMainWindow>("MainWindow",(ISwMainWindow *)this);
    }
    else
    {
      //Exportation de l'interface ISwWidget
      _provider_service->RegisterProvidedInterface<ISwWidget>("MainWindowAsWidget",(ISwWidget *)this);
    }
    //Importation de l'interface ISwWidget
    _consumer_service->RegisterConsumedInterface<ISwWidget>(CL_CENTRALWIDGET_INTERFACE_NAME,&_handle_central_widget);

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

    //Enregistrement des propriétés
    _properties_service->CreatePropertiesForQObject(_main_window,"QMainWindow");
    
    //Gestion des menus
    _menus_nb_property=_properties_service->CreateProperty<uint>("nb_menus");
    if (_menus_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_menus property\n"));
    } 
    _menus_nb_property->SetDescription("Define how many ISwMenu interfaces this component accept");  
    _menus_nb_property->SetValue(QVariant(_menus_nb));
    _menus_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompMainWindow::OnPropertyChange);
    
    //Gestion des actions
    _actions_nb_property=_properties_service->CreateProperty<uint>("nb_actions");
    if (_actions_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_actions property\n"));
    }
    _actions_nb_property->SetDescription("Define how many ISwAction interfaces this component accept");  
    _actions_nb_property->SetValue(QVariant(_actions_nb));
    _actions_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompMainWindow::OnPropertyChange);
    //Gestion des toolbars
    _toolbars_nb_property=_properties_service->CreateProperty<uint>("nb_toolbars");
    if (_toolbars_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_toolbars property\n"));
    }
    _toolbars_nb_property->SetDescription("Define how many ISwToolBar interfaces this component accept");  
    _toolbars_nb_property->SetValue(QVariant(_toolbars_nb));
    _toolbars_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompMainWindow::OnPropertyChange);
    
    //Gestion des dockwidgets
    _dockwidgets_nb_property=_properties_service->CreateProperty<uint>("nb_dockwidgets");
    if (_dockwidgets_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_dockwidgets property\n"));
    }
    _dockwidgets_nb_property->SetDescription("Define how many ISwDockWidget interfaces this component accept");  
    _dockwidgets_nb_property->SetValue(QVariant(_dockwidgets_nb));
    _dockwidgets_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompMainWindow::OnPropertyChange);

    // choix de l'interface externe
    _use_aswidget_property   =_properties_service->CreateProperty<bool>("use_aswidget");    
    if (_use_aswidget_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register _use_aswidget_property property\n"));
    }
    _use_aswidget_property->SetDescription("Define kind of widget interface this component produces");  
    _use_aswidget_property->SetValue(QVariant(_useAsWidget));
    _use_aswidget_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompMainWindow::OnPropertyChange);
    
    //Fin
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiMainWindow done\n"));

}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompMainWindow::OnPropertyChange(ISwProperty * property) {
    uint val;
    QString interface_name;
    QString property_name;
    ISwProperty * toolbar_position;
    ISwProperty * dockwidget_position;
    QVariant variant;
    if (_use_aswidget_property==property) 
    {
       bool boolval=property->GetValue().toBool();    
       if (!_useAsWidget && boolval)
       {
           _provider_service->UnregisterProvidedInterface("MainWindow");
           //Exportation de l'interface ISwWidget
           _provider_service->RegisterProvidedInterface<ISwWidget>("MainWindowAsWidget",(ISwWidget *)this);
       }
       else
       {
        if (_useAsWidget && !boolval)
        {
            _provider_service->UnregisterProvidedInterface("MainWindowAsWidget");
            //Exportation de l'interface ISwMainWindow
            _provider_service->RegisterProvidedInterface<ISwMainWindow>("MainWindow",(ISwMainWindow *)this);
        }
       }
    }
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
    if (_toolbars_nb_property==property) {
        val=property->GetValue().toUInt();
        if (val==_toolbars_nb) return;
        if (val<_toolbars_nb) {
            for (uint i=val;i<_toolbars_nb;i++) {
                interface_name=QString(CL_TOOLBAR_INTERFACE_NAME).arg(i);
                _consumer_service->UnregisterConsumedInterface(interface_name);
                property_name=interface_name+".where";
                _toolbar_positions.erase(_toolbar_positions.find(interface_name));
                _properties_service->DestroyProperty(property_name.toLatin1().constData());
            }
        } else {
            variant.setValue(_default_toolbar_position);
            for (uint i=_toolbars_nb;i<val;i++) {
                interface_name=QString(CL_TOOLBAR_INTERFACE_NAME).arg(i);
                _toolbars.insert(interface_name,(ISwToolBar *)NULL);
                _consumer_service->RegisterConsumedInterface<ISwToolBar>(interface_name,&_tmp_handle_toolbar);
                //Ajout d'une propriete pour le placement
                property_name=interface_name+".where";
                toolbar_position=_properties_service->CreateProperty<SwEnum>(property_name.toLatin1().constData());
                if (toolbar_position!=NULL) {
                    toolbar_position->SetValue(variant);
                    _toolbar_positions.insert(interface_name,toolbar_position);
                    toolbar_position->SetDescription("Define where is attached Toolbar");  

                }
            }
        }
        _toolbars_nb=val;
    }
    if (_dockwidgets_nb_property==property) {
        val=property->GetValue().toUInt();
        if (val==_dockwidgets_nb) return;
        if (val<_dockwidgets_nb) {
            for (uint i=val;i<_dockwidgets_nb;i++) {
                interface_name=QString(CL_DOCKWIDGET_INTERFACE_NAME).arg(i);
                _consumer_service->UnregisterConsumedInterface(interface_name);
                property_name=interface_name+".where";
                _dockwidget_positions.erase(_dockwidget_positions.find(interface_name));
                _properties_service->DestroyProperty(property_name.toLatin1().constData());
            }
        } else {
            variant.setValue(_default_dockwidget_position);
            for (uint i=_dockwidgets_nb;i<val;i++) {
                interface_name=QString(CL_DOCKWIDGET_INTERFACE_NAME).arg(i);
                _dockwidgets.insert(interface_name,(ISwDockWidget *)NULL);
                _consumer_service->RegisterConsumedInterface<ISwDockWidget>(interface_name,&_tmp_handle_dockwidget);
                //Ajout d'une propriete pour le placement
                property_name=interface_name+".where";
                dockwidget_position=_properties_service->CreateProperty<SwEnum>(property_name.toLatin1().constData());
                if (dockwidget_position!=NULL) {
                    dockwidget_position->SetValue(variant);
                    _dockwidget_positions.insert(interface_name,dockwidget_position);
                    dockwidget_position->SetDescription("Define where is attached DockWidget");  
                }
            }
        }
        _dockwidgets_nb=val;
    }
}

//---------------------------------------------------------------------
// Interface ISwMainWindow
//---------------------------------------------------------------------
/*! \brief Renvoie le nom du service
\return le nom du service */
QMainWindow & _SwGuiCompMainWindow::GetMainWindow() {
    return *_main_window;
}
  /*! \brief Renvoie le widget
\return le widget */
QWidget & _SwGuiCompMainWindow::GetWidget(){
    return *(dynamic_cast<QWidget *> (_main_window));
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompMainWindow::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,ISwMenu *>::iterator menu_it;
    QMap<QString,ISwAction *>::iterator action_it;
    QMap<QString,ISwToolBar *>::iterator toolbar_it;
    QMap<QString,ISwDockWidget *>::iterator dockwidget_it;
    
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
            action_it.value()->GetAction().setParent(NULL);    
            action_it.value()=NULL;
        }
        return;
    } 
    //Si c'est une toolbar
    toolbar_it=_toolbars.find(interface_name);
    if (toolbar_it!=_toolbars.end()) {
        if (toolbar_it.value()!=NULL) {
            //Et qu'ellle etait definie, on la detache de la main window
            toolbar_it.value()->GetToolBar().setParent(NULL);    
            toolbar_it.value()=NULL;
       }
        return;
    } 
    //Si c'est un dockwidget
    dockwidget_it=_dockwidgets.find(interface_name);
    if (dockwidget_it!=_dockwidgets.end()) {
        if (dockwidget_it.value()!=NULL) {
            //Et qu'ellle etait definie, on la detache de la main window
            dockwidget_it.value()->GetDockWidget().setParent(NULL);    
            dockwidget_it.value()=NULL;
        }
        return;
    } 
    //Si c'est le widget central
    if (interface_name==CL_CENTRALWIDGET_INTERFACE_NAME && _handle_central_widget!=NULL) {
        //Et qu'il est defini, on le detache du parent
        _handle_central_widget->GetWidget().setParent(NULL);
        return;
    }

}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompMainWindow::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,ISwMenu *>::iterator menu_it;
    QMap<QString,ISwAction *>::iterator action_it;
    QMap<QString,ISwToolBar *>::iterator toolbar_it;
    QMap<QString,ISwProperty *>::iterator toolbar_position_it;
    QMap<QString,ISwDockWidget *>::iterator dockwidget_it;
    QMap<QString,ISwProperty *>::iterator dockwidget_position_it;
    
    //Si c'est un menu
    menu_it=_menus.find(interface_name);
    if (menu_it!=_menus.end()) {
        if (menu_it.value()==NULL && _tmp_handle_menu!=NULL) {
            //Et qu'il etait non defini, on l'enregistre et l'attache a la menubar
            menu_it.value()=_tmp_handle_menu;  
            _main_window->menuBar()->addMenu(&(_tmp_handle_menu->GetMenu()));
        }
        return;
    } 
    //Si c'est une action
    action_it=_actions.find(interface_name);
    if (action_it!=_actions.end()) {
        if (action_it.value()==NULL && _tmp_handle_action!=NULL) {
            //Et qu'ellle etait non definie,on l'enregistre et l'attache a la menubar
            action_it.value()=_tmp_handle_action;
            _main_window->menuBar()->addAction(&(_tmp_handle_action->GetAction()));
        }
        return;
    } 
    //Si c'est une toolbar
    toolbar_it=_toolbars.find(interface_name);
    toolbar_position_it=_toolbar_positions.find(interface_name);
    if (toolbar_it!=_toolbars.end() && toolbar_position_it!=_toolbar_positions.end()) {
        if (toolbar_it.value()==NULL && _tmp_handle_toolbar!=NULL) {
            //Et qu'ellle etait non definie,on l'enregistre et l'attache a la mainwindow
            SwEnum enum_value=toolbar_position_it.value()->GetValue().value<SwEnum>();
            toolbar_it.value()=_tmp_handle_toolbar;
            _main_window->addToolBar((Qt::ToolBarArea)enum_value.ToInt(),&(_tmp_handle_toolbar->GetToolBar()));
        }
        return;
    } 
    //Si c'est un dockwidget
    dockwidget_it=_dockwidgets.find(interface_name);
    dockwidget_position_it=_dockwidget_positions.find(interface_name);
    if (dockwidget_it!=_dockwidgets.end() && dockwidget_position_it!=_dockwidget_positions.end()) {

        if (dockwidget_it.value()==NULL  && _tmp_handle_dockwidget!=NULL) {
            //Et qu'ellle etait non definie,on l'enregistre et l'attache a la mainwindow
            SwEnum enum_value=dockwidget_position_it.value()->GetValue().value<SwEnum>();
            dockwidget_it.value()=_tmp_handle_dockwidget;
            //Cas du tabbed mode
            if (_main_window->dockOptions().testFlag(QMainWindow::ForceTabbedDocks) |
                _main_window->dockOptions().testFlag(QMainWindow::AllowTabbedDocks)) {
                QDockWidget *tabbedWidget=0;
                QObjectList list=_main_window->children();
                for(int i=0;i<list.count() && tabbedWidget==0;i++) {
                    QDockWidget *aDockWidget=dynamic_cast<QDockWidget *>(list[i]);
                    if (aDockWidget!=0 &&  _main_window->dockWidgetArea(aDockWidget)==(Qt::DockWidgetArea)enum_value.ToInt()) {
                        tabbedWidget=aDockWidget;    
                    }
                }
                _main_window->addDockWidget((Qt::DockWidgetArea)enum_value.ToInt(),&(_tmp_handle_dockwidget->GetDockWidget()));          
                if (tabbedWidget!=0) {
                    _main_window->tabifyDockWidget(tabbedWidget,&(_tmp_handle_dockwidget->GetDockWidget()));
                }
            } else {
                _main_window->addDockWidget((Qt::DockWidgetArea)enum_value.ToInt(),&(_tmp_handle_dockwidget->GetDockWidget()));          
            }
            
        }
       return;
    } 
    //Si c'est le widget central
    if (interface_name==CL_CENTRALWIDGET_INTERFACE_NAME && _handle_central_widget!=NULL) {
        //Et qu'il est defini, on l'attache a la mainwindow
        _main_window->setCentralWidget(&(_handle_central_widget->GetWidget()));
        return;
    }

}
