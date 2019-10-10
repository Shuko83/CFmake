/*!
 \file _SwGuiPluginFactory_Class.cpp
 \brief Factory de gui de base
 \date 23-ao�t-2006 16:04:34
 \version 1.0
 \author F.Bighelli
 */

#include <QRegularExpression>

#include "SwMacros.h"
#include "ISwServiceExtensions.h"

#include "_SwGuiPluginFactory_Class.h"
#include "_SwGuiCompQMainWindow.h"
#include "_SwGuiCompMainWindow.h"
#include "_SwGuiCompMenu.h"
#include "_SwGuiCompToolbar.h"
#include "_SwGuiCompQDockWidget.h"
#include "_SwGuiCompDockWidget.h"
#include "_SwGuiCompDockToQDock.h"
#include "_SwGuiCompQDockToDock.h"
#include "_SwGuiCompListDockWidget.h"
#include "_SwGuiCompWidget.h"
#include "_SwGuiCompFrame.h"
#include "_SwGuiCompScrollArea.h"
#include "_SwGuiCompLabel.h"
#include "_SwGuiCompGroupBox.h"
#include "_SwGuiCompTabWidget.h"
#include "_SwGuiCompSplitterWidget.h"
#include "_SwGuiCompPluginsTree.h"
#include "_SwGuiCompVBoxLayout.h"
#include "_SwGuiCompHBoxLayout.h"
#include "_SwGuiCompTreeView.h"
#include "_SwGuiCompToPropertiesModel.h"
#include "_SwGuiCompHttpServer.h"
#include "_SwGuiVisualLogConsole.h"
#include "_SwGuiVisualUpdater.h"
#include "_SwPerspectivesManager.h"
#include "_SwBasicPerspective.h"
#include "_SwGuiCompGridLayout.h"
#include "_SwGuiCompActionList.h"
#include "_SwGuiCompActionProvider.h"
#include "_SwGuiCssEditor.h"
#include "_SwGuiConsole.h"
#include "_SwGuiCompStackedWidget.h"
#include "_SwGuiCompQActionToStackedWidget.h"
#include "_SwGuiCompFromQActionToStackedWidget.h"
#include "SwMacros.h"
#include "_SwGuiQActionToWidget.h"
#include "_SwGuiQActionConsumedToWidget.h"
#include "_SwGuiCompPushButtonToQAction.h"
#include "SwEventToPopup.h"
#include "SwActivationToQAction.h"
#include "SwActionToToolButton.h"
#include "_SwGuiCompQWidgetToQDialog.h"

using namespace StreamWork::SwCore;

//---------------------------------------------------------------------
_SwGuiPluginFactory_Class::_SwGuiPluginFactory_Class() :SwPluginFactory_Class()
{

}

//---------------------------------------------------------------------
_SwGuiPluginFactory_Class::~_SwGuiPluginFactory_Class()
{

}

//---------------------------------------------------------------------
void _SwGuiPluginFactory_Class::Initialize()
{
	RegisterComponent("SwGuiMainWindow", "Advanced Main application window (SwMainWindow)");
	RegisterComponent("SwGuiQMainWindow", "Main application window (QMainWindow)");
	RegisterComponent("SwGuiMenu", "Simple menu (QMenu)");
	RegisterComponent("SwGuiActionList", "Simple action list (Actions)");
	RegisterComponent("SwGuiActionProvider", "Simple action provider");
	RegisterComponent("SwGuiToolBar", "Simple toolbar (QToolBar)");
	RegisterComponent("SwGuiQDockWidget", "Simple dock widget (QDockWidget)");
	RegisterComponent("SwGuiDockWidget", "Advanced Dock widget (SwDockWidget)");
	RegisterComponent("SwGuiDockToQDock", "Convert DockWidget into QDockWidget");
	RegisterComponent("SwGuiQDockToDock", "Convert QDockWidget into DockWidget");
	RegisterComponent("SwGuiListDockWidget", "List of Advanced Dock widget (SwDockWidget)");
	RegisterComponent("SwGuiWidget", "Simple widget (QWidget)");
	RegisterComponent("SwGuiFrame", "Simple frame (QFrame)");
	RegisterComponent("SwGuiScrollArea", "Simple scrollArea (QScrollArea)");
	RegisterComponent("SwGuiLabel", "Simple label (QLabel)");
	RegisterComponent("SwGuiGroupBox", "Simple group box (QGroupBox)");
	RegisterComponent("SwGuiTabWidget", "Simple tabwidget (QTabWidget)");
	RegisterComponent("SwGuiSplitterWidget", "Simple splitter (QSplitter)");
	RegisterComponent("SwGuiPluginsTree", "Available plugins tree view (acces by QAction)");
	RegisterComponent("SwGuiVerticalLayout", "Vertical layout (use QVBoxLayout)");
	RegisterComponent("SwGuiHorizontalLayout", "Horizontal layout (use QHBoxLayout)");
	RegisterComponent("SwGuiTreeView", "Tree view (use QTreeView)");
	RegisterComponent("SwGuiToPropertiesModel", "Provide an editable QAbstractItemModel from properties of component");
	RegisterComponent("SwGuiHttpServer", "Tiny Http Server");
	RegisterComponent("SwGuiVisualLogConsole", "Console logger");
	RegisterComponent("SwGuiVisualUpdater", "Visual http updater");
	RegisterComponent("SwPerspectivesManager", "PerspectiveManager");
	RegisterComponent("SwBasicPerspective", "BasicPerspective");
	RegisterComponent("SwGuiGridLayout", "Grid layout (use QGridLayout)");
	RegisterComponent("SwGuiCssEditor", "Css Editor");
	RegisterComponent("SwGuiConsole", "Provide a console to display stdout");
	RegisterComponent("SwGuiStackedWidget", "Simple StackedWidget (QStackedWidget)");
	RegisterComponent("SwGuiQActionToStackedWidget", "Interface de mapping des QActions sur les index d'un QStackedWidget");
	RegisterComponent("SwGuiFromQActionToStackedWidget", "Interface de mapping des QActions sur les index d'un QStackedWidget");
	RegisterComponent("SwGuiQActionToWidget", "Produit un ISwAction permettant de cacher ou d'afficher la widget");
	RegisterComponent("SwGuiQActionConsumedToWidget", "Consomme un ISwAction permettant de cacher ou d'afficher la widget");
	RegisterComponent("SwGuiPushButtonToQAction", "Produit un button widget qui permet d'exécuter des actions");
	RegisterComponent("SwEventToPopup", "Popup sur SwEvent");
	RegisterComponent("SwActivationToQAction", "Permet de d'activer un composant via une QAction");
	RegisterComponent("SwActionToToolButton", "Permet de convertir une action en QToolButton");
	RegisterComponent("SwGuiQWidgetToQDialog", "Permet de convertir un qwidget en dialog avec gestion parent");

}
/*! \brief Liberation */
void _SwGuiPluginFactory_Class::Liberate()
{

}
/*! \brief instanciation d'un composant */
SwComponent_Class * _SwGuiPluginFactory_Class::CreateInstanceOf(QString name)
{
	SW_PUBLISH_COMPONENT("SwGuiQMainWindow", _SwGuiCompQMainWindow);

	SW_PUBLISH_COMPONENT("SwGuiMainWindow", _SwGuiCompMainWindow);
	SW_PUBLISH_COMPONENT("SwGuiMenu", _SwGuiCompMenu);
	SW_PUBLISH_COMPONENT("SwGuiActionList", _SwGuiCompActionList);
	SW_PUBLISH_COMPONENT("SwGuiActionProvider", _SwGuiCompActionProvider);
	SW_PUBLISH_COMPONENT("SwGuiToolBar", _SwGuiCompToolBar);
	SW_PUBLISH_COMPONENT("SwGuiQDockWidget", _SwGuiCompQDockWidget);
	SW_PUBLISH_COMPONENT("SwGuiDockWidget", _SwGuiCompDockWidget);
	SW_PUBLISH_COMPONENT("SwGuiDockToQDock", _SwGuiCompDockToQDock);
	SW_PUBLISH_COMPONENT("SwGuiQDockToDock", _SwGuiCompQDockToDock);
	SW_PUBLISH_COMPONENT("SwGuiListDockWidget", _SwGuiCompListDockWidget);
	SW_PUBLISH_COMPONENT("SwGuiWidget", _SwGuiCompWidget);
	SW_PUBLISH_COMPONENT("SwGuiFrame", _SwGuiCompFrame);
	SW_PUBLISH_COMPONENT("SwGuiScrollArea", _SwGuiCompScrollArea);
	SW_PUBLISH_COMPONENT("SwGuiLabel", _SwGuiCompLabel);
	SW_PUBLISH_COMPONENT("SwGuiGroupBox", _SwGuiCompGroupBox);
	SW_PUBLISH_COMPONENT("SwGuiTabWidget", _SwGuiCompTabWidget);
	SW_PUBLISH_COMPONENT("SwGuiSplitterWidget", _SwGuiCompSplitterWidget);
	SW_PUBLISH_COMPONENT("SwGuiPluginsTree", _SwGuiCompPluginsTree);
	SW_PUBLISH_COMPONENT("SwGuiVerticalLayout", _SwGuiCompVBoxLayout);
	SW_PUBLISH_COMPONENT("SwGuiHorizontalLayout", _SwGuiCompHBoxLayout);
	SW_PUBLISH_COMPONENT("SwGuiTreeView", _SwGuiCompTreeView);
	SW_PUBLISH_COMPONENT("SwGuiToPropertiesModel", _SwGuiCompToPropertiesModel);
	SW_PUBLISH_COMPONENT("SwPerspectivesManager", _SwPerspectivesManager);
	SW_PUBLISH_COMPONENT("SwBasicPerspective", _SwBasicPerspective);
	SW_PUBLISH_COMPONENT("SwGuiGridLayout", _SwGuiCompGridLayout);
	SW_PUBLISH_COMPONENT("SwGuiCssEditor", _SwGuiCssEditor);
	SW_PUBLISH_COMPONENT("SwGuiConsole", _SwGuiConsole);
	SW_PUBLISH_COMPONENT("SwGuiStackedWidget", _SwGuiCompStackedWidget);
	SW_PUBLISH_COMPONENT("SwGuiQActionToStackedWidget", _SwGuiCompQActionToStackedWidget);
	SW_PUBLISH_COMPONENT("SwGuiFromQActionToStackedWidget", _SwGuiCompFromQActionToStackedWidget);
	SW_PUBLISH_COMPONENT("SwGuiQActionToWidget", _SwGuiQActionToWidget);
	SW_PUBLISH_COMPONENT("SwGuiQActionConsumedToWidget", _SwGuiQActionConsumedToWidget);
	SW_PUBLISH_COMPONENT("SwGuiPushButtonToQAction", _SwGuiCompPushButtonToQAction);
	SW_PUBLISH_COMPONENT("SwEventToPopup", SwEventToPopup);
	SW_PUBLISH_COMPONENT("SwActivationToQAction", SwActivationToQAction);
	SW_PUBLISH_COMPONENT("SwActionToToolButton", SwActionToToolButton);
	SW_PUBLISH_COMPONENT("SwGuiQWidgetToQDialog", _SwGuiCompQWidgetToQDialog);
	
	return NULL;
}

//---------------------------------------------------------------------
QIcon _SwGuiPluginFactory_Class::CreateIconOf(QString name) const
{
	QIcon ico;
	SW_PUBLISH_ICON("SwGuiQMainWindow", ":/SwGui/mainwindow.png");
	SW_PUBLISH_ICON("SwGuiMainWindow", ":/SwGui/mainwindow.png");
	SW_PUBLISH_ICON("SwGuiMenu", ":/SwGui/menu.png");
	SW_PUBLISH_ICON("SwGuiActionList", ":/SwGui/menu.png");
	SW_PUBLISH_ICON("SwGuiActionProvider", ":/SwGui/menu.png");
	SW_PUBLISH_ICON("SwGuiToolBar", ":/SwGui/toolbar.png");
	SW_PUBLISH_ICON("SwGuiQDockWidget", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiDockWidget", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiDockToQDock", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiQDockToDock", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiListDockWidget", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiWidget", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiFrame", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiScrollArea", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiLabel", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiGroupBox", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiTabWidget", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiSplitterWidget", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiPluginsTree", ":/SwGui/plugins.png");
	SW_PUBLISH_ICON("SwGuiVerticalLayout", ":/SwGui/vlayout.png");
	SW_PUBLISH_ICON("SwGuiHorizontalLayout", ":/SwGui/hlayout.png");
	SW_PUBLISH_ICON("SwGuiTreeView", ":/SwGui/listview.png");
	SW_PUBLISH_ICON("SwGuiToPropertiesModel", ":/SwGui/modellistview.png");
	SW_PUBLISH_ICON("SwGuiHttpServer", ":/SwGui/blueMarble.png");
	SW_PUBLISH_ICON("SwGuiVisualLogConsole", ":/SwGui/console.png");
	SW_PUBLISH_ICON("SwGuiVisualUpdater", ":/SwGui/update.png");
	SW_PUBLISH_ICON("SwPerspectivesManager", ":/SwGui/BurnableFolder.png");
	SW_PUBLISH_ICON("SwBasicPerspective", ":/SwGui/Burn.png");
	SW_PUBLISH_ICON("SwGuiGridLayout", ":/SwGui/gridlayout.png");
	SW_PUBLISH_ICON("SwGuiCssEditor", ":/SwGui/editCSS.png");
	SW_PUBLISH_ICON("SwGuiConsole", ":/SwGui/consoleWindows.png");
	SW_PUBLISH_ICON("SwGuiStackedWidget", ":/SwGui/widgetstack.png");
	SW_PUBLISH_ICON("SwGuiQActionToStackedWidget", ":/SwGui/widgetstack.png");
	SW_PUBLISH_ICON("SwGuiFromQActionToStackedWidget", ":/SwGui/widgetstack.png");
	SW_PUBLISH_ICON("SwGuiQActionToWidget", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiQActionConsumedToWidget", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiPushButtonToQAction", ":/SwGui/widget.png");
	SW_PUBLISH_ICON("SwGuiQWidgetToQDialog", ":/SwGui/widget.png");
	
	return ico;
}

//---------------------------------------------------------------------
QString _SwGuiPluginFactory_Class::GetPluginVersion()
{
	QString build;

	build = QString("%1 %2").arg(__DATE__).arg(__TIME__);
	build = build.toUpper();
	build.replace(QRegularExpression("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
	build += " (Debug)";
#else
	build += " (Release)";
#endif
	return build;
}



//---------------------------------------------------------------------
void _SwGuiPluginFactory_Class::OnRegisterService(ISwService * service)
{
	ISwServiceExtensions * eservice = dynamic_cast<ISwServiceExtensions *>(service);
	if ( eservice != 0 )
	{
		eservice->registerExtension<ISwPerspective>("Perspective", "SwBasicPerspective");
		eservice->registerExtension<ISwActionList>("ActionList", "SwGuiActionList");
		eservice->registerExtension<QAbstractItemModel>("Model", "SwGuiToPropertiesModel");
		eservice->registerExtension<ISwQAbstractItemViewSlots>("ViewSlots", "SwGuiToPropertiesModel");
		eservice->registerExtension<ISwQDockWidget>("QDockWidget", "SwGuiQDockWidget");
		eservice->registerExtension<ISwDockWidget>("DockWidget", "SwGuiDockWidget");
		eservice->registerExtension<ISwListDockWidget>("ListDockWidget", "SwGuiListDockWidget");
		eservice->registerExtension<ISwWidget>("Widget", "SwGuiFrame");
		eservice->registerExtension<ISwWidget>("Widget", "SwGuiScrollArea");
		eservice->registerExtension<ISwLayout>("GridLayout", "SwGuiGridLayout");
		eservice->registerExtension<ISwWidget>("Widget", "SwGuiGroupBox");
		eservice->registerExtension<ISwLayout>("Layout", "SwGuiHorizontalLayout");
		eservice->registerExtension<ISwHttpServer>("ISwHttpServer", "SwGuiHttpServer");
		eservice->registerExtension<ISwWidget>("Widget", "SwGuiLabel");
		eservice->registerExtension<ISwQMainWindow>("QMainWindow", "SwGuiQMainWindow");
		eservice->registerExtension<ISwMainWindow>("MainWindow", "SwGuiMainWindow");
		eservice->registerExtension<ISwMenu>("Menu", "SwGuiMenu");
		eservice->registerExtension<ISwAction>("Action", "SwGuiPluginsTree");
		eservice->registerExtension<ISwAction>("Action", "SwGuiQActionToWidget");
		eservice->registerExtension<ISwAction>("Action", "SwGuiQActionConsumedToWidget");
		eservice->registerExtension<ISwWidget>("Widget", "SwGuiSplitterWidget");
		eservice->registerExtension<ISwWidget>("Widget", "SwGuiStackedWidget");
		eservice->registerExtension<ISwStackedWidget_Controler>("StackedWidget_Controler", "SwGuiStackedWidget");
		eservice->registerExtension<ISwWidget>("Widget", "SwGuiTabWidget");
		eservice->registerExtension<ISwToolBar>("ToolBar", "SwGuiToolBar");
		eservice->registerExtension<ISwWidget>("Widget", "SwGuiTreeView");
		eservice->registerExtension<ISwLayout>("Layout", "SwGuiVerticalLayout");
		eservice->registerExtension<ISwWidget>("logConsole", "SwGuiVisualLogConsole");
		eservice->registerExtension<ISwWidget>("Widget", "SwGuiWidget");
		eservice->registerExtension<ISwWidget>("Widget", "SwPerspectivesManager");
		eservice->registerExtension<ISwWidget>("Widget", "SwGuiPushButtonToQAction");
		eservice->registerExtension<ISwWidget>("Widget", "SwActionToToolButton");
	}
}

//-------------------------------------------------------------------------
void _SwGuiPluginFactory_Class::OnUnregisterService(ISwService * service)
{

}

//-------------------------------------------------------------------------
void _SwGuiPluginFactory_Class::FinalizeInitialisation()
{

}

//-------------------------------------------------------------------------
double _SwGuiPluginFactory_Class::GetPluginCompilationDate()
{
	//recup de la date à partir de __DATE__
	QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));

	//recup de l'heure à partir de __TIME__
	QString strTime = __TIME__;
	QTime time = time.fromString(strTime, "hh:mm:ss");

	QDateTime finalDateTime;
	//set la date dans QDateTime
	finalDateTime.setDate(date);
	//set le time dans QDateTime
	finalDateTime.setTime(time);

	return (finalDateTime.toMSecsSinceEpoch() / 1000.0);//retourne la date en secondes depuis le 1er janvier 1970

}

//-------------------------------------------------------------------------
SwPluginFactory_Class *  GetPluginInterface()
{
	return new _SwGuiPluginFactory_Class;
}