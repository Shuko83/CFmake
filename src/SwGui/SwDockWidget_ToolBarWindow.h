#ifndef TOOLBARWINDOW_H
#define TOOLBARWINDOW_H

#include <QWidget>
#include <QtXml/QDomDocument>
#include "SwDockWidget_ToolBarItem.h"
#include "SwDockWidget_Overlay.h"

class SwDockWidget_ToolBarWindow : public QWidget
{
	Q_OBJECT

public:
	SwDockWidget_ToolBarWindow(QWidget * parent = 0);
	~SwDockWidget_ToolBarWindow();

	QList<QObject*> getListToolBar();

	void setMainRect(QRect rect);

	//Verrouillage de la disposition des docks
	virtual void lock();
	virtual void releaseLock();
	bool locked();

protected:
	void saveToolBar(QDomDocument doc, QDomElement dom);
	QWidget * loadToolBar(QDomNode node);
	virtual QDomElement writeWidgetParameters(QDomDocument doc, QDomElement dom, QWidget * widget) = 0;
	virtual QWidget * readWidgetParameters(QDomNode node) = 0;

protected slots:
	void reduceInToolBar();
	void releaseFromToolBar(QWidget * dock = NULL);
	
	//Deplacement d'un item pour le changer de toolbar
	void moveToolBarItem(QPoint pos);
	void stopMovingToolBarItem();

	//Deplacement d'une toolbar a un seul item
	void moveToolBar(/*QPoint pos*/);
	void stopMovingToolBar();

	void closeToolBar();

private:
	//Gestion des toolbar
	QWidget * getToolBarUnderCursor(QWidget * activeToolBar);
	void addInToolBar(QWidget * widget, QWidget * toolbar);
	void connectSignals(QWidget * toolbar);

private:
	bool _lock; //Lock configuration
	//Liste des items des toolbar
	QList<QObject*> _listToolBar;
	SwDockWidget_Overlay * _tbOverlay;
	bool _isMovingToolBarItem; //Si un element d'une toolbar est en train d'etre deplace
	QRect _globalMainRect; //Zone principale en coordonnees absolues

};









#endif