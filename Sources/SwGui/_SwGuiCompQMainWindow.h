/*!
 \file _SwGuiCompQMainWindow.h
 \brief Implementation of the Class _SwGuiMainWindow generant une QMainWindow
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
 */

#ifndef ___SwGuiCompQMainWindow_H
#define ___SwGuiCompQMainWindow_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <Qt>
#include <QDir>
#include <Component.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include <SwEnum.h>
#include "ISwMenu.h"
#include "ISwAction.h"
#include "ISwToolBar.h"
#include "ISwQDockWidget.h"
#include "ISwWidget.h"
#include "ISwShortcut.h"
/*
  * INCLUDES LOCAUX
  */
#include "ISwQMainWindow.h"
#include "ISwEvent.h"
#include "ISwEventObserver.h"
#include "SwServiceManager_Helper.h"
#include "_SwServiceMainWindow.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
    \class _SwGuiCompQMainWindow
    \brief _SwGuiMainWindow generant une QMainWindow
    */
class _SwGuiCompQMainWindow :   public Component,
    public QMainWindow,
    public ISwQMainWindow,
    public ISwWidget,
    public ISwEvent,
    public ISwShortcut
{
protected:
    /* fenetre principale */
    //QMainWindow * _main_window;
    /* show */
    SwEnum _show_mode;
    ISwProperty * _show_property;
    // --- Menus ---
    /* nombre de menus */
    uint _menus_nb;
    /* propriété nombre de menu */
    ISwProperty * _menus_nb_property;
    /* map des interfaces menus*/
    QMap<QString, ISwMenu *> _menus;
    /* handle temporaire d'interface menus*/
    ISwMenu * _tmp_handle_menu;
    // --- Actions ---
    /* nombre d'actions */
    uint _actions_nb;
    /* propriété nombre d'actions*/
    ISwProperty * _actions_nb_property;
    /* map des interfaces actions*/
    QMap<QString, ISwAction *> _actions;
    /* handle temporaire d'interface menus*/
    ISwAction * _tmp_handle_action;
    // --- ToolBars ---
    /* default toolbar position */
    SwEnum _default_toolbar_position;
    /* nombre de toolbars */
    uint _toolbars_nb;
    /* propriété nombre de toolbars */
    ISwProperty * _toolbars_nb_property;
    /* map des interfaces toolbars*/
    QMap<QString, ISwToolBar *> _toolbars;
    /* map des properties positions toolbars*/
    QMap<QString, ISwProperty *> _toolbar_positions;
    /* handle temporaire d'interface toolbar*/
    ISwToolBar * _tmp_handle_toolbar;
    // --- DockWidgets ---
    /* default dockwidget position */
    SwEnum _default_dockwidget_position;
    /* nombre de dockwidgets */
    uint _dockwidgets_nb;
    /* propriété nombre de dockwidgets */
    ISwProperty * _dockwidgets_nb_property;
    /* map des interfaces dockwidgets*/
    QMap<QString, ISwQDockWidget *> _dockwidgets;
    /* map des properties positions dockwidgets*/
    QMap<QString, ISwProperty *> _dockwidget_positions;
    /* handle temporaire d'interface dockwidget*/
    ISwQDockWidget * _tmp_handle_dockwidget;
    // --- Central Widget ---
    /* widget central */
    ISwWidget * _handle_central_widget;
    /* choix du type d'interface Widget ou MainWindow*/
    bool _useAsWidget;
    /* propriété nombre d'actions*/
    ISwProperty * _use_aswidget_property;
    /* Est ce que la fermeture est protegée par une fenetre de dialogue*/
    bool _protectClosing;
    /* propriété associée*/
    ISwProperty * _protect_closing_property;
	/* propriété sauvegarde de la géométrie dans un fichier ini */
	bool _save_geometry_ini_file;	
	ISwProperty * _save_geometry_ini_file_property;
	/* propriété du path de la sauvegarde de la géométrie dans un fichier ini */
	QString _configPath;
	ISwProperty * _config_path_property;
	QString _geometryPath;
	/* Lors d'une fermeture: fermer ou caché */
	SwEnum _close_mode;
	ISwProperty * _close_property;
	/* Propriete des flags de la fenetre */
	SwEnum _windowFlagsEnum;
	ISwProperty * _windowFlags;
    
    QList<ISwEventObserver *> _iSwEvent;
    
    StreamWork::Service::_SwServiceMainWindow * _mainWindowService;

public:

    /*! \brief Constructeur */
    _SwGuiCompQMainWindow();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompQMainWindow();
    
    /*! \brief Initialisation du composant */
    virtual void initializeComponent() throw( SwException );
    /*! \brief Callback sur les changements de propriétés*/
    void eventPropertyChange( ISwProperty * property );
    
    //---------------------------------------------------------------------
    // Interface ISwQMainWindow
    //---------------------------------------------------------------------
    virtual QMainWindow & GetMainWindow();
	virtual int getCloseMode();
	virtual Qt::WindowStates getWindowState();
	virtual void saveStateGeometry(const QString& pathTo) const;
	virtual void restoreStateGeometry(const QString& pathFrom);

    //---------------------------------------------------------------------
    // Interface ISwWidget
    //---------------------------------------------------------------------
    virtual QWidget * GetWidget();
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
    virtual void eventBeforeInterfaceAvailability( QString interface_name, SwComponent_Class * provider_host );
    virtual void eventAfterInterfaceAvailability( QString interface_name, SwComponent_Class * provider_host );
    
    //---------------------------------------------------------------------
    // Interface ISwEvent
    //---------------------------------------------------------------------
    void addObserver( ISwEventObserver * obs );
    void removeObserver( ISwEventObserver * obs );
    void notify( QEvent * event );
    
    //---------------------------------------------------------------------
    // Interface ISwShortcut
    //---------------------------------------------------------------------
    void processCommand( QString name ) final;
    
private:
    void showChanged();
protected:
    void closeEvent( QCloseEvent * event );

	void restoreStateGeometry();
	void saveStateGeometry();

    bool _firstTimeRestore;    
};
#endif
