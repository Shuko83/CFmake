#include "SwDockWidget_MainWindow.h"
#include "SwDockWidget_DockWidget.h"
#include "SwDockWidget_MainArea.h"

#include <QSettings>
#include <QMenuBar>


SwDockWidget_MainWindow::SwDockWidget_MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)//, _lock(false)
{
	//ui.setupUi(this);

	//Chargement des parametres de la fenetre
	QSettings settings("Diginext", "TestDock");
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

	//Main window
	_mainArea = new SwDockWidget_MainArea(this, menuBar());
	setCentralWidget(_mainArea);
}

SwDockWidget_MainWindow::~SwDockWidget_MainWindow()
{
	//Sauvegarde des parametres de la fenetre
	QSettings settings("Diginext", "TestDock");
	settings.setValue("mainWindowGeometry", saveGeometry());

	delete _mainArea;
}

void SwDockWidget_MainWindow::setMainWidget(QWidget * widget)
{
	if (widget && _mainArea)
	{
		SwDockWidget_MainArea * _mainDock = qobject_cast<SwDockWidget_MainArea*>(_mainArea);
		if (_mainDock)
			_mainDock->setMainWidget(widget);
	}
}

void SwDockWidget_MainWindow::addDockWidget(/*SwDockWidget_DockWidget* dock */QWidget * widget, QString menuName)
{
	if (widget && _mainArea)
	{
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);
		SwDockWidget_MainArea * _mainDock = qobject_cast<SwDockWidget_MainArea*>(_mainArea);
		if (_mainDock && dock)
			_mainDock->addDockWidget(dock, menuName);
	}
}

void SwDockWidget_MainWindow::removeDockWidget(/*SwDockWidget_DockWidget * dock*/QWidget * widget)
{
	if (widget && _mainArea)
	{
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);
		SwDockWidget_MainArea * _mainDock = qobject_cast<SwDockWidget_MainArea*>(_mainArea);
		if (_mainDock && dock)
			_mainDock->removeDockWidget(dock);
	}
}

void SwDockWidget_MainWindow::loadConfiguration()
{
	//if (_dockArea)
	{
		SwDockWidget_MainArea * _mainDock = qobject_cast<SwDockWidget_MainArea*>(_mainArea);
		if (_mainDock)
			_mainDock->loadDockPosition();
	}
}

/*bool SwDockWidget_MainWindow::getLock()
{
	return _lock;
}

void SwDockWidget_MainWindow::setLock(bool lock)
{
	if (lock != _lock)
	{
		_lock = lock;
		SwDockWidget_MainArea * _mainDock = qobject_cast<SwDockWidget_MainArea*>(_mainArea);
		if (_mainDock)
		{
			if (lock)
				_mainDock->lock();
			else
				_mainDock->releaseLock();
		}
	}
}
*/