/*!
 \file _SwGuiMainWindow.cpp
 \brief Implementation of the Class _SwGuiMainWindow generant une SwDockWidget_MainWindow
 \version 1.0
 \date
 \author
*/
#include <QDesktopWidget>
#include <QApplication>
#include <QMenuBar>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompMainWindow.h"
#include <QEvent>

#include <io.h>
#include <direct.h>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

#define STATUSBAR_INTERFACE "StatusBar"
#define CL_MENU_INTERFACE_NAME "Menu_%1"
#define CL_ACTION_INTERFACE_NAME "Action_%1"
#define CL_TOOLBAR_INTERFACE_NAME "ToolBar_%1"
#define CL_WIDGET_INTERFACE_NAME "Widget_%1"
#define CL_CENTRALWIDGET_INTERFACE_NAME "CentralWidget"
#define CL_LISTDOCKWIDGET_INTERFACE_NAME "ListDockWidget_%1"

#define SW_SHOW_NORMAL 0
#define SW_SHOW_CENTERED 1
#define SW_SHOW_FULLSCREEN 2
#define SW_SHOW_MAXIMIZED 3
#define SW_SHOW_MINIMIZED 4

// constantes pour la gestion des paths des fichiers XML sur le disque
QString HOME_PATH; 
QString APP_PATH;	


//-----------------------------------------------------------------------------
/*! \brief Constructeur */
_SwGuiCompMainWindow::_SwGuiCompMainWindow(): SwAssistedComponent()
{
	setConsumerServiceAvaibility(true);
	setProviderServiceAvaibility(true);
	setPropertyServiceAvaibility(true);
	setOwnerServiceAvaibility(true);

	_relativePath = "Undefined Value - will not be saved";
	_absolutePath = "Undefined Value - will not be saved";
	_pathType = UserHomeDir;
	
    _use_aswidget_property = 0;
    _menus_nb = 0;
    _actions_nb = 0;
    _toolbars_nb = 0;
    _dockwidgets_nb = 0;
	_listdockwidgets_nb = 0;
	_saveAutoPeriod = 0;

	_mainWindow = NULL;

	_default_toolbar_position.AddKey(Qt::LeftToolBarArea,"Left");
    _default_toolbar_position.AddKey(Qt::RightToolBarArea,"Right");
    _default_toolbar_position.AddKey(Qt::TopToolBarArea,"Top");
    _default_toolbar_position.AddKey(Qt::BottomToolBarArea,"Bottom");
    _default_toolbar_position.FromInt(Qt::TopToolBarArea);

    _show_mode.AddKey(SW_SHOW_NORMAL,"None");
    _show_mode.AddKey(SW_SHOW_CENTERED,"Centered");
    _show_mode.AddKey(SW_SHOW_FULLSCREEN,"FullScreen");
    _show_mode.AddKey(SW_SHOW_MAXIMIZED,"Maximized");
    _show_mode.AddKey(SW_SHOW_MINIMIZED,"Minimized");
    _show_mode.FromInt(SW_SHOW_NORMAL);

    _useAsWidget = false;
	_finalized = false;

	HOME_PATH = QDir::homePath () + QDir::separator(); 
	APP_PATH	= SwApplication::GetInstance()->GetApplicationDirPath() + QDir::separator();

	_relativePath = "AppData\\Roaming\\diginext\\Starlinx\\Configuration\\dockParameters.xml";
}

//-----------------------------------------------------------------------------
/*! \brief Destructeur */
_SwGuiCompMainWindow::~_SwGuiCompMainWindow()
{
    //Menus
    QMap<QString,ISwMenu *>::iterator menu_it = _menus.begin();
    while (menu_it != _menus.end())
	{
        if (menu_it.value()!=NULL)
		{
            //S'il etait defini, on le detache de la menubar
            menu_it.value()->GetMenu().setParent(NULL);   
            menu_it.value() = NULL;
        }
        menu_it++;
    } 

    //Actions
    QMap<QString,ISwAction *>::iterator action_it = _actions.begin();
    while (action_it != _actions.end())
	{
        if (action_it.value() != NULL)
		{
            //Si elle etait definie, on la detache de la menubar
            action_it.value()->GetAction().setParent(NULL);    
            action_it.value() = NULL;
        }
        action_it++;
    } 

    //Toolbar
    QMap<QString,ISwToolBar *>::iterator toolbar_it = _toolbars.begin();
    while (toolbar_it != _toolbars.end())
	{
        if (toolbar_it.value() != NULL)
		{
            //Si elle etait definie, on la detache de la main window
            toolbar_it.value()->GetToolBar().setParent(NULL);    
            toolbar_it.value() = NULL;
       }
       toolbar_it++;
    } 

    //Dockwidget
	QMap<QString, SwDockWidget_DockWidget *>::iterator dockwidget_it = _dockwidgets.begin();
    while (dockwidget_it != _dockwidgets.end())
	{
        if (dockwidget_it.value() != NULL)
		{
            //S'il etait definie, on le detache de la main window
            dockwidget_it.value()->setParent(NULL); 
            dockwidget_it.value() = NULL;
        }
        dockwidget_it++;
    } 

	//ListDockWidget
	QMap<QString,ISwListDockWidget *>::iterator listdockwidget_it = _listdockwidgets.begin();
	while (listdockwidget_it != _listdockwidgets.end())
	{
        if (listdockwidget_it.value() != NULL)
		{
			//Pour chaque dock de la liste
			QList<SwDockWidget_DockWidget*> list = listdockwidget_it.value()->GetListDockWidget();
			foreach(SwDockWidget_DockWidget * widget, list)
			{
				//S'il etait definie, on le detache de la main window
				if (widget)
					widget->setParent(NULL);
			}
        }
        listdockwidget_it++;
    } 

	//DockWidget
	for (uint i = 0; i < _dockwidgets_nb; i++)
		unconsummeInterface(QString(CL_WIDGET_INTERFACE_NAME).arg(i));

	//ListDockWidget
	for (uint i = 0; i < _listdockwidgets_nb; i++)
		unconsummeInterface(QString(CL_LISTDOCKWIDGET_INTERFACE_NAME).arg(i));

	//Central widget
	unconsummeInterface(CL_CENTRALWIDGET_INTERFACE_NAME);

	//Status bar
	unconsummeInterface(STATUSBAR_INTERFACE);

	//Event
	unprovideInterface("ISwEvent");

	//Sortie de la Main Window
	if (!_useAsWidget)
		unprovideInterface("MainWindow");
    else
		unprovideInterface("MainWindowAsWidget");

	if (_mainWindow)
		delete _mainWindow;
}

//-----------------------------------------------------------------------------
/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompMainWindow::initializeComponent() throw(SwException)
{
    //Creation de l'interface principale
    _mainWindow = new SwDockWidget_MainWindow();

    if (!_useAsWidget)
		provideInterface<ISwMainWindow>("MainWindow",(ISwMainWindow *)this);
    else
		provideInterface<ISwWidget>("MainWindowAsWidget",(ISwWidget *)this);

    //Central widget
	consummeInterface<ISwWidget>(CL_CENTRALWIDGET_INTERFACE_NAME);

	//Exportation de l'interface ISwWidget
	provideInterface<ISwEvent>("ISwEvent",(ISwEvent*)this);

	//Status bar
	consummeInterface<ISwWidget>(STATUSBAR_INTERFACE);

	//Enregistrement des propriétés
	createPropertiesForThisObject(QString(), true);
	createPropertiesForQObject(_mainWindow,"QWidget"); //Necessaire pour acceder au "show"
    
    //Gestion des menus
	_menus_nb_property=getPropertiesService().CreateProperty<uint>("Connexions_Menus");
    if (_menus_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_menus property\n"));
    } 
    _menus_nb_property->SetDescription("Define how many ISwMenu interfaces this component accept");  
    _menus_nb_property->SetValue(QVariant(_menus_nb));
	_menus_nb_property->GetOnChangeSignal().iconnect(*this, &_SwGuiCompMainWindow::eventPropertyChange);

    //Gestion des actions
    _actions_nb_property=getPropertiesService().CreateProperty<uint>("Connexions_Actions");
    if (_actions_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_actions property\n"));
    }
    _actions_nb_property->SetDescription("Define how many ISwAction interfaces this component accept");  
    _actions_nb_property->SetValue(QVariant(_actions_nb));
	_actions_nb_property->GetOnChangeSignal().iconnect(*this, &_SwGuiCompMainWindow::eventPropertyChange);

    //Gestion des toolbars
    _toolbars_nb_property=getPropertiesService().CreateProperty<uint>("Connexions_Toolbars");
    if (_toolbars_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_toolbars property\n"));
    }
    _toolbars_nb_property->SetDescription("Define how many ISwToolBar interfaces this component accept");  
    _toolbars_nb_property->SetValue(QVariant(_toolbars_nb));
	_toolbars_nb_property->GetOnChangeSignal().iconnect(*this, &_SwGuiCompMainWindow::eventPropertyChange);

    //Gestion des dockwidgets
    _dockwidgets_nb_property = getPropertiesService().CreateProperty<uint>("Connexions_Widgets");
    if (_dockwidgets_nb_property == NULL)
	{
        if (SW_APP->IsVerbose())
			SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_widgets property\n"));
    }
    _dockwidgets_nb_property->SetDescription("Define how many ISwWidget interfaces this component accept");  
    _dockwidgets_nb_property->SetValue(QVariant(_dockwidgets_nb));
	_dockwidgets_nb_property->GetOnChangeSignal().iconnect(*this, &_SwGuiCompMainWindow::eventPropertyChange);

	//Gestion des listes de dockwidgets
    _listdockwidgets_nb_property = getPropertiesService().CreateProperty<uint>("Connexions_ListDockWidgets");
    if (_listdockwidgets_nb_property == NULL)
	{
        if (SW_APP->IsVerbose())
			SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_listdockwidgets property\n"));
    }
    _listdockwidgets_nb_property->SetDescription("Define how many ISwListDockWidget interfaces this component accept");  
    _listdockwidgets_nb_property->SetValue(QVariant(_listdockwidgets_nb));
	_listdockwidgets_nb_property->GetOnChangeSignal().iconnect(*this, &_SwGuiCompMainWindow::eventPropertyChange);

    // choix de l'interface externe
    _use_aswidget_property = getPropertiesService().CreateProperty<bool>("Parameters_Use as widget");    
    if (_use_aswidget_property == NULL)
	{
        if (SW_APP->IsVerbose())
			SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register use as widget property property\n"));
    }
    _use_aswidget_property->SetDescription("Define kind of widget interface this component produces");  
    _use_aswidget_property->SetValue(QVariant(_useAsWidget));
	_use_aswidget_property->GetOnChangeSignal().iconnect(*this, &_SwGuiCompMainWindow::eventPropertyChange);

	//Mode d'affichage
    _show_property = getPropertiesService().CreateProperty<SwEnum>("Parameters_Show Mode");
    if (_show_property != NULL)
	{
        QVariant variant;
        variant.setValue(_show_mode);
        _show_property->SetValue(variant);
        _show_property->SetDescription("Show Mode");
		_show_property->GetOnChangeSignal().iconnect(*this, &_SwGuiCompMainWindow::eventPropertyChange);
    }

	//Parametrage de la sauvegarde automatique
	_saveAutoPeriod_property = getPropertiesService().CreateProperty<uint>("Parameters_SaveAutoPeriod (s)");
    if (_saveAutoPeriod_property == NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register SaveAutoPeriod property\n"));
    } 
    _saveAutoPeriod_property->SetDescription("Define the period for saving the dock configuration");  
    _saveAutoPeriod_property->SetValue(QVariant(_saveAutoPeriod));
	_saveAutoPeriod_property->GetOnChangeSignal().iconnect(*this, &_SwGuiCompMainWindow::eventPropertyChange);

    //Fin
    if (SW_APP->IsVerbose())
		SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiMainWindow done\n"));

}

//-----------------------------------------------------------------------------
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompMainWindow::eventPropertyChange(ISwProperty * property)
{
    uint val;
    QString interface_name;
    QString property_name;
    ISwProperty * toolbar_position;
    //ISwProperty * dockwidget_position;
    QVariant variant;

	//Periode sauvegarde automatique de la configuration des docks
	if (_saveAutoPeriod_property == property)
	{
		unsigned int val = property->GetValue().toUInt();
		if (_mainWindow)
			_mainWindow->setSaveAutoPeriod(val * 1000); //Conversion en millisecondes
	}

	//Widget en sortie
	if (_use_aswidget_property == property) 
	{
		bool boolval=property->GetValue().toBool();    
		if (!_useAsWidget && boolval)
		{
			unprovideInterface("MainWindow");
			//Exportation de l'interface ISwWidget
			provideInterface<ISwWidget>("MainWindowAsWidget", (ISwWidget *)this);
			_useAsWidget = boolval;
		}
		else if (_useAsWidget && !boolval)
		{
			unprovideInterface("MainWindowAsWidget");
			//Exportation de l'interface ISwQMainWindow
			provideInterface<ISwMainWindow>("MainWindow",(ISwMainWindow *)this);
			_useAsWidget = boolval;
		}
	}

	//Changement du nombre de menus
	if (_menus_nb_property == property)
	{
		val = property->GetValue().toUInt();
		if (val == _menus_nb)
			return;
		//S'il faut en supprimer
		if (val < _menus_nb)
		{
			for (uint i = val; i < _menus_nb; i++)
			{
				interface_name = QString(CL_MENU_INTERFACE_NAME).arg(i);
				unconsummeInterface(interface_name);
			}
		}
		//S'il faut en ajouter
		else
		{
			for (uint i = _menus_nb; i < val; i++)
			{
				interface_name = QString(CL_MENU_INTERFACE_NAME).arg(i);
				_menus.insert(interface_name,(ISwMenu *)NULL);
				consummeInterface<ISwMenu>(interface_name);
			}
		}
		_menus_nb = val;
	}

	//Changement du nombre d'actions
    if (_actions_nb_property == property)
	{
        val = property->GetValue().toUInt();
        if (val == _actions_nb)
			return;
		//S'il faut en supprimer
        if (val < _actions_nb)
		{
            for (uint i = val; i < _actions_nb; i++)
			{
                interface_name = QString(CL_ACTION_INTERFACE_NAME).arg(i);
				unconsummeInterface(interface_name);
            }
        }
		//S'il faut en ajouter
		else
		{
            for (uint i = _actions_nb; i < val; i++)
			{
                interface_name = QString(CL_ACTION_INTERFACE_NAME).arg(i);
                _actions.insert(interface_name,(ISwAction *)NULL);
				consummeInterface<ISwAction>(interface_name);
            }
        }
        _actions_nb = val;
    }

	//Changement du nombre de toolbar
    if (_toolbars_nb_property == property)
	{
        val = property->GetValue().toUInt();
        if (val == _toolbars_nb)
			return;
		//S'il faut en supprimer
        if (val < _toolbars_nb)
		{
            for (uint i = val; i < _toolbars_nb; i++)
			{
                interface_name = QString(CL_TOOLBAR_INTERFACE_NAME).arg(i);
				unconsummeInterface(interface_name);
				//Positionnement
                property_name = interface_name+".where";
                _toolbar_positions.erase(_toolbar_positions.find(interface_name));
                getPropertiesService().DestroyProperty(property_name.toLatin1().constData());
            }
        }
		//S'il faut en ajouter
		else
		{
            variant.setValue(_default_toolbar_position);
            for (uint i = _toolbars_nb; i < val; i++)
			{
                interface_name = QString(CL_TOOLBAR_INTERFACE_NAME).arg(i);
                _toolbars.insert(interface_name,(ISwToolBar *)NULL);
				consummeInterface<ISwToolBar>(interface_name);
                //Positionnement
                property_name = interface_name+".where";
                toolbar_position = getPropertiesService().CreateProperty<SwEnum>(property_name.toLatin1().constData());
                if (toolbar_position != NULL)
				{
                    toolbar_position->SetValue(variant);
                    _toolbar_positions.insert(interface_name,toolbar_position);
                    toolbar_position->SetDescription("Define where is attached Toolbar");  

                }
            }
        }
        _toolbars_nb = val;
    }

	//Changement du nombre de docks
    if (_dockwidgets_nb_property == property)
	{
        val = property->GetValue().toUInt();
        if (val == _dockwidgets_nb)
			return;
		//S'il faut en supprimer
        if (val < _dockwidgets_nb)
		{
            for (uint i = val; i < _dockwidgets_nb; i++)
			{
				interface_name = QString(CL_WIDGET_INTERFACE_NAME).arg(i);
				QMap<QString, SwDockWidget_DockWidget *>::const_iterator iDock = _dockwidgets.find(interface_name);
				if (iDock != _dockwidgets.end() && iDock.key() == interface_name)
				{
					getPropertiesService().DestroyPropertiesBeginWith(interface_name);

					delete iDock.value();
					_dockwidgets.remove(interface_name);
				}
				unconsummeInterface(interface_name);
            }
        }
		//S'il faut en ajouter
		else
		{
            for (uint i = _dockwidgets_nb; i < val; i++)
			{
				interface_name = QString(CL_WIDGET_INTERFACE_NAME).arg(i);

				SwDockWidget_DockWidget * dock = new SwDockWidget_DockWidget();
				createPropertiesForQObject(dock, interface_name, true);
				_dockwidgets.insert(interface_name, dock);
				consummeInterface<ISwWidget>(interface_name);
            }
        }
        _dockwidgets_nb = val;
    }

	//Changement du nombre de listes de docks
    if (_listdockwidgets_nb_property == property)
	{
        val = property->GetValue().toUInt();
        if (val == _listdockwidgets_nb)
			return;
		//S'il faut en supprimer
        if (val < _listdockwidgets_nb)
		{
            for (uint i = val; i < _listdockwidgets_nb; i++)
			{
                interface_name = QString(CL_LISTDOCKWIDGET_INTERFACE_NAME).arg(i);
				unconsummeInterface(interface_name);
            }
        }
		//S'il faut en ajouter
		else
		{
            for (uint i = _listdockwidgets_nb; i < val; i++)
			{
                interface_name = QString(CL_LISTDOCKWIDGET_INTERFACE_NAME).arg(i);
                _listdockwidgets.insert(interface_name,(ISwListDockWidget *)NULL);
				consummeInterface<ISwListDockWidget>(interface_name);
            }
        }
        _listdockwidgets_nb = val;
    }

	//Changement du mode d'affichage
    if (_show_property == property)
	{
        SwEnum showmode = _show_property->GetValue().value<SwEnum>();
        _show_mode = showmode;
        showChanged();
    }
}

//---------------------------------------------------------------------
// Interface ISwMainWindow
//---------------------------------------------------------------------
/*! \brief Renvoie le nom du service
\return le nom du service */
SwDockWidget_MainWindow & _SwGuiCompMainWindow::getMainWindow()
{
    return *_mainWindow;
}

//-----------------------------------------------------------------------------
/*! \brief Renvoie le widget
\return le widget */
QWidget & _SwGuiCompMainWindow::GetWidget()
{
    return *(dynamic_cast<QWidget *> (_mainWindow));
}

//-----------------------------------------------------------------------------
void _SwGuiCompMainWindow::showChanged()
{
	switch(_show_mode.ToInt())
	{
		case SW_SHOW_CENTERED:
			{
				_mainWindow->showNormal();
				QDesktopWidget *desktop = QApplication::desktop();
				QRect screensize= desktop->screenGeometry(desktop->primaryScreen());
				QRect windowSize= _mainWindow->frameGeometry();
				QPoint slefttop(screensize.left(),screensize.top());
				QPoint wlefttop;
				slefttop.setX(slefttop.x()+(screensize.width()-windowSize.width())/2);
				slefttop.setY(slefttop.y()+(screensize.height()-windowSize.height())/2);
				_mainWindow->move(slefttop);
			}
			break;

		case SW_SHOW_FULLSCREEN:
			_mainWindow->showFullScreen();
			break;

		case SW_SHOW_MAXIMIZED:
			_mainWindow->showMaximized();
			break;

		case SW_SHOW_MINIMIZED:
			_mainWindow->showMinimized();
			break;

		case SW_SHOW_NORMAL:
		default:
			break;
	}
}

//-----------------------------------------------------------------------------
// Interface ISwEventObservable
//-----------------------------------------------------------------------------
void _SwGuiCompMainWindow::addObserver(ISwEventObserver * obs)
{
	if(obs!=NULL)
		_iSwEvent.push_back(obs);
}

//-----------------------------------------------------------------------------
void _SwGuiCompMainWindow::removeObserver(ISwEventObserver * obs)
{
	if(obs!=NULL)
	{
		if(_iSwEvent.contains(obs))
			_iSwEvent.removeAt(_iSwEvent.indexOf(obs));
	}	
}

//-----------------------------------------------------------------------------
void _SwGuiCompMainWindow::notify(QEvent * event)
{
	ISwEventObserver* itObs = NULL;
	foreach(itObs, _iSwEvent)
	{
		itObs->onEvent(event);
	}
}

//-----------------------------------------------------------------------------
void _SwGuiCompMainWindow::closeEvent(QCloseEvent* event)
{
	notify((QEvent*)event);
}

//-----------------------------------------------------------------------------
// Interface ISwFinalizer
//-----------------------------------------------------------------------------
bool _SwGuiCompMainWindow::Finalize( quint64 historic_index )
{
	if (_mainWindow)
	{
		_mainWindow->loadConfiguration(getFilePath());
		_finalized = true;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
// Interface ISwListDockWidgetListener
//----------------------------------------------------------------------------
void _SwGuiCompMainWindow::addDockWidget(SwDockWidget_DockWidget * dock, QString menuName)
{
	if (dock)
		_mainWindow->addDockWidget(dock, menuName);
}

void _SwGuiCompMainWindow::removeDockWidget(SwDockWidget_DockWidget * dock)
{
	if (dock)
		_mainWindow->removeDockWidget(dock);
}

//-----------------------------------------------------------------------------
// Gestion des Propriétés
//-----------------------------------------------------------------------------
void _SwGuiCompMainWindow::interfaceAvailable(QString interfaceName)
{
	//Central widget
	if(interfaceName == CL_CENTRALWIDGET_INTERFACE_NAME)
	{
		ISwWidget * widget = getInterface<ISwWidget>(interfaceName);
		if (widget && _mainWindow)
			_mainWindow->setMainWidget(&(widget->GetWidget()));
		return;
	}

	//DockWidget
	QMap<QString, SwDockWidget_DockWidget *>::iterator dockwidget_it = _dockwidgets.find(interfaceName);
	if (dockwidget_it != _dockwidgets.end() && dockwidget_it.value() != NULL)
	{
		ISwWidget * widget = getInterface<ISwWidget>(interfaceName);
		if (widget)
		{
			dockwidget_it.value()->setWidget(&(widget->GetWidget()));
			_mainWindow->addDockWidget(dockwidget_it.value());
		}
		return;
	}

	//Liste DockWidget
	QMap<QString,ISwListDockWidget *>::iterator listdockwidget_it = _listdockwidgets.find(interfaceName);
	if (listdockwidget_it != _listdockwidgets.end() && listdockwidget_it.value() == NULL)
	{
		ISwListDockWidget * widget = getInterface<ISwListDockWidget>(interfaceName);
		if (widget)
		{
			listdockwidget_it.value() = widget;
			//Enregistrement du listener
			widget->addDockWidgetListener(this);
			//Ajout des docks presents
			foreach(SwDockWidget_DockWidget * dock, widget->GetListDockWidget())
			{
				if (dock && _mainWindow)
					_mainWindow->addDockWidget(dock, widget->getName());
			}
		}
		return;
	}

	//Menu
	QMap<QString,ISwMenu *>::iterator menu_it = _menus.find(interfaceName);
    if (menu_it != _menus.end() && menu_it.value() == NULL)
	{
		ISwMenu * menu = getInterface<ISwMenu>(interfaceName);
		if (menu)
		{
			menu_it.value() = menu;  
			_mainWindow->menuBar()->addMenu(&(menu->GetMenu()));
		}
        return;
    }

	//Action
	QMap<QString,ISwAction *>::iterator action_it = _actions.find(interfaceName);
	if (action_it != _actions.end() && action_it.value() == NULL)
	{
		ISwAction * action = getInterface<ISwAction>(interfaceName);
		if (action)
		{
			action_it.value() = action;
			_mainWindow->menuBar()->addAction(&(action->GetAction()));
		}
		return;
	}

	//Toolbar
	QMap<QString,ISwToolBar *>::iterator toolbar_it = _toolbars.find(interfaceName);
	QMap<QString,ISwProperty *>::iterator toolbar_position_it = _toolbar_positions.find(interfaceName);
	if (toolbar_it != _toolbars.end() && toolbar_position_it != _toolbar_positions.end() && toolbar_it.value() == NULL)
	{
		ISwToolBar * toolbar = getInterface<ISwToolBar>(interfaceName);
		if (toolbar)
		{
			SwEnum enum_value = toolbar_position_it.value()->GetValue().value<SwEnum>();
			toolbar_it.value() = toolbar;
			_mainWindow->addToolBar((Qt::ToolBarArea)enum_value.ToInt(),&(toolbar->GetToolBar()));
		}
		return;
	}

	//Status bar
	if(interfaceName == STATUSBAR_INTERFACE)
	{
		ISwWidget * widget = getInterface<ISwWidget>(interfaceName);
		if (widget && _mainWindow && _mainWindow->statusBar())
			_mainWindow->statusBar()->addPermanentWidget(&widget->GetWidget(), 1);
		return;
	}
}

//-----------------------------------------------------------------------------
void _SwGuiCompMainWindow::interfaceUnavailable(QString interfaceName)
{
	//Central widget
	if(interfaceName == CL_CENTRALWIDGET_INTERFACE_NAME)
	{
		ISwWidget * widget = getInterface<ISwWidget>(interfaceName);
		if (widget && _mainWindow)
			_mainWindow->setMainWidget(0);
		return;
	}

	//DockWidget
	QMap<QString, SwDockWidget_DockWidget *>::iterator dockwidget_it = _dockwidgets.find(interfaceName);
    if (dockwidget_it != _dockwidgets.end() && dockwidget_it.value() != NULL)
	{
		SwDockWidget_DockWidget * dock = dockwidget_it.value();
		if (dock && _mainWindow)
			_mainWindow->removeDockWidget(dock);
		dockwidget_it.value()->setWidget(NULL);
		return;
	}

	//ListDockWidget
	QMap<QString,ISwListDockWidget *>::iterator listdockwidget_it = _listdockwidgets.find(interfaceName);
    if (listdockwidget_it != _listdockwidgets.end() && listdockwidget_it.value() != NULL)
	{
		ISwListDockWidget * widget = listdockwidget_it.value();
		if (widget && _mainWindow)
		{
			//Suppression du listener
			widget->removeDockWidgetListener(this);
			//Suppression des docks presents
			foreach(SwDockWidget_DockWidget * dock, widget->GetListDockWidget())
			{
				if (dock && _mainWindow)
					_mainWindow->removeDockWidget(dock);
			}
			//Suppression du menu
			_mainWindow->removeMenu(widget->getName());
		}
		listdockwidget_it.value()=NULL;
		return;
	}

	//Menu
    QMap<QString,ISwMenu *>::iterator menu_it = _menus.find(interfaceName);
    if (menu_it != _menus.end() && menu_it.value() != NULL)
	{
        menu_it.value()->GetMenu().setParent(NULL);   
        menu_it.value() = NULL;
        return;
    }

	//Action
    QMap<QString,ISwAction *>::iterator action_it = _actions.find(interfaceName);
    if (action_it !=_actions.end() && action_it.value() != NULL)
	{
        action_it.value()->GetAction().setParent(NULL);    
        action_it.value() = NULL;
		return;
    }

	//Toolbar
	QMap<QString,ISwToolBar *>::iterator toolbar_it = _toolbars.find(interfaceName);
    if (toolbar_it != _toolbars.end() && toolbar_it.value() != NULL)
	{
        toolbar_it.value()->GetToolBar().setParent(NULL);    
        toolbar_it.value() = NULL;
		return;
	}
	
	//Status bar
	if(interfaceName == STATUSBAR_INTERFACE)
	{
		ISwWidget * widget = getInterface<ISwWidget>(interfaceName);
		if (widget && _mainWindow && _mainWindow->statusBar())
		{
			_mainWindow->statusBar()->removeWidget(&widget->GetWidget());
			widget->GetWidget().setParent(0);
		}
		return;
	}
}

//-----------------------------------------------------------------------------
void _SwGuiCompMainWindow::saveConfiguration()
{
	if (_mainWindow)
		_mainWindow->saveConfiguration();
}

//-----------------------------------------------------------------------------
void _SwGuiCompMainWindow::restoreConfiguration()
{
	if (_mainWindow)
		_mainWindow->restoreConfiguration();
}

//-------------------------------------------------------------------------
QString _SwGuiCompMainWindow::getFilePath()
{
	QString retFilePath = "";
	QString directoryBasePath = "";

	if(_pathType == UserHomeDir)
	{
		retFilePath = HOME_PATH + _relativePath;
		directoryBasePath = HOME_PATH;
	}
	else if(_pathType == ApplicationDir)
	{
		retFilePath = APP_PATH + _relativePath;
		directoryBasePath = APP_PATH;
	}
	else if(_pathType == Fixed)
	{
		retFilePath = _absolutePath;
	}

	// Vérification des répertoires
	checkDirectory(directoryBasePath);

	return retFilePath;
}

//-------------------------------------------------------------------------
void _SwGuiCompMainWindow::checkDirectory(QString inDirectoryBasePath)
{
	QString directoryPath = inDirectoryBasePath;
	QStringList directoriesToCheck = _relativePath.split("\\");

	// On teste si le(s) répertoire(s) existe(nt)
	for(int i=0; i<directoriesToCheck.size(); i++)
	{
		if(!directoriesToCheck[i].contains("."))
		{
			directoryPath += directoriesToCheck[i];

			// Si ce n'est pas le cas, on le créé
			if(_access(directoryPath.toStdString().c_str(), 0) == -1)
			{
				qDebug() << "The repertory named " << directoryPath << " doesn't exist, creation!!";
				_mkdir(directoryPath.toStdString().c_str());
			}
			directoryPath += "\\";
		}
	}
}

//-------------------------------------------------------------------------
void _SwGuiCompMainWindow::setPathType( _SwGuiCompMainWindow::PathType val )
{
	if(val == UserHomeDir)
	{
		getPropertiesService().ChangePropertyEdition("relativePath",true);
		getPropertiesService().ChangePropertyEdition("absolutePath",false);
	}
	else if(val == ApplicationDir)
	{
		getPropertiesService().ChangePropertyEdition("relativePath",true);
		getPropertiesService().ChangePropertyEdition("absolutePath",false);
	}
	else if(val == Fixed)
	{
		getPropertiesService().ChangePropertyEdition("relativePath",false);
		getPropertiesService().ChangePropertyEdition("absolutePath",true);
	}
	_pathType = val;
}

//-------------------------------------------------------------------------
_SwGuiCompMainWindow::PathType _SwGuiCompMainWindow::getPathType()
{
	return _pathType;
}

//-------------------------------------------------------------------------
QString _SwGuiCompMainWindow::getAbsolutePath()
{
	return _absolutePath;
}

//-------------------------------------------------------------------------
void _SwGuiCompMainWindow::setAbsolutePath( QString val )
{
	_absolutePath = val;
}

//-------------------------------------------------------------------------
QString _SwGuiCompMainWindow::getRelativePath()
{
	return _relativePath;
}

//-------------------------------------------------------------------------
void _SwGuiCompMainWindow::setRelativePath( QString val )
{
	_relativePath = val;
}