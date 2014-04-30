#ifndef SWDOCKWIDGET_MAINTABBAR_H
#define SWDOCKWIDGET_MAINTABBAR_H

#include <QTabBar>
#include <QLineEdit>

class SwDockWidget_MainTabBar : public QTabBar
{
	Q_OBJECT

public:
	SwDockWidget_MainTabBar(QWidget * parent = 0);
	~SwDockWidget_MainTabBar();

signals:
	void moveTabRequested(QPoint pos);
	void freeTabRequested(int index, QPoint pos);
	void stopMovingTabRequested();
	void insertNewTab();

protected:
	virtual void mousePressEvent(QMouseEvent * e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void mouseDoubleClickEvent ( QMouseEvent * event );
	virtual void keyPressEvent ( QKeyEvent * event );
	virtual void paintEvent ( QPaintEvent * event );
	virtual void mouseMoveEvent(QMouseEvent * e);

protected slots:
	void renameTabTemporarily(QString name);
	void renameTab();
	void cancelNameEdit();

private:
	void connectNameEdit();
	void disconnectNameEdit();
	void renameAction();

private:
	QLineEdit * _nameEdit;
	QString _name;
	bool _moving;
	QPoint _clickPos; //Position du curseur lors du clic
};

#endif