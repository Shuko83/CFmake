#include "SwDockWidget_MainWindow.h"
#include "SwDockWidget_DockWidget.h"

#include <QSettings>
#include <QMenuBar>

SwDockWidget_MainWindow::SwDockWidget_MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	//Chargement des parametres de la fenetre
	QSettings settings("Diginext", "TestDock");
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

	//Main window
	_dockArea = new SwDockWidget_MainArea(this, menuBar());
	setCentralWidget(_dockArea);
}

SwDockWidget_MainWindow::~SwDockWidget_MainWindow()
{
	//Sauvegarde des parametres de la fenetre
	QSettings settings("Diginext", "TestDock");
	settings.setValue("mainWindowGeometry", saveGeometry());

	delete _dockArea;
}

void SwDockWidget_MainWindow::setMainWidget(QWidget * widget)
{
	if (widget && _dockArea)
	{
		_dockArea->setMainWidget(widget);
	}
}

void SwDockWidget_MainWindow::addDockWidget(SwDockWidget_DockWidget * dock, QString menuName)
{
	if (dock && _dockArea)
		_dockArea->addDockWidget(dock, menuName);
}

void SwDockWidget_MainWindow::removeDockWidget(SwDockWidget_DockWidget * dock)
{
	if (dock && _dockArea)
	{
		_dockArea->removeDockWidget(dock);
	}
}

void SwDockWidget_MainWindow::loadConfiguration()
{
	if (_dockArea)
	{
		_dockArea->loadDockPosition();
	}
}
