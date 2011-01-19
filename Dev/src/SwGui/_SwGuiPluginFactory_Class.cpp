/*!
 \file _SwGuiPluginFactory_Class.cpp
 \brief Factory de gui de base
 \date 23-ao�t-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#include <QRegExp>

#include "_SwGuiPluginFactory_Class.h"
#include "_SwGuiCompMainWindow.h"
#include "_SwGuiCompMenu.h"
#include "_SwGuiCompToolbar.h"
#include "_SwGuiCompDockWidget.h"
#include "_SwGuiCompWidget.h"
#include "_SwGuiCompFrame.h"
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
#include "_SwGuiCssEditor.h"
#include "_SwGuiConsole.h"
#include "_SwGuiCompStackedWidget.h"
#include "_SwGuiCompQActionToStackedWidget.h"
#include "SwMacros.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*! \brief Constructeur */
_SwGuiPluginFactory_Class::_SwGuiPluginFactory_Class():SwPluginFactory_Class() {

}
/*! \brief Destructeur */
_SwGuiPluginFactory_Class::~_SwGuiPluginFactory_Class() {

}
/*! \brief Initialisation */
void _SwGuiPluginFactory_Class::Initialize() {
    RegisterComponent("SwGuiMainWindow","Main application window (QMainWindow)");
    RegisterComponent("SwGuiMenu","Simple menu (QMenu)");       
    RegisterComponent("SwGuiActionList","Simple action list (Actions)");
    RegisterComponent("SwGuiToolBar","Simple toolbar (QToolBar)");
    RegisterComponent("SwGuiDockWidget","Simple dock widget (QDockWidget)");
    RegisterComponent("SwGuiWidget","Simple widget (QWidget)");
    RegisterComponent("SwGuiFrame","Simple frame (QFrame)");
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

}
/*! \brief Liberation */
void _SwGuiPluginFactory_Class::Liberate() {

}
/*! \brief instanciation d'un composant */
SwComponent_Class * _SwGuiPluginFactory_Class::CreateInstanceOf(QString name) {
    if (name=="SwGuiMainWindow") {
        return new _SwGuiCompMainWindow;
    }
    if (name=="SwGuiMenu") {
        return new _SwGuiCompMenu;
    } 
    if (name=="SwGuiActionList") {
        return new _SwGuiCompActionList;
    }
    if (name=="SwGuiToolBar") {
        return new _SwGuiCompToolBar;
    }
    if (name=="SwGuiDockWidget") {
        return new _SwGuiCompDockWidget;
    }
    if (name=="SwGuiWidget") {
        return new _SwGuiCompWidget;
    }
    if (name=="SwGuiFrame") {
        return new _SwGuiCompFrame;
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
    if (name=="SwGuiHttpServer") {
        return new _SwGuiCompHttpServer;
    }
    if (name=="SwGuiVisualLogConsole") {
        return new _SwGuiVisualLogConsole;
    }
    if (name=="SwGuiVisualUpdater") {
        return new _SwGuiVisualUpdater;
    }
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
    return NULL;
}
/*! \brief acces a l'icone d'un composant
\param[in] name nom du composant
\return l'incone ou icone vide si non trouvé*/
QIcon _SwGuiPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
    if (name=="SwGuiMainWindow") {
        return QIcon(":/SwGui/mainwindow.png");
    }
    if (name=="SwGuiMenu") {
        return QIcon(":/SwGui/menu.png");
    }   
    if (name=="SwGuiActionList") {
        return QIcon(":/SwGui/menu.png");
    }
    if (name=="SwGuiToolBar") {
        return QIcon(":/SwGui/toolbar.png");
    }
    if (name=="SwGuiDockWidget") {
        return QIcon(":/SwGui/widget.png");
    }
    if (name=="SwGuiWidget") {
        return QIcon(":/SwGui/widget.png");
    }
    if (name=="SwGuiFrame") {
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

