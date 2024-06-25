/*!
 \file _SwControllerActionStart.cpp
 \brief Implementation d une action sur le controller d execution de la configuration
 \version 1.0
 \date 
 \author 
*/
#include "_SwControllerActionStart.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwConfiguration;

/*! \brief Constructeur */
_SwControllerActionStart::_SwControllerActionStart():
            _SwControllerAction()
{
   setAction( new QAction("Start",0));
   _action->setIcon(QIcon(":/SwConfiguration/play.png"));
   _action->setIconText("Start");
   _action->setStatusTip("Connect and start processing");

}

/*! \brief Destructeur */
_SwControllerActionStart::~_SwControllerActionStart()
{

}

//---------------------------------------------------------------------
// Interface ISwConfigurationControllerListener
//---------------------------------------------------------------------
/*! \brief sur changement d etat */
void _SwControllerActionStart::OnStartedChanged(ISwConfigurationController *)
{
    if (_configurationController != 0)
    {
        _action->setEnabled(!_configurationController->isStarted());
    }
}

/** @brief sur changement d etat*/
void _SwControllerActionStart::OnConfigurationEditedChanged(ISwConfigurationController *)
{
    
}

void _SwControllerActionStart::onActionActivated()
{
    if (_configurationController != 0)
    {
        _configurationController->Start();
    }
}
