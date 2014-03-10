/*!
 \file _SwGuiCompMainWindow.h
 \brief Implementation of the Class _SwGuiMainWindow generant une SwDockWidget_MainWindow
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef ___SwGuiCompMainWindow_H
#define ___SwGuiCompMainWindow_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <Qt>

#include "SwAssistedComponent.h"
#include <Component.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include <SwEnum.h>
#include "ISwMenu.h"
#include "ISwAction.h"
#include "ISwToolBar.h"
#include "ISwDockWidget.h"
#include "ISwListDockWidget.h"
#include "ISwWidget.h"
//#include "ISwFinalizer.h"
#include "ISwListDockWidgetListener.h"


/*
  * INCLUDES LOCAUX
  */
#include "ISwMainWindow.h"
#include "ISwEvent.h"
#include "ISwEventObserver.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
using namespace StreamWork::SwFoundation;

/*!
	\class _SwGuiCompMainWindow 
	\brief _SwGuiMainWindow generant une QMainWindow
*/
class _SwGuiCompMainWindow : public SwAssistedComponent,
							 //public Component, 
							 public ISwMainWindow,
							 public ISwWidget,
							 public ISwEvent,
							 public ISwFinalizer,
							 public ISwListDockWidgetListener
{

	//Q_OBJECT
	//Q_PROPERTY(bool _lock READ getLock WRITE setLock)

protected:
    /* fenetre principale */
	SwDockWidget_MainWindow * _mainWindow;

    /* show */
    SwEnum _show_mode;
    ISwProperty * _show_property;
    
	// --- Menus ---
    /* nombre de menus */
    uint _menus_nb;
    /* propriété nombre de menu */
    ISwProperty * _menus_nb_property;
    /* map des interfaces menus*/
    QMap<QString,ISwMenu *> _menus;
    /* handle temporaire d'interface menus*/
    //ISwMenu * _tmp_handle_menu;
    
	// --- Actions ---
    /* nombre d'actions */
    uint _actions_nb;
    /* propriété nombre d'actions*/
    ISwProperty * _actions_nb_property;
    /* map des interfaces actions*/
    QMap<QString,ISwAction *> _actions;
    /* handle temporaire d'interface menus*/
    //ISwAction * _tmp_handle_action;
    
	// --- ToolBars ---
    /* default toolbar position */
    SwEnum _default_toolbar_position;
    /* nombre de toolbars */
    uint _toolbars_nb;
    /* propriété nombre de toolbars */
    ISwProperty * _toolbars_nb_property;
    /* map des interfaces toolbars*/
    QMap<QString,ISwToolBar *> _toolbars;
    /* map des properties positions toolbars*/
    QMap<QString,ISwProperty *> _toolbar_positions;
    /* handle temporaire d'interface toolbar*/
    //ISwToolBar * _tmp_handle_toolbar;
    
	// --- DockWidgets ---
    /* default dockwidget position */
    //SwEnum _default_dockwidget_position;
    /* nombre de dockwidgets */
    uint _dockwidgets_nb;
    /* propriété nombre de dockwidgets */
    ISwProperty * _dockwidgets_nb_property;
    /* map des interfaces dockwidgets*/
    QMap<QString,ISwDockWidget *> _dockwidgets;
	/* nombre de listes de dockwidgets */
    uint _listdockwidgets_nb;
    /* propriété nombre de listes de dockwidgets */
    ISwProperty * _listdockwidgets_nb_property;
	/* map des liste des docks widgets*/
	QMap<QString, ISwListDockWidget *> _listdockwidgets;
    /* map des properties positions dockwidgets*/
    //QMap<QString,ISwProperty *> _dockwidget_positions;
    /* handle temporaire d'interface dockwidget*/
    //ISwDockWidget * _tmp_handle_dockwidget;
    
	// --- Central Widget ---
    /* widget central */
    //ISwWidget * _handle_central_widget;
    /* choix du type d'interface Widget ou MainWindow*/
    bool  _useAsWidget;
    /* propriété nombre d'actions*/
    ISwProperty * _use_aswidget_property;

	//Status Bar

	QList<ISwEventObserver*> _iSwEvent;

	//bool getLock();
	//void setLock(bool lock);

public:
    /*! \brief Constructeur */
    _SwGuiCompMainWindow();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompMainWindow();

    /*! \brief Initialisation du composant */
    virtual void initializeComponent() throw(SwException);
     /*! \brief Callback sur les changements de propriétés*/
    void eventPropertyChange(ISwProperty * property);
    
    //---------------------------------------------------------------------
    // Interface ISwQMainWindow
    //---------------------------------------------------------------------
    /*! \brief Renvoie le nom du service
    \return le nom du service */
	virtual SwDockWidget_MainWindow & GetMainWindow();

	//---------------------------------------------------------------------
    // Interface ISwWidget
    //---------------------------------------------------------------------
	/*! \brief Renvoie le widget
	\return le widget */
	virtual QWidget & GetWidget();

    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
	/*! \brief Avant changement de la disponibilité de l'interface */
	virtual void eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);            
	/*! \brief Apres changement de la disponibilité de l'interface */
	virtual void eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);

	//---------------------------------------------------------------------
	// Interface ISwEvent
	//---------------------------------------------------------------------
	void addObserver(ISwEventObserver * obs);
	void removeObserver(ISwEventObserver * obs);
	void notify(QEvent * event);

	//---------------------------------------------------------------------
	// Interface ISwFinalizer
	//---------------------------------------------------------------------
	virtual bool Finalize( quint64 historic_index );

	//----------------------------------------------------------------------------
	// Interface ISwListDockWidgetListener
	//----------------------------------------------------------------------------
	virtual void addDockWidget(ISwDockWidget * widget);
	virtual void removeDockWidget(ISwDockWidget *widget);

	//---------------------------------------------------------------------
	// Gestion des Propriétés
	//---------------------------------------------------------------------
	/**
	 * @brief    : Callback appelée lors de la disponibilité de l'interface
	 * @param	 : QString interfaceName - Nom de l'interface
	 */
	virtual void interfaceAvailable(QString interfaceName);

	/**
	 * @brief    : Callback appelée lors de l'indisponibilité de l'interface
	 * @param	 : QString interfaceName - Nom de l'interface
	 */
	virtual void interfaceUnavailable(QString interfaceName);

private:
    void showChanged();

protected:
	void closeEvent(QCloseEvent* event);

/*private:
	bool _lock;*/
};
#endif 
