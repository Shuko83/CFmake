#ifndef SWDOCKWIDGET_TOOLBARITEM_H
#define SWDOCKWIDGET_TOOLBARITEM_H

#include <QToolButton>

class SwDockWidget_ToolBarItem : public QToolButton
{
	Q_OBJECT

public:
	SwDockWidget_ToolBarItem(QWidget * dock, QString title, QWidget * toolBar, QSize size);
	SwDockWidget_ToolBarItem(SwDockWidget_ToolBarItem * item);
	~SwDockWidget_ToolBarItem();

	QWidget * getToolBar();
	void setToolBar(QWidget * toolBar);

	QWidget * getWidget();

	QWidget * getMovingItem();
	void setMovingItem(QWidget * widget);

	void hideWidget(bool effect = true);
	void setOrientation(Qt::Orientation orientation);
	QSize getTitleBarSize();
	void updateWidgetPosition();

signals:
	void isMoving(QPoint pos);
	void stopMoving();

protected:
	//Evenements
	virtual void mousePressEvent( QMouseEvent * event );
	virtual void mouseReleaseEvent( QMouseEvent * event );
	virtual void mouseMoveEvent( QMouseEvent * event );

private:
	void showWidget();
	QPoint setPosition();

private:
	QWidget * _widget;
	QWidget * _toolBar;
	Qt::Orientation _orientation;
	QSize _titleBarSize;

	QWidget * _tempMovingItem;

	//Parametres de deplacement
	bool _canMove;
	bool _isMoving;
	QPoint _clickPos; //Position du curseur lors du clic

};

#endif