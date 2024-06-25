/*!
 \file _SwControllerActionStop.cpp
 \brief Implementation d une action sur le controller d execution de la configuration
 \version 1.0
 \date 
 \author 
*/
#include "_SwControllerActionStop.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwConfiguration;

/*! \brief Constructeur */
_SwControllerActionStop::_SwControllerActionStop():
            _SwControllerAction()
{
   setAction( new QAction("Stop",0));
   _action->setIcon(QIcon(":/SwConfiguration/stop.png"));
   _action->setIconText("Stop");
   _action->setStatusTip("Disconnect and stop processing");
}

/*! \brief Destructeur */
_SwControllerActionStop::~_SwControllerActionStop()
{

}

//---------------------------------------------------------------------
// Interface ISwConfigurationControllerListener
//---------------------------------------------------------------------
/*! \brief sur changement d etat */
void _SwControllerActionStop::OnStartedChanged(ISwConfigurationController *)
{
    if (_configurationController != 0)
    {
        _action->setEnabled(_configurationController->isStarted());
    }
}


/** @brief sur changement d etat*/
void _SwControllerActionStop::OnConfigurationEditedChanged(ISwConfigurationController *)
{
    
}

void _SwControllerActionStop::onActionActivated()
{
    if (_configurationController != 0)
    {
        _configurationController->Stop();
    }
}
