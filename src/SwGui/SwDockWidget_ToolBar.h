#ifndef SWDOCKWIDGET_TOOLBAR_H
#define SWDOCKWIDGET_TOOLBAR_H

#include "ui_SwDockWidget_ToolBar.h"
#include "SwDockWidget_ToolBarItem.h"
#include "SwDockWidget_Overlay.h"

#include <QtGui/QWidget>

class SwDockWidget_ToolBar : public QWidget
{
	Q_OBJECT

public:
	SwDockWidget_ToolBar(QWidget * parent = 0, QRect rect = QRect());
	~SwDockWidget_ToolBar();

	void setOrientation(Qt::Orientation orientation);
	Qt::Orientation getOrientation();

	void addItem(QWidget * widget);
	void addItem(QWidget * widget, QPoint pos);
	void removeItem(QWidget * widget);
	QWidget * takeItem(QWidget * widget);
	QWidget * itemAt(int index);

	void move(int x, int y);
	void move(const QPoint & pos);

	int indexOf(QWidget * widget);
	bool isEmpty();
	int count();

	QList<QWidget*> getListWidget();
	
	void showOverlay(QPoint pos, SwDockWidget_Overlay * overlay);
	void setMoving(bool state);
	void setMainRect(QRect rect);

	Qt::WindowFrameSection getStuckPosition();
	void setStuckPosition(Qt::WindowFrameSection section);
	void setDist(QPoint dist);
	QPoint getDist();
	QSize getTitleBarSize();

	void lock();
	void releaseLock();


signals:
	void closeToolBarAsked();
	void isMoving();
	void stopMoving();

protected:
	//Evenements
	virtual void mouseReleaseEvent( QMouseEvent * event );
	virtual void mousePressEvent( QMouseEvent * event );
	virtual void mouseMoveEvent( QMouseEvent * event );
	virtual void moveEvent ( QMoveEvent * event );
	virtual bool eventFilter( QObject *obj , QEvent * event );

private:
	void updateSize();
	void updatePosition();
	void updateOrientation();
	QPoint getAdjustedPosition(QPoint p);
	QPoint getAdjustedPosition(int x, int y);
	QPoint checkStuckPosition(int x, int y);
	void moveItem(QWidget * widget, SwDockWidget_ToolBarItem * item, QPoint pos);
	bool isInTriangle(QPoint a, QPoint b, QPoint c, QPoint p);
	int setupButtonPosition(QPoint pos, Qt::Orientation * o);
	void getTopParent();

private slots:
	void fixSize();
	void switchOrientation();

private:
	Ui::ToolBar ui;
	Qt::Orientation _orientation;
	QLayout * _layout;
	QWidget * _topParent;

	Qt::WindowFrameSection _stuck; //Aimante a un bord de la fenetre
	QRect _mainRect; //Rectangle du widget principal
	QPoint _dist; //distance de la toolbar au bord du widget principal

	bool _canMove; //Deplacement autorise
	QPoint _clickPos; //Position du curseur lors du clic

	bool _lock;
};

#endif