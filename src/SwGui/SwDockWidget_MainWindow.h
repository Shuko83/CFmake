#ifndef _SwGuiCompSwMainWindow_H
#define _SwGuiCompSwMainWindow_H

#include <QtGui/QMainWindow>
#include "ui_SwDockWidget_MainWindow.h"
#include "SwDockWidget_MainArea.h"

class SwDockWidget_MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	SwDockWidget_MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SwDockWidget_MainWindow();

	void setMainWidget(QWidget * widget);

	void addDockWidget(SwDockWidget_DockWidget * dock);
	void removeDockWidget(SwDockWidget_DockWidget * dock);

	void loadConfiguration();

private:
	Ui::mainWindow ui;

	SwDockWidget_MainArea *_dockArea;
};

#endif // TESTDOCK_H
