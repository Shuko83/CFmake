/*!
\file SwQActionAdapter.cpp
\brief Implementation of the Class SwQActionAdapter permettant de générer des QAction
\version 1.0
\date 01/09/2013 2:00
\author AAY
*/

#ifndef __SxQActionAdapter_H
#define __SxQActionAdapter_H


#include <QAction.h>
/**
*	@class SwQActionAdapter 
*/   
class  SwQActionAdapter : 
	public QObject
{
	Q_OBJECT

		Q_PROPERTY (QString actionName 
		READ getActionName
		WRITE setActionName) 

		Q_PROPERTY (QIcon actionIcon 
		READ getActionIcon
		WRITE setActionIcon)

public:

	/*! \brief Constructeur */
	SwQActionAdapter();

	/*! \brief Destructeur */
	virtual ~SwQActionAdapter();

	QAction & GetAction();


	QString getActionName() const;
	void setActionName(QString val);

	QIcon getActionIcon() const;
	void setActionIcon(QIcon val);
protected:

	QString _actionName;

	QIcon _actionIcon;

	QAction * _action;  

};


#endif 
