/*!
 \file _SwControllerAction.cpp
 \brief Implementation d une action sur le controller d execution de la configuration
 \version 1.0
 \date 
 \author 
*/
#include "QApplication.h"
#include "_SwControllerAction.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwConfiguration;

/*! \brief Constructeur */
_SwControllerAction::_SwControllerAction():QObject()
{
    _configurationController = 0;
    _action = new QAction(0);
}

/*! \brief Destructeur */
_SwControllerAction::~_SwControllerAction()
{
    if (_action != 0)
        delete _action;
    _action = 0;
   
    if (_configurationController != 0)
        _configurationController->removeControllerListener(this);
}

/*! \brief affectation du controlleur */
void _SwControllerAction::setController(ISwConfigurationController * controller)
{
    if (_configurationController!= 0)
        _configurationController->removeControllerListener(this);

    _configurationController = controller;

    if (_configurationController!= 0)
        _configurationController->addControllerListener(this);
}
//---------------------------------------------------------------------
// Interface ISwConfigurationControllerListener
//---------------------------------------------------------------------

/** @brief sur changement d etat*/
void _SwControllerAction::OnStartedChanged(ISwConfigurationController *)
{
    
}

/** @brief sur changement d etat*/
void _SwControllerAction::OnConfigurationEditedChanged(ISwConfigurationController *)
{
    
}

/*! \brief Renvoie le Action
\return le Action */
QAction* _SwControllerAction::getAction()
{
    return _action;
}
void _SwControllerAction::setAction(QAction * action)
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

void _SwControllerAction::actionActivated()
{
    QWidget * focusWidget = QApplication::focusWidget();
    if (focusWidget != 0)
        focusWidget->clearFocus();
    onActionActivated();
}
void _SwControllerAction::onActionActivated()
{
}
