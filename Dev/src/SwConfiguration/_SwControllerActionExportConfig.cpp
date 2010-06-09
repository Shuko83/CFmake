/*!
 \file _SwControllerActionExportConfig.cpp
 \brief Implementation d une action sur le controller d execution de la configuration
 \version 1.0
 \date 
 \author 
*/
#include "_SwControllerActionExportConfig.h"
#include <QFileDialog>
#include <QFileInfo>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwConfiguration;

/*! \brief Constructeur */
_SwControllerActionExportConfig::_SwControllerActionExportConfig():
            _SwControllerAction()
{
   setAction( new QAction("Export",0));
   _action->setIcon(QIcon(":/SwConfiguration/saveas.png"));
   _action->setIconText("Export");
   _action->setStatusTip("Export configuration file");
   _action->setEnabled(true);
   _configSaver = 0;
   _launchManager = 0;
   _lastPath = "";
}

/*! \brief Destructeur */
_SwControllerActionExportConfig::~_SwControllerActionExportConfig()
{

}
        
void _SwControllerActionExportConfig::setConfigSaver(ISwConfigurationSaver * configSaver)
{
    _configSaver = configSaver;
}
          
void _SwControllerActionExportConfig::setLaunchManager(_ISwConfigurationLaunchManager * launchManager)
{
    _launchManager = launchManager;
}
//---------------------------------------------------------------------
// Interface ISwConfigurationControllerListener
//---------------------------------------------------------------------
/*! \brief sur changement d etat */
void _SwControllerActionExportConfig::OnStartedChanged(ISwConfigurationController *)
{

}

/** @brief sur changement d etat*/
void _SwControllerActionExportConfig::OnConfigurationEditedChanged(ISwConfigurationController *)
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
QAction & _SwControllerActionExportConfig::GetAction()
{
    return *_action;
}

void _SwControllerActionExportConfig::onActionActivated()
{
    if (_configSaver != 0)
    {

        QString fileName = QFileDialog::getSaveFileName(
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
            SwFileDescriptor desc(SwFileDescriptor::FileWrite, "XML Files (*.conf)");
            desc.setFileName(fileName);
            //_launchConfiguration->setConfigurationFile(desc);
            _configSaver->SaveConfiguration(fileName);
            
        }
    }
}
