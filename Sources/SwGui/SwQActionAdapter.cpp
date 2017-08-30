/*!
 \file _SwGuiCompActionProvider.cpp
 \brief Implementation of the Class _SwGuiCompActionProvider permettant de générer des QAction
 \version 1.0
 \date 01/09/2013 2:00
 \author AAY
*/

#include "SwQActionAdapter.h"

using namespace StreamWork::SwGui;
//-----------------------------------------------------------------------
SwQActionAdapter::SwQActionAdapter()
{
	_action = new QAction(NULL);
}

//-----------------------------------------------------------------------
QAction & SwQActionAdapter::GetAction()
{
	return *_action;
}

//-----------------------------------------------------------------------
SwQActionAdapter::~SwQActionAdapter()
{
	_action->setParent(NULL);
	delete _action;
}

//-----------------------------------------------------------------------
QString SwQActionAdapter::getActionName() const
{
	return _actionName;
}

//-----------------------------------------------------------------------
void SwQActionAdapter::setActionName( QString val )
{
	_actionName = val;
	_action->setText(val);
}

//-----------------------------------------------------------------------
QIcon SwQActionAdapter::getActionIcon() const
{
	return _actionIcon;
}

//-----------------------------------------------------------------------
void SwQActionAdapter::setActionIcon( QIcon val )
{
	_actionIcon = val;
	_action->setIcon(val);
}
