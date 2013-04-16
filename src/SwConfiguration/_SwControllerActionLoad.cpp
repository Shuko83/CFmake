/*!
 \file _SwControllerActionLoad.cpp
 \brief Implementation d une action sur le controller d execution de la configuration
 \version 1.0
 \date 
 \author 
*/
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include "_SwControllerActionLoad.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwConfiguration;

/*! \brief Constructeur */
_SwControllerActionLoad::_SwControllerActionLoad():
            _SwControllerAction()
{
   setAction( new QAction("Load",0));
   _action->setIcon(QIcon(":/SwConfiguration/load.png"));
   _action->setIconText("Load");
   _action->setStatusTip("Load configuration file");
   _action->setEnabled(true);
   _configList = 0;
   _launchManager = 0;
   _lastPath = "";
}

/*! \brief Destructeur */
_SwControllerActionLoad::~_SwControllerActionLoad()
{

}
        
void _SwControllerActionLoad::setConfigurationsList(_ISwManagedConfigurationsList * configList)
{
    _configList = configList;
}
          
void _SwControllerActionLoad::setLaunchManager(_ISwConfigurationLaunchManager * launchManager)
{
    _launchManager = launchManager;
}
//---------------------------------------------------------------------
// Interface ISwConfigurationControllerListener
//---------------------------------------------------------------------
/*! \brief sur changement d etat */
void _SwControllerActionLoad::OnStartedChanged(ISwConfigurationController *)
{

}

/** @brief sur changement d etat*/
void _SwControllerActionLoad::OnConfigurationEditedChanged(ISwConfigurationController *)
{
   if (_launchManager != 0)
    {
        if (_launchManager->getStartMode().ToInt() == 0 )
        {
            _action->setEnabled(true);
        }
        else
        {
            _action->setEnabled(false);
        }
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
QAction & _SwControllerActionLoad::GetAction()
{
    return *_action;
}

void _SwControllerActionLoad::onActionActivated()
{


    if (this->_configurationController != 0 && _configurationController->isStarted())
    {    
            QMessageBox msgBox;
            msgBox.setWindowTitle("Application running...");
            msgBox.setText("Do you want to stop application and change configuration ?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();
            if (_configurationController != 0 && ret == QMessageBox::Ok)
            {
                _configurationController->Stop();
               
            }
            else
                return;
           
    }

    if (_configList == 0)
        return;


    QString fileName = QFileDialog::getOpenFileName(
        NULL, // Parent Widget
        "Select Configuration File", // Caption
        _lastPath, // Directory
        "XML Files (*.conf)" // Filter
        );

    if(!fileName.isNull()) 
    {
        QFileInfo fileInfo(fileName);
        if (fileInfo.exists() && fileInfo.isFile())
        {
            _lastPath = fileInfo.dir().absolutePath();
        }
        SwFileDescriptor desc(SwFileDescriptor::FileRead, "XML Files (*.conf)");
        desc.setFileName(fileName);
        _configList->LoadConfiguration(fileName);
        if (_configurationController != 0)
        {
            _configurationController->setConfigurationEdited(true);
        }
    }
}
