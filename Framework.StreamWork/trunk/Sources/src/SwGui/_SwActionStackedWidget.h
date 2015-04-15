/*!
\file _SwActionStackedWidget.h
\brief Action stacked widget
\version 1.0
\date 18/01/2011
\author AAY
*/

#ifndef _SWGUI_SWACTIONSTACKEDWIDGET_H
#define _SWGUI_SWACTIONSTACKEDWIDGET_H

/*
* INCLUDES GLOBAUX
*/
#include <QAction>
#include <QObject>

/*
* INCLUDES LOCAUX
*/
#include <ISwAction.h>

using namespace StreamWork::SwGui;

/*!
\class _SwActionStackedWidget 
\brief Implementation of the Class _ISwControllerAction 
*/
class _SwActionStackedWidget :  public QObject, public virtual ISwAction
{
	Q_OBJECT
public:

	/*! \brief Contructeur */
	_SwActionStackedWidget(QString name,int index);

	/*! \brief Destructeur */
	virtual ~_SwActionStackedWidget();

	/*! \brief Renvoie le Action
	\return le Action */
	virtual QAction & GetAction() ;

protected:
	void setAction(QAction * action);
	virtual void onActionActivated();

	QAction * _action;
	int _index;
protected slots:
	void actionActivated();

signals:
	void callback(int);

};


#endif 
