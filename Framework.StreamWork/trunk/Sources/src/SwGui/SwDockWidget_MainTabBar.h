#ifndef SWDOCKWIDGET_MAINTABBAR_H
#define SWDOCKWIDGET_MAINTABBAR_H

#include <QTabBar>
#include <QLineEdit>
//#include <QAction>

class SwDockWidget_MainTabBar : public QTabBar
{
	Q_OBJECT

public:
	SwDockWidget_MainTabBar(QWidget * parent = 0, Qt::DockWidgetArea area = Qt::NoDockWidgetArea);
	~SwDockWidget_MainTabBar();

	int activeTabUnder(QPoint pos, bool toGlobal = true);

signals:
	void moveTabRequested(int index, Qt::DockWidgetArea area);
	//void freeTabRequested(int index, QPoint pos);
	//void stopMovingTabRequested();
	void insertNewTab();
	void closeTabRequested(int index);

protected:
	virtual void mousePressEvent(QMouseEvent * e);
	virtual void mouseDoubleClickEvent ( QMouseEvent * event );
	virtual void keyPressEvent ( QKeyEvent * event );
	virtual void paintEvent ( QPaintEvent * event );
	virtual QSize tabSizeHint ( int index ) const;

protected slots:
	void renameTabTemporarily(QString name);
	void renameTab();
	void cancelNameEdit();
	void renameAction();
	void closeTab();
	void moveToLeft();
	void moveToRight();
	void moveToTop();
	void moveToBottom();
	void moveToSecondScreen();

private:
	void connectNameEdit();
	void disconnectNameEdit();

private:
	Qt::DockWidgetArea _area;

	QLineEdit * _nameEdit;
	QString _name;
	bool _moving;
	QPoint _clickPos; //Position du curseur lors du clic
	/*QAction * _renameAction;
	QAction * _closeAction;
	QAction * _moveToLeft;
	QAction * _moveToRight;
	QAction * _moveToTop;
	QAction * _moveToBottom;*/
};

#endif