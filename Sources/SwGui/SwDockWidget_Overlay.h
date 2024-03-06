#ifndef SWDOCKWIDGET_OVERLAY_H
#define SWDOCKWIDGET_OVERLAY_H

#include <QWidget>

class SwDockWidget_Overlay : public QWidget
{
public:
	SwDockWidget_Overlay(QWidget *parent, QWidget * mainWidget, QColor color = QColor(100, 125, 151, 200));
	~SwDockWidget_Overlay();

	void setWidgets(QWidget * back, QWidget * top);
	void showOn(Qt::DockWidgetArea area, bool toMain = false);
	void setMainWidget(QWidget * mainWidget);

	void setColor(QColor color);

	QWidget * getDockWidget();

protected:
	virtual void paintEvent(QPaintEvent *event);

private:
	QWidget * _back;
	QWidget * _top;
	QWidget * _mainWidget;

	QColor _color;
};

#endif
