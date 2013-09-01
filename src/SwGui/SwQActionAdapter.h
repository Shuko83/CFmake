/*!
\file _SwGuiCompActionProvider.cpp
\brief Implementation of the Class _SwGuiCompActionProvider permettant de générer des QAction
\version 1.0
\date 01/09/2013 2:00
\author AAY
*/

#ifndef __SwQActionAdapter_H
#define __SwQActionAdapter_H


#include "ISwAction.h"
#include <QAction.h>
/**
*	@class SwQActionAdapter 
*/   
class  SwQActionAdapter : 
	public QObject,
	virtual public StreamWork::SwGui::ISwAction
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
