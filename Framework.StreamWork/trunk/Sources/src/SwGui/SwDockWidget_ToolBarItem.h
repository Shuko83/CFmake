#ifndef SWDOCKWIDGET_TOOLBARITEM_H
#define SWDOCKWIDGET_TOOLBARITEM_H

#include <QToolButton>
#include <QMenu>
#include <QLineEdit>

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

signals:
	void isMoving(QPoint pos);
	void stopMoving();
	void renameFinished();

protected slots:
	void removeAction();

	void renameAction();
	void renameTemporarily(QString name);
	void renameItem();
	void cancelNameEdit();

protected:
	//Evenements
	virtual void mousePressEvent( QMouseEvent * event );
	virtual void mouseReleaseEvent( QMouseEvent * event );
	virtual void mouseMoveEvent( QMouseEvent * event );
	virtual void keyPressEvent(QKeyEvent * event);
	virtual bool eventFilter( QObject *obj , QEvent * event );

private:
	void showWidget();
	QPoint setPosition();
	void setMenu();
	void connectNameEdit();
	void disconnectNameEdit();

private:
	QWidget * _widget;
	QWidget * _toolBar;
	Qt::Orientation _orientation;
	QSize _titleBarSize;
	QMenu * _menu;
	QString _name;

	QWidget * _tempMovingItem;
	QLineEdit * _nameEdit;

	//Parametres de deplacement
	bool _canMove;
	bool _isMoving;
	QPoint _clickPos; //Position du curseur lors du clic

};

#endif