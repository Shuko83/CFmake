#ifndef _SwGuiCompSwMainWindow_H
#define _SwGuiCompSwMainWindow_H

#include <QtGui/QMainWindow>

class SwDockWidget_MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	SwDockWidget_MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SwDockWidget_MainWindow();

	void saveConfiguration();
	void restoreConfiguration();
	void loadConfiguration();

	void setMainWidget(QWidget * widget);
	QWidget * getMainWidget();

	void addDockWidget(QWidget * dock, QString menuName = "");
	void removeDockWidget(QWidget * dock);

	QString getConfigurationFileName();
	void setConfigurationFileName(QString name);

private:
	QWidget * _mainArea;

	QString _configurationFileName;
};

#endif
