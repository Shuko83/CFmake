#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtGui/QWidget>
#include <QTabWidget>
#include "SwDockWidget_DockWidget.h"

class SwDockWidget_TabWidget : public QTabWidget
{
	Q_OBJECT

public:
	SwDockWidget_TabWidget(QWidget *parent = 0);
	~SwDockWidget_TabWidget();

	int addTab(QWidget * widget, int pos = -1);

private:
	void setSize();
};

#endif //TABWIDGET_H
