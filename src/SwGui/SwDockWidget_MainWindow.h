#ifndef _SwGuiCompSwMainWindow_H
#define _SwGuiCompSwMainWindow_H

#include <QtGui/QMainWindow>
//#include "ui_SwDockWidget_MainWindow.h"
//#include "SwDockWidget_MainArea.h"

class SwDockWidget_MainWindow : public QMainWindow
{
	Q_OBJECT
	Q_PROPERTY(bool _lock READ getLock WRITE setLock)

public:
	SwDockWidget_MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SwDockWidget_MainWindow();

	void setMainWidget(QWidget * widget);

	void addDockWidget(/*SwDockWidget_DockWidget*/QWidget * dock, QString menuName = "");
	void removeDockWidget(/*SwDockWidget_DockWidget*/QWidget * dock);

	void loadConfiguration();

	bool getLock();
	void setLock(bool lock);

private:
	//Ui::mainWindow ui;
	bool _lock;

	//SwDockWidget_MainArea *_dockArea;
	QWidget * _mainArea;
};

#endif // TESTDOCK_H
