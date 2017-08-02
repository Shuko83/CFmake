#ifndef _SwGuiCompSwMainWindow_H
#define _SwGuiCompSwMainWindow_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

class SwDockWidget_MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	SwDockWidget_MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~SwDockWidget_MainWindow();

	void saveConfiguration();
	void restoreConfiguration();
	void loadConfiguration(QString filePath);

	void setMainWidget(QWidget * widget, bool quitOnClose);
	QWidget * getMainWidget();

	void addDockWidget(QWidget * dock, QString menuName = "");
	void removeDockWidget(QWidget * dock);
	void removeMenu(QString name);

	void setSaveAutoPeriod(unsigned int value);

	QString getConfigurationPathFileName(){return _configurationPathFileName;}
	void setConfigurationPathFileName(QString pathName){_configurationPathFileName = pathName;}

private:
	QWidget * _mainArea;

	QString _configurationPathFileName;
};

#endif
