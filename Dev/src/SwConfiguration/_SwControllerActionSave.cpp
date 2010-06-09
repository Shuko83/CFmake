/*!
 \file _SwControllerActionSave.cpp
 \brief Implementation d une action sur le controller d execution de la configuration
 \version 1.0
 \date 
 \author 
*/
#include "_SwControllerActionSave.h"
#include <QMessageBox>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwConfiguration;

/*! \brief Constructeur */
_SwControllerActionSave::_SwControllerActionSave():
            _SwControllerAction()
{
   setAction( new QAction("Save",0));
   _action->setIcon(QIcon(":/SwConfiguration/save.png"));
   _action->setIconText("Save");
   _action->setStatusTip("Save configuration");
   _action->setEnabled(true);
   
   _launchManager = 0;
}

/*! \brief Destructeur */
_SwControllerActionSave::~_SwControllerActionSave()
{

}
void _SwControllerActionSave::setLaunchManager(_ISwConfigurationLaunchManager * launchManager)
{
    _launchManager = launchManager;
    if (_launchManager != 0 )
        _action->setEnabled(_launchManager->getStartMode().ToInt() == 0);
    else
        _action->setEnabled(false);
}

//---------------------------------------------------------------------
// Interface ISwConfigurationControllerListener
//---------------------------------------------------------------------
/*! \brief sur changement d etat */
void _SwControllerActionSave::OnStartedChanged(ISwConfigurationController *)
{
   /* if (_configurationController->isStarted())
        _action->setEnabled(false);
    else
        _action->setEnabled(_launchManager->getStartMode().ToInt() == 0);*/
}

/** @brief sur changement d etat*/
void _SwControllerActionSave::OnConfigurationEditedChanged(ISwConfigurationController *)
{
   if (_configurationController != 0)
    {
        if (_configurationController->getConfigurationEdited())
        {
            _action->setText("Save*");
            _action->setIconText("Save*");
        }
        else 
        {
            _action->setText("Save");
            _action->setIconText("Save"); 
        }
        if (_launchManager != 0 )
            _action->setEnabled(_launchManager->getStartMode().ToInt() == 0);
        else
            _action->setEnabled(false);
    }
    else
    {
        _action->setEnabled(false);
    }
}

//---------------------------------------------------------------------
// Interface ISwAction
//---------------------------------------------------------------------

/*! \brief Renvoie le Action
\return le Action */
QAction & _SwControllerActionSave::GetAction()
{
    return *_action;
}

void _SwControllerActionSave::onActionActivated()
{
    if (_configurationController != 0)
    {
           _configurationController->Save();
           if (!_configurationController->isStarted())
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Configuration Saved...");
                msgBox.setText("Do you want to start application now ?");
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Ok);
                int ret = msgBox.exec();
                 
                if (_configurationController != 0 && ret == QMessageBox::Ok)
                {
                    _configurationController->Start();
                }
            }
    }
}
