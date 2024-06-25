/*!
 \file _SwControllerActionRestart.cpp
 \brief Implementation d une action sur le controller d execution de la configuration
 \version 1.0
 \date 
 \author 
*/
#include "_SwControllerActionRestart.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwConfiguration;

/*! \brief Constructeur */
_SwControllerActionRestart::_SwControllerActionRestart():
            _SwControllerAction()
{
   setAction( new QAction("Restart",0));
   _action->setIcon(QIcon(":/SwConfiguration/restart.png"));
   _action->setIconText("Restart");
   _action->setStatusTip("Reconnect and restart processing");
}

/*! \brief Destructeur */
_SwControllerActionRestart::~_SwControllerActionRestart()
{

}

//---------------------------------------------------------------------
// Interface ISwConfigurationControllerListener
//---------------------------------------------------------------------
/*! \brief sur changement d etat */
void _SwControllerActionRestart::OnStartedChanged(ISwConfigurationController *)
{
    if (_configurationController != 0)
    {
        _action->setEnabled(_configurationController->isStarted());
    }
}


/** @brief sur changement d etat*/
void _SwControllerActionRestart::OnConfigurationEditedChanged(ISwConfigurationController *)
{
    
}

void _SwControllerActionRestart::onActionActivated()
{
    if (_configurationController != 0)
    {
        _configurationController->Stop();
        _configurationController->Start();
    }
}
