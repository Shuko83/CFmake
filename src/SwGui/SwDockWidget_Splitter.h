#ifndef DOCKSPLITTER_H
#define DOCKSPLITTER_H

#include <QtGui/QSplitter>

#include "SwDockWidget_DockWidget.h"

class SwDockWidget_Splitter : public QSplitter
{
	Q_OBJECT

public:
	SwDockWidget_Splitter(Qt::Orientation orientation, QWidget *parent = 0);
	SwDockWidget_Splitter(QWidget *parent = 0);
	~SwDockWidget_Splitter();

	void fill(QWidget * widget1, QWidget * widget2, Qt::DockWidgetArea area, Qt::DockWidgetArea parentArea = Qt::NoDockWidgetArea);
	void addWidget(QWidget * widget, int pos, Qt::DockWidgetArea area = Qt::NoDockWidgetArea);
	void replaceWidget(QWidget * widget, int pos);

	void updateSizes(QSize size1, QSize size2);

	void moveSplitter(int pos);

protected slots:
	virtual void resizeEvent( QResizeEvent * r );
	void childrenResized();

private:
	QWidget * _widget1;
	QSize _widget1_Size;
	QWidget * _widget2;
	QSize _widget2_Size;
};

















#endif