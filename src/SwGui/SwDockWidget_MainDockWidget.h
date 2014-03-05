#ifndef MAINDOCKWIDGET_H
#define MAINDOCKWIDGET_H

#include "SwDockWidget_DockWidget.h"
#include "SwDockWidget_MainTabWidget.h"

#include <QTabWidget>

class SwDockWidget_MainDockWidget : public SwDockWidget_DockWidget
{
	Q_OBJECT

public:
	SwDockWidget_MainDockWidget(QWidget * parent = 0, QString name = "", bool withAddButton = true);
	~SwDockWidget_MainDockWidget();

	void addWidget(QWidget * widget, QString name = "", QSize * size = 0);
	void insertWidget(int index, QWidget * widget);
	void removeWidget(int index);
	void setMoving(bool state);

	QWidget * getWidget(int index);
	QWidget * getTabWidget();
	QString tabText(int index);
	void setCurrentWidget(int index);
	int currentIndex();
	virtual int count();
	bool empty();
	bool isCurrentEmpty();

	virtual bool event( QEvent * event );

public slots:
	void updateContents(/*bool light = false*/);

signals:
	void moveTabRequested(QPoint pos);
	void freeTabRequested(int index, QPoint pos);
	void stopMovingTabRequested();

private:
	SwDockWidget_MainTabWidget * _tab;
	QString _styleSheet;
};







#endif
