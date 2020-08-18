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
#include <SwMacros.h>
#include "_SwGuiCompQMainWindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>
#include <SwFileDescriptor.h>

#include "ISwServiceShortcuts.h"
#include <SwApplication.h>
#include <QMetaEnum>

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
#define SW_SHOW_TOPRIBBON 5
#define SW_CLOSE_CLOSED 0
#define SW_CLOSE_HIDE 1
#define TOGGLE_FULLSCREEN "Show Fullscreen"

//-----------------------------------------------------------------------
_SwGuiCompQMainWindow::_SwGuiCompQMainWindow() : Component()
{
    _firstTimeRestore = true;
    _use_aswidget_property = 0;
    _menus_nb = 0;
    _actions_nb = 0;
    _toolbars_nb = 0;
    _dockwidgets_nb = 0;
    _tmp_handle_menu = nullptr;
    _tmp_handle_action = nullptr;
    _tmp_handle_toolbar = nullptr;
    _tmp_handle_dockwidget = nullptr;
    _handle_central_widget = nullptr;
    _default_toolbar_position.AddKey( Qt::LeftToolBarArea, "Left" );
    _default_toolbar_position.AddKey( Qt::RightToolBarArea, "Right" );
    _default_toolbar_position.AddKey( Qt::TopToolBarArea, "Top" );
    _default_toolbar_position.AddKey( Qt::BottomToolBarArea, "Bottom" );
    _default_toolbar_position.FromInt( Qt::TopToolBarArea );
    _default_dockwidget_position.AddKey( Qt::LeftDockWidgetArea, "Left" );
    _default_dockwidget_position.AddKey( Qt::RightDockWidgetArea, "Right" );
    _default_dockwidget_position.AddKey( Qt::TopDockWidgetArea, "Top" );
    _default_dockwidget_position.AddKey( Qt::BottomDockWidgetArea, "Bottom" );
    _default_dockwidget_position.FromInt( Qt::LeftDockWidgetArea );
    _show_mode.AddKey( SW_SHOW_NORMAL, "None" );
    _show_mode.AddKey( SW_SHOW_CENTERED, "Centered" );
    _show_mode.AddKey( SW_SHOW_FULLSCREEN, "FullScreen" );
    _show_mode.AddKey( SW_SHOW_MAXIMIZED, "Maximized" );
    _show_mode.AddKey( SW_SHOW_MINIMIZED, "Minimized" );
	_show_mode.AddKey( SW_SHOW_TOPRIBBON, "TopRibbon");
    _show_mode.FromInt( SW_SHOW_NORMAL );
	_close_mode.AddKey(SW_CLOSE_CLOSED, "Closed");
	_close_mode.AddKey(SW_CLOSE_HIDE, "Hide");
	_close_mode.FromInt(SW_CLOSE_CLOSED);
    _useAsWidget = false;
    _protectClosing = false;
	_save_geometry_ini_file = false;
	_geometryPath = "";

	_windowFlagsEnum.ChangeFlagStatus(true);
	const QMetaObject * metaEnumMetaObject = QMetaType::metaObjectForType(qMetaTypeId<Qt::WindowFlags>());
	if (metaEnumMetaObject)
	{
		QString metaEnumName = QMetaType::typeName(qMetaTypeId<Qt::WindowFlags>());
		metaEnumName = metaEnumName.mid(metaEnumName.lastIndexOf(":") + 1);
		QMetaEnum metaEnum = metaEnumMetaObject->enumerator(metaEnumMetaObject->indexOfEnumerator(metaEnumName.toLatin1()));

		for (int key = 0; key < metaEnum.keyCount(); ++key) {
			_windowFlagsEnum.AddKey(metaEnum.value(key), metaEnum.key(key));
		}
	}
	_windowFlagsEnum.FromInt(windowFlags());
    
    // Shortcuts
    ISwServiceShortcuts * serviceShortcuts = dynamic_cast <ISwServiceShortcuts *>( SW_APP->QueryService( CG_SW_SERVICE_SHORTCUTS ) );
    if( serviceShortcuts )
        serviceShortcuts->registerCommand( "Display", TOGGLE_FULLSCREEN, this );
}

//-----------------------------------------------------------------------
_SwGuiCompQMainWindow::~_SwGuiCompQMainWindow()
{
	saveStateGeometry();

    delete _mainWindowService;
    
    QMap<QString, ISwMenu *>::iterator menu_it;
    QMap<QString, ISwAction *>::iterator action_it;
    QMap<QString, ISwToolBar *>::iterator toolbar_it;
    QMap<QString, ISwQDockWidget *>::iterator dockwidget_it;
    
    //Si c'est un menu
    menu_it = _menus.begin();
    while( menu_it != _menus.end() )
    {
        if( menu_it.value() )
        {
            //Et qu'il etait defini, on le detache de la menubar
            menu_it.value()->GetMenu().setParent( nullptr );
            menu_it.value() = nullptr;
        }
        menu_it++;
    }
    //Si c'est une action
    action_it = _actions.begin();
    while( action_it != _actions.end() )
    {
        if( action_it.value() )
        {
            //Et qu'ellle etait definie, on la detache de la menubar
            action_it.value()->GetAction().setParent( nullptr );
            action_it.value() = nullptr;
        }
        action_it++;
    }
    //Si c'est une toolbar
    toolbar_it = _toolbars.begin();
    while( toolbar_it != _toolbars.end() )
    {
        if( toolbar_it.value() )
        {
            //Et qu'ellle etait definie, on la detache de la main window
            toolbar_it.value()->GetToolBar().setParent( nullptr );
            toolbar_it.value() = nullptr;
        }
        toolbar_it++;
    }
    
    //Si c'est un dockwidget
    dockwidget_it = _dockwidgets.begin();
    if( dockwidget_it != _dockwidgets.end() )
    {
        if( dockwidget_it.value() )
        {
            //Et qu'ellle etait definie, on la detache de la main window
            dockwidget_it.value()->GetDockWidget().setParent( nullptr );
            dockwidget_it.value() = nullptr;
        }
        dockwidget_it++;
    }
    
    //Si c'est le widget central Et qu'il est defini, on le detache du parent
    if( _handle_central_widget )
        _handle_central_widget->setParent( nullptr );
        
    ISwServiceShortcuts * serviceShortcuts = dynamic_cast <ISwServiceShortcuts *>( SW_APP->QueryService( CG_SW_SERVICE_SHORTCUTS ) );
    if( serviceShortcuts )
        serviceShortcuts->unregisterCommand( "Display", TOGGLE_FULLSCREEN, this );
}


//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::initializeComponent() throw( SwException )
{
    if( !_useAsWidget )
        getIProviderService().RegisterProvidedInterface<ISwQMainWindow>( "MainWindow", ( ISwQMainWindow * )this );
    else
        getIProviderService().RegisterProvidedInterface<QWidget>( "MainWindowAsWidget", this );
        
    //Importation de l'interface QWidget
    getIConsumerService().RegisterConsumedInterface<QWidget>( CL_CENTRALWIDGET_INTERFACE_NAME, &_handle_central_widget );
    
    //Exportation de l'interface QWidget
    getIProviderService().RegisterProvidedInterface<ISwEvent>( "ISwEvent", ( ISwEvent * )this );
    
    //Enregistrement des propriétés
    getPropertiesService().CreatePropertiesForQObject( ( QMainWindow * )this, "QMainWindow" );
    
    //Gestion des menus
    _menus_nb_property = getPropertiesService().CreateProperty<uint>( "nb_menus" );
    if( _menus_nb_property == nullptr )
    {
        if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Warning, QString( "Fail to register nb_menus property\n" ) );
    }
    _menus_nb_property->SetDescription( "Define how many ISwMenu interfaces this component accept" );
    _menus_nb_property->SetValue( QVariant( _menus_nb ) );
    enableListeningChangeForProperty( _menus_nb_property );
    
    //Gestion des actions
    _actions_nb_property = getPropertiesService().CreateProperty<uint>( "nb_actions" );
    if( _actions_nb_property == nullptr )
    {
        if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Warning, QString( "Fail to register nb_actions property\n" ) );
    }
    _actions_nb_property->SetDescription( "Define how many ISwAction interfaces this component accept" );
    _actions_nb_property->SetValue( QVariant( _actions_nb ) );
    enableListeningChangeForProperty( _actions_nb_property );
    
    //Gestion des toolbars
    _toolbars_nb_property = getPropertiesService().CreateProperty<uint>( "nb_toolbars" );
    if( _toolbars_nb_property == nullptr )
    {
        if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Warning, QString( "Fail to register nb_toolbars property\n" ) );
    }
    _toolbars_nb_property->SetDescription( "Define how many ISwToolBar interfaces this component accept" );
    _toolbars_nb_property->SetValue( QVariant( _toolbars_nb ) );
    enableListeningChangeForProperty( _toolbars_nb_property );
    
    //Gestion des dockwidgets
    _dockwidgets_nb_property = getPropertiesService().CreateProperty<uint>( "nb_dockwidgets" );
    if( _dockwidgets_nb_property == nullptr )
    {
        if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Warning, QString( "Fail to register nb_dockwidgets property\n" ) );
    }
    _dockwidgets_nb_property->SetDescription( "Define how many ISwQDockWidget interfaces this component accept" );
    _dockwidgets_nb_property->SetValue( QVariant( _dockwidgets_nb ) );
    enableListeningChangeForProperty( _dockwidgets_nb_property );
    
    // choix de l'interface externe
    _use_aswidget_property = getPropertiesService().CreateProperty<bool>( "use_aswidget" );
    if( _use_aswidget_property == nullptr )
    {
        if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Warning, QString( "Fail to register _use_aswidget_property property\n" ) );
    }
    _use_aswidget_property->SetDescription( "Define kind of widget interface this component produces" );
    _use_aswidget_property->SetValue( QVariant( _useAsWidget ) );
    enableListeningChangeForProperty( _use_aswidget_property );
    
    _show_property = getPropertiesService().CreateProperty<SwEnum>( "ShowMode" );
    if( _show_property != nullptr )
    {
        QVariant variant;
        variant.setValue( _show_mode );
        _show_property->SetValue( variant );
        _show_property->SetDescription( "Show Mode" );
        enableListeningChangeForProperty( _show_property );
    }
    
    _protect_closing_property = getPropertiesService().CreateProperty<bool>( "confirm closing dialog" );
    if( _protect_closing_property == nullptr )
    {
        if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Warning, QString( "Fail to register _protect_closing_property property\n" ) );
    }
    _protect_closing_property->SetDescription( "Define kind of widget interface this component produces" );
    _protect_closing_property->SetValue( QVariant( _protectClosing ) );
    enableListeningChangeForProperty( _protect_closing_property );

	_save_geometry_ini_file_property = getPropertiesService().CreateProperty<bool>("save geometry");
	if (_save_geometry_ini_file_property == nullptr)
	{
		if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning, QString("Fail to register _save_geometry_ini_file property\n"));
	}
	_save_geometry_ini_file = false;
	_save_geometry_ini_file_property->SetDescription("Define if the QMainWindow geometry is saving in ini file");
	_save_geometry_ini_file_property->SetValue(QVariant(_save_geometry_ini_file));
	enableListeningChangeForProperty(_save_geometry_ini_file_property);

	_config_path_property = getPropertiesService().CreateProperty<QString>("configPath");
	if (_config_path_property == nullptr)
	{
		if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning, QString("Fail to register _config_path_property property\n"));
	}	
	_config_path_property->SetDescription("Define if the QMainWindow path geometry ini file");
	_config_path_property->SetValue(QVariant(_configPath));
	enableListeningChangeForProperty(_config_path_property);

	_close_property = getPropertiesService().CreateProperty<SwEnum>("Close or Hide");
	if (_close_property != nullptr)
	{
		QVariant variant;
		variant.setValue(_close_mode);
		_close_property->SetValue(variant);
		_close_property->SetDescription("Close Mode");
		enableListeningChangeForProperty(_close_property);
	}

	_windowFlags = getPropertiesService().CreateProperty<SwEnum>("Window flags");
 	if (_windowFlags != nullptr)
	{
		QVariant variant;
		variant.setValue(_windowFlagsEnum);
 		_windowFlags->SetValue(variant);
		_windowFlags->SetDescription("This enum type is used to specify various window-system properties for the widget.");
		enableListeningChangeForProperty(_windowFlags);
 	}
    
    //Fin
    if( SW_APP->IsVerbose() )
        SW_APP->Logger().Log( LogLvl_Info, QString( "InitializeResources of SwGuiMainWindow done\n" ) );
        
    //Provide the service
    _mainWindowService = new StreamWork::Service::_SwServiceMainWindow( this );
}

//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::eventPropertyChange( ISwProperty * property )
{
    uint val;
    QString interface_name;
    QString property_name;
    ISwProperty * toolbar_position;
    ISwProperty * dockwidget_position;
    QVariant variant;
    if( _use_aswidget_property == property )
    {
        bool boolval = property->GetValue().toBool();
        if( !_useAsWidget && boolval )
        {
            getIProviderService().UnregisterProvidedInterface( "MainWindow" );
            //Exportation de l'interface QWidget
            getIProviderService().RegisterProvidedInterface<QWidget>( "MainWindowAsWidget", this );
        }
        else
        {
            if( _useAsWidget && !boolval )
            {
                getIProviderService().UnregisterProvidedInterface( "MainWindowAsWidget" );
                //Exportation de l'interface ISwQMainWindow
                getIProviderService().RegisterProvidedInterface<ISwQMainWindow>( "MainWindow", ( ISwQMainWindow * )this );
            }
        }
    }
    if( _menus_nb_property == property )
    {
        val = property->GetValue().toUInt();
        if( val == _menus_nb ) return;
        if( val < _menus_nb )
        {
            for( uint i = val; i < _menus_nb; i++ )
            {
				interface_name = QString(CL_MENU_INTERFACE_NAME).arg(i);
                getIConsumerService().UnregisterConsumedInterface( interface_name );
            }
        }
        else
        {
            for( uint i = _menus_nb; i < val; i++ )
            {
                interface_name = QString( CL_MENU_INTERFACE_NAME ).arg( i );
                _menus.insert( interface_name, ( ISwMenu * ) nullptr );
                getIConsumerService().RegisterConsumedInterface<ISwMenu>( interface_name, &_tmp_handle_menu );
            }
        }
        _menus_nb = val;
    }
    if( _actions_nb_property == property )
    {
        val = property->GetValue().toUInt();
        if( val == _actions_nb ) return;
        if( val < _actions_nb )
        {
            for( uint i = val; i < _actions_nb; i++ )
            {
                interface_name = QString( CL_ACTION_INTERFACE_NAME ).arg( i );
                getIConsumerService().UnregisterConsumedInterface( interface_name );
            }
        }
        else
        {
            for( uint i = _actions_nb; i < val; i++ )
            {
                interface_name = QString( CL_ACTION_INTERFACE_NAME ).arg( i );
                _actions.insert( interface_name, ( ISwAction * ) nullptr );
                getIConsumerService().RegisterConsumedInterface<ISwAction>( interface_name, &_tmp_handle_action );
            }
        }
        _actions_nb = val;
    }
    if( _toolbars_nb_property == property )
    {
        val = property->GetValue().toUInt();
        if( val == _toolbars_nb ) return;
        if( val < _toolbars_nb )
        {
            for( uint i = val; i < _toolbars_nb; i++ )
            {
                interface_name = QString( CL_TOOLBAR_INTERFACE_NAME ).arg( i );
                getIConsumerService().UnregisterConsumedInterface( interface_name );
                property_name = interface_name + ".where";
                _toolbar_positions.erase( _toolbar_positions.find( interface_name ) );
                getPropertiesService().DestroyProperty( property_name.toLatin1().constData() );
            }
        }
        else
        {
            variant.setValue( _default_toolbar_position );
            for( uint i = _toolbars_nb; i < val; i++ )
            {
                interface_name = QString( CL_TOOLBAR_INTERFACE_NAME ).arg( i );
                _toolbars.insert( interface_name, ( ISwToolBar * ) nullptr );
                getIConsumerService().RegisterConsumedInterface<ISwToolBar>( interface_name, &_tmp_handle_toolbar );
                //Ajout d'une propriete pour le placement
                property_name = interface_name + ".where";
                toolbar_position = getPropertiesService().CreateProperty<SwEnum>( property_name.toLatin1().constData() );
                if( toolbar_position )
                {
                    toolbar_position->SetValue( variant );
                    _toolbar_positions.insert( interface_name, toolbar_position );
                    toolbar_position->SetDescription( "Define where is attached Toolbar" );
                    
                }
            }
        }
        _toolbars_nb = val;
    }
    if( _dockwidgets_nb_property == property )
    {
        val = property->GetValue().toUInt();
        if( val == _dockwidgets_nb ) return;
        if( val < _dockwidgets_nb )
        {
            for( uint i = val; i < _dockwidgets_nb; i++ )
            {
                interface_name = QString( CL_DOCKWIDGET_INTERFACE_NAME ).arg( i );
                getIConsumerService().UnregisterConsumedInterface( interface_name );
                property_name = interface_name + ".where";
                _dockwidget_positions.erase( _dockwidget_positions.find( interface_name ) );
                getPropertiesService().DestroyProperty( property_name.toLatin1().constData() );
            }
        }
        else
        {
            variant.setValue( _default_dockwidget_position );
            for( uint i = _dockwidgets_nb; i < val; i++ )
            {
                interface_name = QString( CL_DOCKWIDGET_INTERFACE_NAME ).arg( i );
                _dockwidgets.insert( interface_name, nullptr );
                getIConsumerService().RegisterConsumedInterface<ISwQDockWidget>( interface_name, &_tmp_handle_dockwidget );
                //Ajout d'une propriete pour le placement
                property_name = interface_name + ".where";
                dockwidget_position = getPropertiesService().CreateProperty<SwEnum>( property_name.toLatin1().constData() );
                if( dockwidget_position )
                {
                    dockwidget_position->SetValue( variant );
                    _dockwidget_positions.insert( interface_name, dockwidget_position );
                    dockwidget_position->SetDescription( "Define where is attached DockWidget" );
                }
            }
        }
        _dockwidgets_nb = val;
    }
    if( _show_property == property )
    {
        SwEnum showmode = _show_property->GetValue().value<SwEnum>();
        _show_mode = showmode;
        showChanged();
    }
    if( _protect_closing_property == property )
    {
        _protectClosing = property->GetValue().toBool();
    }

	if (_save_geometry_ini_file_property == property)
	{
		_save_geometry_ini_file = property->GetValue().toBool();
		if (_save_geometry_ini_file && !_geometryPath.isEmpty())
			restoreStateGeometry();
	}

	if (_config_path_property == property)
	{
		_configPath = property->GetValue().toString();
		_geometryPath = _configPath + QDir::separator() + QStringLiteral("geometry.ini");
		if (_save_geometry_ini_file && !_geometryPath.isEmpty())
			restoreStateGeometry();
	}

	if (_close_property == property)
	{
		SwEnum closemode = _close_property->GetValue().value<SwEnum>();
		_close_mode = closemode;
	}

	if (_windowFlags == property)
	{
		SwEnum wFlags = _windowFlags->GetValue().value<SwEnum>();
		_windowFlagsEnum = wFlags;
		QPoint pos = this->pos();
		GetMainWindow().setWindowFlags(static_cast<Qt::WindowFlags>(wFlags.ToInt()));
		if (pos.x() < 0)
			pos.setX(0);
		if (pos.y() < 0)
			pos.setY(0);
		move(pos);
		show();
	}
}

//-----------------------------------------------------------------------
QMainWindow & _SwGuiCompQMainWindow::GetMainWindow()
{
    return *this;
}

//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::eventBeforeInterfaceAvailability( QString interface_name, SwComponent_Class * provider_host )
{
    QMap<QString, ISwMenu *>::iterator menu_it;
    QMap<QString, ISwAction *>::iterator action_it;
    QMap<QString, ISwToolBar *>::iterator toolbar_it;
    QMap<QString, ISwQDockWidget *>::iterator dockwidget_it;
    
    //Si c'est un menu
    menu_it = _menus.find( interface_name );
    if( menu_it != _menus.end() )
    {
        if( menu_it.value() )
        {
            //Et qu'il etait defini, on le detache de la menubar
            menu_it.value()->GetMenu().setParent( nullptr );
            menu_it.value() = nullptr;
        }
        return;
    }
    //Si c'est une action
    action_it = _actions.find( interface_name );
    if( action_it != _actions.end() )
    {
        if( action_it.value() )
        {
            //Et qu'elle etait definie, on la detache de la menubar
            action_it.value()->GetAction().setParent( nullptr );
            action_it.value() = nullptr;
        }
        return;
    }
    //Si c'est une toolbar
    toolbar_it = _toolbars.find( interface_name );
    if( toolbar_it != _toolbars.end() )
    {
        if( toolbar_it.value() != nullptr )
        {
            //Et qu'elle etait definie, on la detache de la main window
            toolbar_it.value()->GetToolBar().setParent( nullptr );
            toolbar_it.value() = nullptr;
        }
        return;
    }
    //Si c'est un dockwidget
    dockwidget_it = _dockwidgets.find( interface_name );
    if( dockwidget_it != _dockwidgets.end() )
    {
        if( dockwidget_it.value() )
        {
            //Et qu'elle etait definie, on la detache de la main window
            dockwidget_it.value()->GetDockWidget().setParent( nullptr );
            dockwidget_it.value() = nullptr;
        }
        return;
    }
    //Si c'est le widget central
    if( interface_name == CL_CENTRALWIDGET_INTERFACE_NAME && _handle_central_widget )
    {
        //Et qu'il est defini, on le detache du parent
        _handle_central_widget->setParent( nullptr );
        return;
    }
    
}

//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::eventAfterInterfaceAvailability( QString interface_name, SwComponent_Class * provider_host )
{
    QMap<QString, ISwMenu *>::iterator menu_it;
    QMap<QString, ISwAction *>::iterator action_it;
    QMap<QString, ISwToolBar *>::iterator toolbar_it;
    QMap<QString, ISwProperty *>::iterator toolbar_position_it;
    QMap<QString, ISwQDockWidget *>::iterator dockwidget_it;
    QMap<QString, ISwProperty *>::iterator dockwidget_position_it;
    
    //Si c'est un menu
    menu_it = _menus.find( interface_name );
    if( menu_it != _menus.end() )
    {
        if( menu_it.value() == nullptr && _tmp_handle_menu )
        {
            //Et qu'il etait non defini, on l'enregistre et l'attache a la menubar
            menu_it.value() = _tmp_handle_menu;
            this->menuBar()->addMenu( &( _tmp_handle_menu->GetMenu() ) );
        }
        return;
    }
    //Si c'est une action
    action_it = _actions.find( interface_name );
    if( action_it != _actions.end() )
    {
        if( action_it.value() == nullptr && _tmp_handle_action )
        {
            //Et qu'ellle etait non definie,on l'enregistre et l'attache a la menubar
            action_it.value() = _tmp_handle_action;
            this->menuBar()->addAction( &( _tmp_handle_action->GetAction() ) );
        }
        return;
    }
    //Si c'est une toolbar
    toolbar_it = _toolbars.find( interface_name );
    toolbar_position_it = _toolbar_positions.find( interface_name );
    if( toolbar_it != _toolbars.end() && toolbar_position_it != _toolbar_positions.end() )
    {
        if( toolbar_it.value() == nullptr && _tmp_handle_toolbar != nullptr )
        {
            //Et qu'ellle etait non definie,on l'enregistre et l'attache a la mainwindow
            SwEnum enum_value = toolbar_position_it.value()->GetValue().value<SwEnum>();
            toolbar_it.value() = _tmp_handle_toolbar;
            this->addToolBar( ( Qt::ToolBarArea )enum_value.ToInt(), &( _tmp_handle_toolbar->GetToolBar() ) );
        }
        return;
    }
    //Si c'est un dockwidget
    dockwidget_it = _dockwidgets.find( interface_name );
    dockwidget_position_it = _dockwidget_positions.find( interface_name );
    if( dockwidget_it != _dockwidgets.end() && dockwidget_position_it != _dockwidget_positions.end() )
    {
    
        if( dockwidget_it.value() == nullptr  && _tmp_handle_dockwidget != nullptr )
        {
            //Et qu'ellle etait non definie,on l'enregistre et l'attache a la mainwindow
            SwEnum enum_value = dockwidget_position_it.value()->GetValue().value<SwEnum>();
            dockwidget_it.value() = _tmp_handle_dockwidget;
            //Cas du tabbed mode
            if( this->dockOptions().testFlag( QMainWindow::ForceTabbedDocks ) |
                this->dockOptions().testFlag( QMainWindow::AllowTabbedDocks ) )
            {
                QDockWidget * tabbedWidget = 0;
                //QObjectList list=_main_window->children();
                QObjectList list = ( ( QMainWindow * )this )->children();
                for( int i = 0; i < list.count() && tabbedWidget == 0; i++ )
                {
                    QDockWidget * aDockWidget = dynamic_cast<QDockWidget *>( list[i] );
                    if( aDockWidget != 0 && this->dockWidgetArea( aDockWidget ) == ( Qt::DockWidgetArea )enum_value.ToInt() )
                    {
                        tabbedWidget = aDockWidget;
                    }
                }
                this->addDockWidget( ( Qt::DockWidgetArea )enum_value.ToInt(), &( _tmp_handle_dockwidget->GetDockWidget() ) );
                if( tabbedWidget != 0 )
                {
                    this->tabifyDockWidget( tabbedWidget, &( _tmp_handle_dockwidget->GetDockWidget() ) );
                }
            }
            else
            {
                this->addDockWidget( ( Qt::DockWidgetArea )enum_value.ToInt(), &( _tmp_handle_dockwidget->GetDockWidget() ) );
            }
            
        }
        return;
    }
    //Si c'est le widget central
    if( interface_name == CL_CENTRALWIDGET_INTERFACE_NAME && _handle_central_widget != nullptr )
    {
        this->setCentralWidget( _handle_central_widget );
        return;
    }
    
}

//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::showChanged()
{
    switch( _show_mode.ToInt() )
    {
		case SW_SHOW_TOPRIBBON:
		{
			this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
			this->showNormal();
			QDesktopWidget * desktop = QApplication::desktop();
			QRect screensize = desktop->screenGeometry(desktop->primaryScreen());
			this->setMinimumWidth(screensize.width());
			this->setMaximumWidth(screensize.width());
			this->move(QPoint(0,0));
		}
		break;
        case SW_SHOW_CENTERED:
        {
            this->showNormal();
            QDesktopWidget * desktop = QApplication::desktop();
            QRect screensize = desktop->screenGeometry( desktop->primaryScreen() );
            QRect windowSize = this->frameGeometry();
            QPoint slefttop( screensize.left(), screensize.top() );
            QPoint wlefttop;
            slefttop.setX( slefttop.x() + ( screensize.width() - windowSize.width() ) / 2 );
            slefttop.setY( slefttop.y() + ( screensize.height() - windowSize.height() ) / 2 );
            //_main_window->move(slefttop);
            this->move( slefttop );
        }
        break;
        case SW_SHOW_FULLSCREEN:
            this->showFullScreen();
            break;
        case SW_SHOW_MAXIMIZED:
            this->showMaximized();
            break;
        case SW_SHOW_MINIMIZED:
            this->showMinimized();
            break;
        case SW_SHOW_NORMAL:
        default:
            break;
    }
}

//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::addObserver( ISwEventObserver * obs )
{
    if( obs != nullptr )
        _iSwEvent.push_back( obs );
}

//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::removeObserver( ISwEventObserver * obs )
{
    if( obs != nullptr )
    {
        if( _iSwEvent.contains( obs ) )
            _iSwEvent.removeAt( _iSwEvent.indexOf( obs ) );
    }
}
//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::notify( QEvent * event )
{
	for (auto itObs : _iSwEvent)
		itObs->onEvent(event);
}

//---------------------------------------------------------------------------------
void _SwGuiCompQMainWindow::processCommand( QString name )
{
    if( name == TOGGLE_FULLSCREEN )
    {
        if( _show_mode.ToInt() == SW_SHOW_FULLSCREEN )
            _show_mode.FromInt( SW_SHOW_MAXIMIZED );
        else
            _show_mode.FromInt( SW_SHOW_FULLSCREEN );
        showChanged();
    }
}

//-----------------------------------------------------------------------


int _SwGuiCompQMainWindow::getCloseMode()
{
	return _close_mode.ToInt();
}

//-----------------------------------------------------------------------
Qt::WindowStates _SwGuiCompQMainWindow::getWindowState()
{
	return windowState();
}

//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::closeEvent( QCloseEvent * event )
{
	if (_close_mode.ToInt() == SW_CLOSE_HIDE && windowState() != Qt::WindowState::WindowMinimized)
	{
		setWindowState(Qt::WindowState::WindowMinimized);
		event->ignore();
		return;
	}

    if( _protectClosing )
    {
        int ret = QMessageBox::question( this, "Exit", "Do you really want to exit?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );
        if( ret == QMessageBox::No )
            event->ignore();
    }	

	saveStateGeometry();
    
    notify( ( QEvent * ) event );
}

//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::saveStateGeometry(const QString& pathTo) const
{
	if (!pathTo.isEmpty())
	{
		SwFileDescriptor fd;
		fd.setFileName(pathTo);
		QSettings settings(fd.getDoubleDottedPath(), QSettings::IniFormat);
		settings.setValue("geometry", saveGeometry());
		settings.setValue("windowState", saveState());
	}
}

//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::restoreStateGeometry(const QString& pathFrom)
{
	if (!pathFrom.isEmpty())
	{
		SwFileDescriptor fd;
		fd.setFileName(pathFrom);
		QSettings settings(fd.getDoubleDottedPath(), QSettings::IniFormat);
		restoreGeometry(settings.value("geometry").toByteArray());
		restoreState(settings.value("windowState").toByteArray());
	}
}

//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::saveStateGeometry()
{
	if (_save_geometry_ini_file && !_geometryPath.isEmpty())
	{
		SwFileDescriptor fd;
		fd.setFileName(_geometryPath);
		QSettings settings(fd.getDoubleDottedPath(), QSettings::IniFormat);
		settings.setValue("geometry", saveGeometry());
		settings.setValue("windowState", saveState());
	}
}

//-----------------------------------------------------------------------
void _SwGuiCompQMainWindow::restoreStateGeometry()
{
	if (_firstTimeRestore)
	{
		SwFileDescriptor fd;
		fd.setFileName(_geometryPath);
		QSettings settings(fd.getDoubleDottedPath(), QSettings::IniFormat);
		restoreGeometry(settings.value("geometry").toByteArray());
		restoreState(settings.value("windowState").toByteArray());

		_firstTimeRestore = false;
	}
}
