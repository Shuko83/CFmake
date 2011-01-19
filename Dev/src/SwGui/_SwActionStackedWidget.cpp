/*!
 \file _SwActionStackedWidget.cpp
 \brief Implementation d une action sur le controller d execution de la configuration
 \version 1.0
 \date 
 \author 
*/
#include "QApplication.h"
#include "_SwActionStackedWidget.h"

using namespace StreamWork::SwGui;

/*! \brief Constructeur */
_SwActionStackedWidget::_SwActionStackedWidget(QString name,int index):QObject(),_action(NULL)
{
	setAction( new QAction(name,0));
	_action->setIconText(name);
	_action->setStatusTip(name);
	_action->setEnabled(true);
	_index = index;
}

/*! \brief Destructeur */
_SwActionStackedWidget::~_SwActionStackedWidget()
{
    if (_action != 0)
        delete _action;
    _action = 0;
   
}

//---------------------------------------------------------------------
// Interface ISwAction
//---------------------------------------------------------------------

/*! \brief Renvoie l' Action
\return le Action */
QAction & _SwActionStackedWidget::GetAction()
{
    return *_action;
}

void _SwActionStackedWidget::setAction(QAction * action)
{
    if (_action != 0)
        delete _action;

    _action = action;

    if (_action != 0)
    {
        _action->connect(_action, SIGNAL(triggered()), this, SLOT(actionActivated()));
        _action->setEnabled(false);
    }
}       

void _SwActionStackedWidget::actionActivated()
{
    QWidget * focusWidget = QApplication::focusWidget();
    if (focusWidget != 0)
        focusWidget->clearFocus();

    onActionActivated();
}

void _SwActionStackedWidget::onActionActivated()
{
	emit callback(_index);
}