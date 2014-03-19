#include "SwDockWidget_MainWindow.h"
#include "SwDockWidget_DockWidget.h"
#include "SwDockWidget_MainArea.h"

#include <QSettings>
#include <QMenuBar>

//-----------------------------------------------------------------------------
SwDockWidget_MainWindow::SwDockWidget_MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	//Chargement des parametres de la fenetre
	//QSettings settings("Diginext", "TestDock");
	//restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
	restoreConfiguration();

	//Main window
	_mainArea = new SwDockWidget_MainArea(this, menuBar());
	setCentralWidget(_mainArea);
}

//-----------------------------------------------------------------------------
SwDockWidget_MainWindow::~SwDockWidget_MainWindow()
{
	//Sauvegarde des parametres de la fenetre
	//QSettings settings("Diginext", "TestDock");
	//settings.setValue("mainWindowGeometry", saveGeometry());
	saveConfiguration();

	delete _mainArea;
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainWindow::setMainWidget(QWidget * widget)
{
	if (widget && _mainArea)
	{
		SwDockWidget_MainArea * _mainDock = qobject_cast<SwDockWidget_MainArea*>(_mainArea);
		if (_mainDock)
			_mainDock->setMainWidget(widget);
	}
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_MainWindow::getMainWidget()
{
	SwDockWidget_MainArea * _mainDock = qobject_cast<SwDockWidget_MainArea*>(_mainArea);
	if (_mainDock)
		return _mainDock->getMainWidget();
	return NULL;
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainWindow::addDockWidget(QWidget * widget, QString menuName)
{
	if (widget && _mainArea)
	{
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);
		SwDockWidget_MainArea * _mainDock = qobject_cast<SwDockWidget_MainArea*>(_mainArea);
		if (_mainDock && dock)
			_mainDock->addDockWidget(dock, menuName);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainWindow::removeDockWidget(QWidget * widget)
{
	if (widget && _mainArea)
	{
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);
		SwDockWidget_MainArea * _mainDock = qobject_cast<SwDockWidget_MainArea*>(_mainArea);
		if (_mainDock && dock)
			_mainDock->removeDockWidget(dock);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainWindow::saveConfiguration()
{
	QSettings settings("Diginext", "MainWindow");
	settings.setValue("mainWindowGeometry", saveGeometry());
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainWindow::restoreConfiguration()
{
	QSettings settings("Diginext", "MainWindow");
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainWindow::loadConfiguration()
{
	//DEBUG ONLY : suppression du chargement
	SwDockWidget_MainArea * _mainDock = qobject_cast<SwDockWidget_MainArea*>(_mainArea);
	if (_mainDock)
		_mainDock->loadDockPosition();
}

//-----------------------------------------------------------------------------
QString SwDockWidget_MainWindow::getConfigurationFileName()
{
	return _configurationFileName;
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainWindow::setConfigurationFileName(QString name, bool apply)
{
	if (_configurationFileName != name)
	{
		_configurationFileName = name;
		SwDockWidget_MainArea * _mainDock = qobject_cast<SwDockWidget_MainArea*>(_mainArea);
		if (_mainDock)
		{
			//Ajout du path complet
			QString path = QDir::homePath () + QDir::separator() +
								"AppData" + QDir::separator() +
								"Roaming" + QDir::separator() +
								"diginext" + QDir::separator() +
								"Starlinx" + QDir::separator() +
								"DockConfiguration";
			//Creation si necessaire du repertoire
			QDir lDir;
			if (!lDir.exists(path)) 
				lDir.mkpath(path);

			QString fullName = path + QDir::separator() + name;
			_mainDock->setConfigurationFileName(fullName, apply);
		}
	}
}


