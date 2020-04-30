#ifndef SWDOCKWIDGET_TABWIDGET_H
#define SWDOCKWIDGET_TABWIDGET_H

#include <QtWidgets>

#include <QTabWidget>
#include "SwDockWidget_DockWidget.h"

class SwDockWidget_TabWidget : public QTabWidget
{
	Q_OBJECT

public:
	SwDockWidget_TabWidget(QWidget *parent = 0);
	~SwDockWidget_TabWidget();

	int addTab(QWidget * widget, int pos = -1);

protected:
	virtual bool event( QEvent * event );

private:
	void setSize();
};

#endif
