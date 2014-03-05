#ifndef TOOLBARITEM_H
#define TOOLBARITEM_H

#include <QToolButton>

class SwDockWidget_ToolBarItem : public QToolButton
{
	Q_OBJECT

public:
	SwDockWidget_ToolBarItem(QWidget * dock, QString title, QWidget * toolBar);
	SwDockWidget_ToolBarItem(SwDockWidget_ToolBarItem * item);
	~SwDockWidget_ToolBarItem();

	QWidget * getToolBar();
	void setToolBar(QWidget * toolBar);

	QWidget * getWidget();

	QWidget * getMovingItem();
	void setMovingItem(QWidget * widget);

	void hideWidget(bool effect = true);

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

private:
	QWidget * _widget;
	QWidget * _toolBar;

	QWidget * _tempMovingItem;

	//Parametres de deplacement
	bool _canMove;
	bool _isMoving;
	QPoint _clickPos; //Position du curseur lors du clic

};






#endif //TOOLBARITEM_H