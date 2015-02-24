/*!
 \file _SwGuiMainWindow.cpp
 \brief Implementation of the Class _SwGuiMainWindow generant une QMainWindow
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QDesktopWidget>
#include <QApplication>
#include <QMenuBar>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompQMainWindow.h"
#include <QEvent>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


#define CL_MENU_INTERFACE_NAME "Menu_%1"
#define CL_ACTION_INTERFACE_NAME "Action_%1"
#define CL_TOOLBAR_INTERFACE_NAME "ToolBar_%1"
#define CL_DOCKWIDGET_INTERFACE_NAME "DockWidget_%1"
#define CL_CENTRALWIDGET_INTERFACE_NAME "CentralWidget"
#define SW_SHOW_NORMAL 0
#define SW_SHOW_CENTERED 1
#define SW_SHOW_FULLSCREEN 2
#define SW_SHOW_MAXIMIZED 3
#define SW_SHOW_MINIMIZED 4


/*! \brief Constructeur */
_SwGuiCompQMainWindow::_SwGuiCompQMainWindow(): Component(){
    //_main_window=NULL;
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
    _show_mode.AddKey(SW_SHOW_NORMAL,"None");
    _show_mode.AddKey(SW_SHOW_CENTERED,"Centered");
    _show_mode.AddKey(SW_SHOW_FULLSCREEN,"FullScreen");
    _show_mode.AddKey(SW_SHOW_MAXIMIZED,"Maximized");
    _show_mode.AddKey(SW_SHOW_MINIMIZED,"Minimized");
    _show_mode.FromInt(SW_SHOW_NORMAL);
    _useAsWidget = false;
}
/*! \brief Destructeur */
_SwGuiCompQMainWindow::~_SwGuiCompQMainWindow(){
    QMap<QString,ISwMenu *>::iterator menu_it;
    QMap<QString,ISwAction *>::iterator action_it;
    QMap<QString,ISwToolBar *>::iterator toolbar_it;
    QMap<QString,ISwQDockWidget *>::iterator dockwidget_it;
    
    //Si c'est un menu
    menu_it=_menus.begin();
    while (menu_it!=_menus.end()) {
        if (menu_it.value()!=NULL) {
            //Et qu'il etait defini, on le detache de la menubar
            menu_it.value()->GetMenu().setParent(NULL);   
            menu_it.value()=NULL;
        }
        menu_it++;
    } 
    //Si c'est une action
    action_it=_actions.begin();
    while (action_it!=_actions.end()) {
        if (action_it.value()!=NULL) {
            //Et qu'ellle etait definie, on la detache de la menubar
            action_it.value()->GetAction().setParent(NULL);    
            action_it.value()=NULL;
        }
        action_it++;
    } 
    //Si c'est une toolbar
    toolbar_it=_toolbars.begin();
    while (toolbar_it!=_toolbars.end()) {
        if (toolbar_it.value()!=NULL) {
            //Et qu'ellle etait definie, on la detache de la main window
            toolbar_it.value()->GetToolBar().setParent(NULL);    
            toolbar_it.value()=NULL;
       }
       toolbar_it++;
    } 
    //Si c'est un dockwidget
    dockwidget_it=_dockwidgets.begin();
    if (dockwidget_it!=_dockwidgets.end()) {
        if (dockwidget_it.value()!=NULL) {
            //Et qu'ellle etait definie, on la detache de la main window
            dockwidget_it.value()->GetDockWidget().setParent(NULL);    
            dockwidget_it.value()=NULL;
        }
        dockwidget_it++;
    } 
    //Si c'est le widget central
    if (_handle_central_widget!=NULL) {
        //Et qu'il est defini, on le detache du parent
        _handle_central_widget->GetWidget().setParent(NULL);
    }


    //delete _main_window;
    //_main_window=0;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompQMainWindow::initializeComponent() throw(SwException) {
    //Creation de l'interface principale
    //_main_window=new QMainWindow();


    if (!_useAsWidget)
    {
      //Exportation de l'interface ISwQMainWindow
      getIProviderService().RegisterProvidedInterface<ISwQMainWindow>("MainWindow",(ISwQMainWindow *)this);
    }
    else
    {
      //Exportation de l'interface ISwWidget
      getIProviderService().RegisterProvidedInterface<ISwWidget>("MainWindowAsWidget",(ISwWidget *)this);
    }
    //Importation de l'interface ISwWidget
    getIConsumerService().RegisterConsumedInterface<ISwWidget>(CL_CENTRALWIDGET_INTERFACE_NAME,&_handle_central_widget);

	//Exportation de l'interface ISwWidget
	getIProviderService().RegisterProvidedInterface<ISwEvent>("ISwEvent",(ISwEvent*)this);

    //Enregistrement des propriétés
	//getPropertiesService().CreatePropertiesForQObject(_main_window,"QMainWindow");
	getPropertiesService().CreatePropertiesForQObject((QMainWindow*)this,"QMainWindow");
    
    //Gestion des menus
    _menus_nb_property=getPropertiesService().CreateProperty<uint>("nb_menus");
    if (_menus_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_menus property\n"));
    } 
    _menus_nb_property->SetDescription("Define how many ISwMenu interfaces this component accept");  
    _menus_nb_property->SetValue(QVariant(_menus_nb));
    enableListeningChangeForProperty(_menus_nb_property);
    
    //Gestion des actions
    _actions_nb_property=getPropertiesService().CreateProperty<uint>("nb_actions");
    if (_actions_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_actions property\n"));
    }
    _actions_nb_property->SetDescription("Define how many ISwAction interfaces this component accept");  
    _actions_nb_property->SetValue(QVariant(_actions_nb));
    enableListeningChangeForProperty(_actions_nb_property);

    //Gestion des toolbars
    _toolbars_nb_property=getPropertiesService().CreateProperty<uint>("nb_toolbars");
    if (_toolbars_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_toolbars property\n"));
    }
    _toolbars_nb_property->SetDescription("Define how many ISwToolBar interfaces this component accept");  
    _toolbars_nb_property->SetValue(QVariant(_toolbars_nb));
    enableListeningChangeForProperty(_toolbars_nb_property);

    //Gestion des dockwidgets
    _dockwidgets_nb_property=getPropertiesService().CreateProperty<uint>("nb_dockwidgets");
    if (_dockwidgets_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_dockwidgets property\n"));
    }
    _dockwidgets_nb_property->SetDescription("Define how many ISwQDockWidget interfaces this component accept");  
    _dockwidgets_nb_property->SetValue(QVariant(_dockwidgets_nb));
    enableListeningChangeForProperty(_dockwidgets_nb_property);

    // choix de l'interface externe
    _use_aswidget_property   =getPropertiesService().CreateProperty<bool>("use_aswidget");    
    if (_use_aswidget_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register _use_aswidget_property property\n"));
    }
    _use_aswidget_property->SetDescription("Define kind of widget interface this component produces");  
    _use_aswidget_property->SetValue(QVariant(_useAsWidget));
    enableListeningChangeForProperty(_use_aswidget_property);

    _show_property=getPropertiesService().CreateProperty<SwEnum>("ShowMode");
    if (_show_property!=NULL) {
        QVariant variant;
        variant.setValue(_show_mode);
        _show_property->SetValue(variant);
        _show_property->SetDescription("Show Mode");  
        enableListeningChangeForProperty(_show_property);
    }

    //Fin
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiMainWindow done\n"));

}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompQMainWindow::eventPropertyChange(ISwProperty * property) {
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
           getIProviderService().UnregisterProvidedInterface("MainWindow");
           //Exportation de l'interface ISwWidget
           getIProviderService().RegisterProvidedInterface<ISwWidget>("MainWindowAsWidget",(ISwWidget *)this);
       }
       else
       {
        if (_useAsWidget && !boolval)
        {
            getIProviderService().UnregisterProvidedInterface("MainWindowAsWidget");
            //Exportation de l'interface ISwQMainWindow
            getIProviderService().RegisterProvidedInterface<ISwQMainWindow>("MainWindow",(ISwQMainWindow *)this);
        }
       }
    }
    if (_menus_nb_property==property) {
        val=property->GetValue().toUInt();
        if (val==_menus_nb) return;
        if (val<_menus_nb) {
            for (uint i=val;i<_menus_nb;i++) {
                interface_name=QString(CL_MENU_INTERFACE_NAME).arg(i);
                getIConsumerService().UnregisterConsumedInterface(interface_name);
            }
        } else {
            for (uint i=_menus_nb;i<val;i++) {
                interface_name=QString(CL_MENU_INTERFACE_NAME).arg(i);
                _menus.insert(interface_name,(ISwMenu *)NULL);
                getIConsumerService().RegisterConsumedInterface<ISwMenu>(interface_name,&_tmp_handle_menu);
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
                getIConsumerService().UnregisterConsumedInterface(interface_name);
            }
        } else {
            for (uint i=_actions_nb;i<val;i++) {
                interface_name=QString(CL_ACTION_INTERFACE_NAME).arg(i);
                _actions.insert(interface_name,(ISwAction *)NULL);
                getIConsumerService().RegisterConsumedInterface<ISwAction>(interface_name,&_tmp_handle_action);
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
                getIConsumerService().UnregisterConsumedInterface(interface_name);
                property_name=interface_name+".where";
                _toolbar_positions.erase(_toolbar_positions.find(interface_name));
                getPropertiesService().DestroyProperty(property_name.toLatin1().constData());
            }
        } else {
            variant.setValue(_default_toolbar_position);
            for (uint i=_toolbars_nb;i<val;i++) {
                interface_name=QString(CL_TOOLBAR_INTERFACE_NAME).arg(i);
                _toolbars.insert(interface_name,(ISwToolBar *)NULL);
                getIConsumerService().RegisterConsumedInterface<ISwToolBar>(interface_name,&_tmp_handle_toolbar);
                //Ajout d'une propriete pour le placement
                property_name=interface_name+".where";
                toolbar_position=getPropertiesService().CreateProperty<SwEnum>(property_name.toLatin1().constData());
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
                getIConsumerService().UnregisterConsumedInterface(interface_name);
                property_name=interface_name+".where";
                _dockwidget_positions.erase(_dockwidget_positions.find(interface_name));
                getPropertiesService().DestroyProperty(property_name.toLatin1().constData());
            }
        } else {
            variant.setValue(_default_dockwidget_position);
            for (uint i=_dockwidgets_nb;i<val;i++) {
                interface_name=QString(CL_DOCKWIDGET_INTERFACE_NAME).arg(i);
                _dockwidgets.insert(interface_name,(ISwQDockWidget *)NULL);
                getIConsumerService().RegisterConsumedInterface<ISwQDockWidget>(interface_name,&_tmp_handle_dockwidget);
                //Ajout d'une propriete pour le placement
                property_name=interface_name+".where";
                dockwidget_position=getPropertiesService().CreateProperty<SwEnum>(property_name.toLatin1().constData());
                if (dockwidget_position!=NULL) {
                    dockwidget_position->SetValue(variant);
                    _dockwidget_positions.insert(interface_name,dockwidget_position);
                    dockwidget_position->SetDescription("Define where is attached DockWidget");  
                }
            }
        }
        _dockwidgets_nb=val;
    }
    if (_show_property==property) {
        SwEnum showmode=_show_property->GetValue().value<SwEnum>();
        _show_mode=showmode;
        showChanged();
    }
}

//---------------------------------------------------------------------
// Interface ISwQMainWindow
//---------------------------------------------------------------------
/*! \brief Renvoie le nom du service
\return le nom du service */
QMainWindow & _SwGuiCompQMainWindow::GetMainWindow() {
    //return *_main_window;
	return *this;
}
  /*! \brief Renvoie le widget
\return le widget */
QWidget & _SwGuiCompQMainWindow::GetWidget(){
    //return *(dynamic_cast<QWidget *> (_main_window));
	return *(dynamic_cast<QWidget *> (this));
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompQMainWindow::eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,ISwMenu *>::iterator menu_it;
    QMap<QString,ISwAction *>::iterator action_it;
    QMap<QString,ISwToolBar *>::iterator toolbar_it;
    QMap<QString,ISwQDockWidget *>::iterator dockwidget_it;
    
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
            //Et qu'elle etait definie, on la detache de la menubar
            action_it.value()->GetAction().setParent(NULL);    
            action_it.value()=NULL;
        }
        return;
    } 
    //Si c'est une toolbar
    toolbar_it=_toolbars.find(interface_name);
    if (toolbar_it!=_toolbars.end()) {
        if (toolbar_it.value()!=NULL) {
            //Et qu'elle etait definie, on la detache de la main window
            toolbar_it.value()->GetToolBar().setParent(NULL);    
            toolbar_it.value()=NULL;
       }
        return;
    } 
    //Si c'est un dockwidget
    dockwidget_it=_dockwidgets.find(interface_name);
    if (dockwidget_it!=_dockwidgets.end()) {
        if (dockwidget_it.value()!=NULL) {
            //Et qu'elle etait definie, on la detache de la main window
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
void _SwGuiCompQMainWindow::eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host) {
	QMap<QString,ISwMenu *>::iterator menu_it;
	QMap<QString,ISwAction *>::iterator action_it;
	QMap<QString,ISwToolBar *>::iterator toolbar_it;
	QMap<QString,ISwProperty *>::iterator toolbar_position_it;
	QMap<QString,ISwQDockWidget *>::iterator dockwidget_it;
	QMap<QString,ISwProperty *>::iterator dockwidget_position_it;

	//Si c'est un menu
	menu_it=_menus.find(interface_name);
	if (menu_it!=_menus.end()) {
		if (menu_it.value()==NULL && _tmp_handle_menu!=NULL) {
			//Et qu'il etait non defini, on l'enregistre et l'attache a la menubar
			menu_it.value()=_tmp_handle_menu;  
			//_main_window->menuBar()->addMenu(&(_tmp_handle_menu->GetMenu()));
			this->menuBar()->addMenu(&(_tmp_handle_menu->GetMenu()));
		}
		return;
	} 
	//Si c'est une action
	action_it=_actions.find(interface_name);
	if (action_it!=_actions.end()) {
		if (action_it.value()==NULL && _tmp_handle_action!=NULL) {
			//Et qu'ellle etait non definie,on l'enregistre et l'attache a la menubar
			action_it.value()=_tmp_handle_action;
			//_main_window->menuBar()->addAction(&(_tmp_handle_action->GetAction()));
			this->menuBar()->addAction(&(_tmp_handle_action->GetAction()));
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
			//_main_window->addToolBar((Qt::ToolBarArea)enum_value.ToInt(),&(_tmp_handle_toolbar->GetToolBar()));
			this->addToolBar((Qt::ToolBarArea)enum_value.ToInt(),&(_tmp_handle_toolbar->GetToolBar()));
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
			//if (_main_window->dockOptions().testFlag(QMainWindow::ForceTabbedDocks) |
			//	_main_window->dockOptions().testFlag(QMainWindow::AllowTabbedDocks)) {
			if (this->dockOptions().testFlag(QMainWindow::ForceTabbedDocks) |
				this->dockOptions().testFlag(QMainWindow::AllowTabbedDocks)) {
					QDockWidget *tabbedWidget=0;
					//QObjectList list=_main_window->children();
					QObjectList list=((QMainWindow*)this)->children();
					for(int i=0;i<list.count() && tabbedWidget==0;i++) {
						QDockWidget *aDockWidget=dynamic_cast<QDockWidget *>(list[i]);
						//if (aDockWidget!=0 &&  _main_window->dockWidgetArea(aDockWidget)==(Qt::DockWidgetArea)enum_value.ToInt()) {
						if (aDockWidget!=0 &&  this->dockWidgetArea(aDockWidget)==(Qt::DockWidgetArea)enum_value.ToInt()) {
							tabbedWidget=aDockWidget;    
						}
					}
					//_main_window->addDockWidget((Qt::DockWidgetArea)enum_value.ToInt(),&(_tmp_handle_dockwidget->GetDockWidget()));          
					this->addDockWidget((Qt::DockWidgetArea)enum_value.ToInt(),&(_tmp_handle_dockwidget->GetDockWidget()));          
					if (tabbedWidget!=0) {
						//_main_window->tabifyDockWidget(tabbedWidget,&(_tmp_handle_dockwidget->GetDockWidget()));
						this->tabifyDockWidget(tabbedWidget,&(_tmp_handle_dockwidget->GetDockWidget()));
					}
			} else {
				//_main_window->addDockWidget((Qt::DockWidgetArea)enum_value.ToInt(),&(_tmp_handle_dockwidget->GetDockWidget()));          
				this->addDockWidget((Qt::DockWidgetArea)enum_value.ToInt(),&(_tmp_handle_dockwidget->GetDockWidget()));          
			}

		}
		return;
	} 
	//Si c'est le widget central
	if (interface_name==CL_CENTRALWIDGET_INTERFACE_NAME && _handle_central_widget!=NULL) {
		//Et qu'il est defini, on l'attache a la mainwindow
		//_main_window->setCentralWidget(&(_handle_central_widget->GetWidget()));
		this->setCentralWidget(&(_handle_central_widget->GetWidget()));
		return;
	}

}
void _SwGuiCompQMainWindow::showChanged() {
	switch(_show_mode.ToInt()) {
		case SW_SHOW_CENTERED: {
			//_main_window->showNormal();
			this->showNormal();
			QDesktopWidget *desktop = QApplication::desktop();
			QRect screensize= desktop->screenGeometry(desktop->primaryScreen());
			//QRect windowSize= _main_window->frameGeometry();
			QRect windowSize= this->frameGeometry();
			QPoint slefttop(screensize.left(),screensize.top());
			QPoint wlefttop;
			slefttop.setX(slefttop.x()+(screensize.width()-windowSize.width())/2);
			slefttop.setY(slefttop.y()+(screensize.height()-windowSize.height())/2);
			//_main_window->move(slefttop);
			this->move(slefttop);

							}
							break;
		case SW_SHOW_FULLSCREEN:
			//_main_window->showFullScreen();
			this->showFullScreen();
			break;
		case SW_SHOW_MAXIMIZED:
			//_main_window->showMaximized();
			this->showMaximized();
			break;
		case SW_SHOW_MINIMIZED:
			//_main_window->showMinimized();
			this->showMinimized();
			break;
		case SW_SHOW_NORMAL:
		default:
			break;
	}
}

//---------------------------------------------------------------------
// Interface ISwEventObservable
//---------------------------------------------------------------------
void _SwGuiCompQMainWindow::addObserver(ISwEventObserver * obs)
{
	if(obs!=NULL)
		_iSwEvent.push_back(obs);
}
void _SwGuiCompQMainWindow::removeObserver(ISwEventObserver * obs)
{
	if(obs!=NULL)
	{
		if(_iSwEvent.contains(obs))
			_iSwEvent.removeAt(_iSwEvent.indexOf(obs));
	}	
}
void _SwGuiCompQMainWindow::notify(QEvent * event)
{
	ISwEventObserver* itObs = NULL;
	foreach(itObs, _iSwEvent)
	{
		itObs->onEvent(event);
	}
}

void _SwGuiCompQMainWindow::closeEvent(QCloseEvent* event)
{
	notify((QEvent*)event);
}


