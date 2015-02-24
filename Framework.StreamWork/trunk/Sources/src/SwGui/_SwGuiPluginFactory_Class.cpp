/*!
 \file _SwGuiPluginFactory_Class.cpp
 \brief Factory de gui de base
 \date 23-ao�t-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#include <QRegExp>

#include "SwApplication.h"
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

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwGuiPluginFactory_Class::_SwGuiPluginFactory_Class():SwPluginFactory_Class() {

}
/*! \brief Destructeur */
_SwGuiPluginFactory_Class::~_SwGuiPluginFactory_Class() {

}
/*! \brief Initialisation */
void _SwGuiPluginFactory_Class::Initialize() {
    RegisterComponent("SwGuiMainWindow","Advanced Main application window (SwMainWindow)");
	RegisterComponent("SwGuiQMainWindow","Main application window (QMainWindow)");
    RegisterComponent("SwGuiMenu","Simple menu (QMenu)");       
	RegisterComponent("SwGuiActionList","Simple action list (Actions)");
	RegisterComponent("SwGuiActionProvider","Simple action provider");
    RegisterComponent("SwGuiToolBar","Simple toolbar (QToolBar)");
    RegisterComponent("SwGuiQDockWidget","Simple dock widget (QDockWidget)");
	RegisterComponent("SwGuiDockWidget","Advanced Dock widget (SwDockWidget)");
	RegisterComponent("SwGuiDockToQDock","Convert DockWidget into QDockWidget");
	RegisterComponent("SwGuiQDockToDock","Convert QDockWidget into DockWidget");
	RegisterComponent("SwGuiListDockWidget","List of Advanced Dock widget (SwDockWidget)");
    RegisterComponent("SwGuiWidget","Simple widget (QWidget)");
	RegisterComponent("SwGuiFrame","Simple frame (QFrame)");
	RegisterComponent("SwGuiScrollArea","Simple scrollArea (QScrollArea)");
    RegisterComponent("SwGuiLabel","Simple label (QLabel)");
    RegisterComponent("SwGuiGroupBox","Simple group box (QGroupBox)");
    RegisterComponent("SwGuiTabWidget","Simple tabwidget (QTabWidget)");
	RegisterComponent("SwGuiSplitterWidget","Simple splitter (QSplitter)");
    RegisterComponent("SwGuiPluginsTree","Available plugins tree view (acces by QAction)");
    RegisterComponent("SwGuiVerticalLayout","Vertical layout (use QVBoxLayout)");
    RegisterComponent("SwGuiHorizontalLayout","Horizontal layout (use QHBoxLayout)");
    RegisterComponent("SwGuiTreeView","Tree view (use QTreeView)");
    RegisterComponent("SwGuiCompToPropertiesModel","Provide an editable QAbstractItemModel from properties of component");
    RegisterComponent("SwGuiHttpServer","Tiny Http Server");
    RegisterComponent("SwGuiVisualLogConsole","Console logger");
    RegisterComponent("SwGuiVisualUpdater","Visual http updater");
    RegisterComponent("SwPerspectivesManager","PerspectiveManager");
    RegisterComponent("SwBasicPerspective","BasicPerspective");
    RegisterComponent("SwGuiGridLayout","Grid layout (use QGridLayout)");
	RegisterComponent("SwGuiCssEditor","Css Editor");
	RegisterComponent("SwGuiConsole","Provide a console to display stdout");
	RegisterComponent("SwGuiStackedWidget","Simple StackedWidget (QStackedWidget)");
	RegisterComponent("SwGuiQActionToStackedWidget","Interface de mapping des QActions sur les index d'un QStackedWidget");
	RegisterComponent("SwGuiCompFromQActionToStackedWidget","Interface de mapping des QActions sur les index d'un QStackedWidget");
	RegisterComponent("SwGuiQActionToWidget","Produit un ISwAction permettant de cacher ou d'afficher la widget");
	RegisterComponent("SwGuiQActionConsumedToWidget","Consomme un ISwAction permettant de cacher ou d'afficher la widget");
	RegisterComponent("SwGuiCompPushButtonToQAction","Produit un button widget qui permet d'exécuter des actions");
	RegisterComponent("SwEventToPopup","Popup sur SwEvent");
	RegisterComponent("SwActivationToQAction","Permet de d'activer un composant via une QAction");

}
/*! \brief Liberation */
void _SwGuiPluginFactory_Class::Liberate() {

}
/*! \brief instanciation d'un composant */
SwComponent_Class * _SwGuiPluginFactory_Class::CreateInstanceOf(QString name) {
    if (name=="SwGuiQMainWindow") {
        return new _SwGuiCompQMainWindow;
    }
	if (name=="SwGuiMainWindow") {
        return new _SwGuiCompMainWindow;
    }
    if (name=="SwGuiMenu") {
        return new _SwGuiCompMenu;
    } 
    if (name=="SwGuiActionList") {
        return new _SwGuiCompActionList;
	} 
	if (name=="SwGuiActionProvider") {
		return new _SwGuiCompActionProvider;
	}
    if (name=="SwGuiToolBar") {
        return new _SwGuiCompToolBar;
    }
    if (name=="SwGuiQDockWidget") {
        return new _SwGuiCompQDockWidget;
    }
    if (name=="SwGuiDockWidget") {
        return new _SwGuiCompDockWidget;
    }
	if (name=="SwGuiDockToQDock") {
		return new _SwGuiCompDockToQDock;
	}
	if (name=="SwGuiQDockToDock") {
		return new _SwGuiCompQDockToDock;
	}
    if (name=="SwGuiListDockWidget") {
        return new _SwGuiCompListDockWidget;
    }
    if (name=="SwGuiWidget") {
        return new _SwGuiCompWidget;
    }
    if (name=="SwGuiFrame") {
        return new _SwGuiCompFrame;
    }
	if (name=="SwGuiScrollArea"){
		return new _SwGuiCompScrollArea;
	}
    if (name=="SwGuiLabel") {
        return new _SwGuiCompLabel;
    }
    if (name=="SwGuiGroupBox") {
        return new _SwGuiCompGroupBox;
    }
    if (name=="SwGuiTabWidget") {
        return new _SwGuiCompTabWidget;
    }
	if (name=="SwGuiSplitterWidget") {
        return new _SwGuiCompSplitterWidget;
    }
    if (name=="SwGuiPluginsTree") {
        return new _SwGuiCompPluginsTree;
    }
    if (name=="SwGuiVerticalLayout") {
        return new _SwGuiCompVBoxLayout;
    }
    if (name=="SwGuiHorizontalLayout") {
        return new _SwGuiCompHBoxLayout;
    }
    if (name=="SwGuiTreeView") {
        return new _SwGuiCompTreeView;
    }
    if (name=="SwGuiCompToPropertiesModel") {
        return new _SwGuiCompToPropertiesModel;
    }
    /*if (name=="SwGuiHttpServer") {
        return new _SwGuiCompHttpServer;
    }*/
    /*if (name=="SwGuiVisualLogConsole") {
        return new _SwGuiVisualLogConsole;
    }*/
    /*if (name=="SwGuiVisualUpdater") {
        return new _SwGuiVisualUpdater;
    }*/
    if (name=="SwPerspectivesManager") {
        return new _SwPerspectivesManager;
    }
    if (name=="SwBasicPerspective") {
        return new _SwBasicPerspective;
    }
    if (name=="SwGuiGridLayout") {
        return new _SwGuiCompGridLayout;
    }
    if (name=="SwGuiCssEditor") {
        return new _SwGuiCssEditor;
    }
	if (name=="SwGuiConsole") {
		return new _SwGuiConsole;
	}
	if (name=="SwGuiStackedWidget") {
		return new _SwGuiCompStackedWidget;
	}
	if (name=="SwGuiQActionToStackedWidget") {
		return new _SwGuiCompQActionToStackedWidget;
	}
	if (name=="SwGuiCompFromQActionToStackedWidget") {
		return new _SwGuiCompFromQActionToStackedWidget;
	}
	if (name=="SwGuiQActionToWidget") {
		return new _SwGuiQActionToWidget;
	}
	if (name=="SwGuiQActionConsumedToWidget") {
		return new _SwGuiQActionConsumedToWidget;
	}
	if (name=="SwGuiCompPushButtonToQAction") {
		return new _SwGuiCompPushButtonToQAction;
	}
	if (name=="SwEventToPopup") {
		return new SwEventToPopup;
	}
	if (name=="SwActivationToQAction") {
		return new SwActivationToQAction;
	}
    return NULL;
}
/*! \brief acces a l'icone d'un composant
\param[in] name nom du composant
\return l'incone ou icone vide si non trouvé*/
QIcon _SwGuiPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
	if (name=="SwGuiQMainWindow") {
        return QIcon(":/SwGui/mainwindow.png");
    }
    if (name=="SwGuiMainWindow") {
        return QIcon(":/SwGui/mainwindow.png");
    }
    if (name=="SwGuiMenu") {
        return QIcon(":/SwGui/menu.png");
    }   
    if (name=="SwGuiActionList") {
        return QIcon(":/SwGui/menu.png");
	}
	if (name=="SwGuiActionProvider") {
		return QIcon(":/SwGui/menu.png");
	}
    if (name=="SwGuiToolBar") {
        return QIcon(":/SwGui/toolbar.png");
    }
    if (name=="SwGuiQDockWidget") {
        return QIcon(":/SwGui/widget.png");
    }
    if (name=="SwGuiDockWidget") {
        return QIcon(":/SwGui/widget.png");
    }
	if (name=="SwGuiDockToQDock") {
		return QIcon(":/SwGui/widget.png");
	}
	if (name=="SwGuiQDockToDock") {
		return QIcon(":/SwGui/widget.png");
	}
    if (name=="SwGuiListDockWidget") {
        return QIcon(":/SwGui/widget.png");
    }
    if (name=="SwGuiWidget") {
        return QIcon(":/SwGui/widget.png");
    }
    if (name=="SwGuiFrame") {
        return QIcon(":/SwGui/widget.png");
	}
	if (name=="SwGuiScrollArea") {
		return QIcon(":/SwGui/widget.png");
	}
    if (name=="SwGuiLabel") {
        return QIcon(":/SwGui/widget.png");
    }
    if (name=="SwGuiGroupBox") {
        return QIcon(":/SwGui/widget.png");
    }
    if (name=="SwGuiTabWidget") {
        return QIcon(":/SwGui/widget.png");
    }
	if (name=="SwGuiSplitterWidget") {
        return QIcon(":/SwGui/widget.png");
    }
    if (name=="SwGuiPluginsTree") {
        return QIcon(":/SwGui/plugins.png");
    }
    if (name=="SwGuiVerticalLayout") {
        return QIcon(":/SwGui/vlayout.png");
    }
    if (name=="SwGuiHorizontalLayout") {
        return QIcon(":/SwGui/hlayout.png");
    }
    if (name=="SwGuiTreeView") {
        return QIcon(":/SwGui/listview.png");
    }
    if (name=="SwGuiCompToPropertiesModel") {
        return QIcon(":/SwGui/modellistview.png");
    }
    if (name=="SwGuiHttpServer") {
        return QIcon(":/SwGui/blueMarble.png");
    }
    if (name=="SwGuiVisualLogConsole") {
        return QIcon(":/SwGui/console.png");
    }
	if (name=="SwGuiVisualUpdater") {
		return QIcon(":/SwGui/update.png");
	}
    if (name=="SwPerspectivesManager") {
        return QIcon(":/SwGui/BurnableFolder.png");
    }
    if (name=="SwBasicPerspective") {
        return QIcon(":/SwGui/Burn.png");
    }
    if (name=="SwGuiGridLayout") {
        return QIcon(":/SwGui/gridlayout.png");
    }
    if (name=="SwGuiCssEditor") {
       return QIcon(":/SwGui/editCSS.png");
	} 
	if (name=="SwGuiConsole") {
		return QIcon(":/SwGui/consoleWindows.png");
	}
	if (name=="SwGuiStackedWidget") {
		return QIcon(":/SwGui/widgetstack.png");
	}
	if (name=="SwGuiQActionToStackedWidget") {
		return QIcon(":/SwGui/widgetstack.png");
	}
	if (name=="SwGuiCompFromQActionToStackedWidget") {
		return QIcon(":/SwGui/widgetstack.png");
	}
	if (name=="SwGuiQActionToWidget") {
		return QIcon(":/SwGui/widget.png");
	}
	if (name=="SwGuiQActionConsumedToWidget") {
		return QIcon(":/SwGui/widget.png");
	}
	if (name=="SwGuiCompPushButtonToQAction") {
		return QIcon(":/SwGui/widget.png");
	}
	if (name=="SwEventToPopup") {
		return ico;
	}
    return ico;
}
/*! \brief Acces a la version du plugin
\return version*/
QString _SwGuiPluginFactory_Class::GetPluginVersion(){
    QString build;

    build=QString("%1 %2").arg(__DATE__).arg(__TIME__);
    build=build.toUpper();
    build.replace(QRegExp("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
    build+=" (Debug)";
#else
    build+=" (Release)";
#endif
    return build;
}



//---------------------------------------------------------------------
// Interface ISwServicesManager_Listener
//---------------------------------------------------------------------
/*! \brief sur ajout d'un service */
void _SwGuiPluginFactory_Class::OnRegisterService(ISwService * service) 
{
    ISwServiceExtensions * eservice=dynamic_cast< ISwServiceExtensions *>(service);
    if (eservice!=0) {
        eservice->registerExtension<ISwPerspective>("Perspective","SwBasicPerspective");
        eservice->registerExtension<ISwActionList>("ActionList","SwGuiActionList");
        eservice->registerExtension<QAbstractItemModel>("Model","SwGuiCompToPropertiesModel");
        eservice->registerExtension<ISwQAbstractItemViewSlots>("ViewSlots","SwGuiCompToPropertiesModel");
        eservice->registerExtension<ISwQDockWidget>("QDockWidget","SwGuiQDockWidget");
		eservice->registerExtension<ISwDockWidget>("DockWidget","SwGuiDockWidget");
		eservice->registerExtension<ISwListDockWidget>("ListDockWidget","SwGuiListDockWidget");
		eservice->registerExtension<ISwWidget>("Widget","SwGuiFrame");
		eservice->registerExtension<ISwWidget>("Widget","SwGuiScrollArea");
        eservice->registerExtension<ISwLayout>("GridLayout","SwGuiGridLayout");
        eservice->registerExtension<ISwWidget>("Widget","SwGuiGroupBox");
        eservice->registerExtension<ISwLayout>("Layout","SwGuiHorizontalLayout");
        eservice->registerExtension<ISwHttpServer>("ISwHttpServer","SwGuiHttpServer");
        eservice->registerExtension<ISwWidget>("Widget","SwGuiLabel");
        eservice->registerExtension<ISwQMainWindow>("QMainWindow","SwGuiQMainWindow");
		eservice->registerExtension<ISwMainWindow>("MainWindow","SwGuiMainWindow");
        eservice->registerExtension<ISwMenu>("Menu","SwGuiMenu");
        eservice->registerExtension<ISwAction>("Action","SwGuiPluginsTree");
        eservice->registerExtension<ISwAction>("Action","SwGuiQActionToWidget");
		eservice->registerExtension<ISwAction>("Action","SwGuiQActionConsumedToWidget");
        eservice->registerExtension<ISwWidget>("Widget","SwGuiSplitterWidget");
        eservice->registerExtension<ISwWidget>("Widget","SwGuiStackedWidget");
        eservice->registerExtension<ISwStackedWidget_Controler>("StackedWidget_Controler","SwGuiStackedWidget");
        eservice->registerExtension<ISwWidget>("Widget","SwGuiTabWidget");
        eservice->registerExtension<ISwToolBar>("ToolBar","SwGuiToolBar");
        eservice->registerExtension<ISwWidget>("Widget","SwGuiTreeView");
        eservice->registerExtension<ISwLayout>("Layout","SwGuiVerticalLayout");
        eservice->registerExtension<ISwWidget>("logConsole","SwGuiVisualLogConsole");
        eservice->registerExtension<ISwWidget>("Widget","SwGuiWidget");
        eservice->registerExtension<ISwWidget>("Widget","SwPerspectivesManager");
		eservice->registerExtension<ISwWidget>("Widget","SwGuiCompPushButtonToQAction");
    }
}
/*! \brief sur suppression d'une  interface */
void _SwGuiPluginFactory_Class::OnUnregisterService(ISwService * service) 
{

}

//-------------------------------------------------------------------------
void _SwGuiPluginFactory_Class::FinalizeInitialisation()
{
}

double _SwGuiPluginFactory_Class::GetPluginCompilationDate(){	
	//recup de la date à partir de __DATE__
	QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));

	//recup de l'heure à partir de __TIME__
	QString strTime = __TIME__;
	QTime time = time.fromString(strTime,"hh:mm:ss");

	QDateTime finalDateTime;
	//set la date dans QDateTime
	finalDateTime.setDate(date);
	//set le time dans QDateTime
	finalDateTime.setTime(time);

	return (finalDateTime.toMSecsSinceEpoch() / 1000.0);//retourne la date en secondes depuis le 1er janvier 1970

}