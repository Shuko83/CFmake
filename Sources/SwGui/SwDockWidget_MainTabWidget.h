#ifndef SWDOCKWIDGET_MAINTABWIDGET_H
#define SWDOCKWIDGET_MAINTABWIDGET_H

#include "SwDockWidget_TabWidget.h"

class SwDockWidget_MainTabWidget : public SwDockWidget_TabWidget
{
	Q_OBJECT

public:
	SwDockWidget_MainTabWidget(QWidget * parent = 0, bool withAddButton = true, Qt::DockWidgetArea area = Qt::NoDockWidgetArea);
	~SwDockWidget_MainTabWidget();

	int count();
	QWidget * getTabBar();

	void lock();
	void releaseLock();

	void clear();

signals:
	void moveTabRequested(int index, Qt::DockWidgetArea area);
	//void freeTabRequested(int index, QPoint pos);
	//void stopMovingTabRequested();

protected slots:
	void closeTab(int index);
	void updateCurrent(int index);
	void insertNewTab();
	void createAddTabWidget();

private:
	bool _lock;
	QWidget * _addWidget;
	Qt::DockWidgetArea _area;
};

#endif