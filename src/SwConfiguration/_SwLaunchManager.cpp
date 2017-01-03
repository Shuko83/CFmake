/*!
 \file _SwLaunchManager.cpp
 \brief 
 \version 1.0
 \date 
 \author 
*/
#include <ISwProperty.h>
#define CL_LAUNCH_CONFIG_XML_MAIN_NODE  "LAUNCH_PROPERTIES"
#define CL_LAUNCH_CONFIG_XML_PROPERTY_NODE "LAUNCH_PROPERTY"
#define CL_LAUNCH_CONFIG_XML_PROPERTY_NAME "NAME"
#define CL_LAUNCH_CONFIG_XML_PROPERTY_VALUE "VALUE"
#include <QMessageBox>
#include <QFile>
#include <QString>

#include "_SwLaunchManager.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwConfiguration;

/*! \brief Constructeur */
_SwLaunchManager::_SwLaunchManager():
    _configurationFile(SwFileDescriptor::FileRead, "XML Files (*.conf)"),
    _recordDirectory(SwFileDescriptor::DirectorySelect, "Directory"),
    _replayDirectory(SwFileDescriptor::DirectorySelect, "Directory")
{

   _autoStart = false;
   _recordActivated = false;    
              
   _isRunning = false;
   _isLoaded = false;
   _startMode.AddKey(0,"Normal Mode");
   _startMode.AddKey(1,"Replay Mode");   
   _startMode.FromInt(0);
  

   _editor = 0;
}



/*! \brief Destructeur */
_SwLaunchManager::~_SwLaunchManager()
{
    
}



////////// interface ISwConfigurationlaunchManager
void _SwLaunchManager::setEditor(ISwConfigurationLaunchEditor * editor)
{
    _editor = editor;
    if (_editor != 0)
        _editor->setLaunchConfigurationModel(this);
}



SwEnum _SwLaunchManager::getStartMode()
{
    return _startMode;
}
   
      
void _SwLaunchManager::setStartMode(SwEnum value)
{
    _startMode = value;
    if (_editor != 0)
        _editor->startModeUpdated();
}
     
          
bool _SwLaunchManager::getAutoStart()
{
    return _autoStart;
}
            
void _SwLaunchManager::setAutoStart(bool value)
{
    _autoStart = value;
    if (_editor != 0)
        _editor->autoStartUpdated();
}


bool _SwLaunchManager::getRecordActivated()
{
    return _recordActivated;
}
           
void _SwLaunchManager::setRecordActivated(bool value) 
{

    _recordActivated = value;
    if (_editor != 0)
        _editor->recordActivatedUpdated();
}
 

SwFileDescriptor _SwLaunchManager::getRecordDirectory()
{

    return _recordDirectory;
}
            
void _SwLaunchManager::setRecordDirectory(SwFileDescriptor value)
{

    _recordDirectory = value;
    if (_editor != 0)
        _editor->recordDirectoryUpdated();
}
 

SwFileDescriptor _SwLaunchManager::getReplayDirectory()
{
    return _replayDirectory;
}
            
void _SwLaunchManager::setReplayDirectory(SwFileDescriptor value)
{
    _replayDirectory = value;
    if (_editor != 0)
        _editor->replayDirectoryUpdated();
}
 
       
SwFileDescriptor _SwLaunchManager::getConfigurationFile()
{
    return _configurationFile;
}
            
void _SwLaunchManager::setConfigurationFile(SwFileDescriptor value)
{
    _configurationFile =  value;
    if (_editor != 0)
        _editor->configurationFileNameUpdated();
}
 
                        
bool _SwLaunchManager::LoadValues(QString filename)
{
    QFile file;
    QDomDocument doc;
    QString xml_error;
    int error_line,error_column;
   

    QString desc;
    file.setFileName(filename);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //QMessageBox::warning(NULL,"StreamWorkEditor alert",QString("Fail to open file stream %1").arg(_streamFileName));
        return false;
    }
    desc=QString(file.readAll());
    file.close();
    //parsing xml du document
    if (!doc.setContent(desc,&xml_error,&error_line,&error_column)) 
    {
        QString msg=QString("XML Parsing error:%1 at position %2,%3\n%4").arg(xml_error).arg(error_line).arg(error_column).arg(desc);
        QMessageBox::warning(NULL,"Loading Failed",msg);
        return false;
    }

    //A ce stade le doc est chargé
    QDomElement rootElement = doc.firstChildElement(CL_LAUNCH_CONFIG_XML_MAIN_NODE);
    LoadLaunchProperties(rootElement);

    return true;

}

bool _SwLaunchManager::SaveValues(QString filename)
{

    QDomDocument doc;
    QByteArray desc;
    QFile file;

   
    QDomElement rootElement;

    rootElement = doc.createElement(CL_LAUNCH_CONFIG_XML_MAIN_NODE);
    doc.appendChild(rootElement);

    // try catch
    SaveLaunchProperties(rootElement,doc);

    //Recuperation du stream
    desc=doc.toByteArray(4); //Indentation de quatre espace
    //Ouverture d'un fichier en ecriture
    file.setFileName(filename);
    if (file.open(QIODevice::WriteOnly  | QIODevice::Truncate)==false) {
        //QMessageBox::critical(NULL,"StreamWorkEditor critical",QString("Fail to save stream in file %1").arg(_streamFileName));
        return false;
    }

    //Ecriture du fichier
    file.write(desc);
    //Fermeture du fichier
    file.close();
  
    return true;

}
 
void _SwLaunchManager::LoadLaunchProperties(QDomElement & elt)
{
    QDomElement elt_ent;
    
    SwEnum startMode;
    startMode.AddKey(0,"Normal Mode");
    startMode.AddKey(1,"Replay Mode");   
    startMode.FromInt(0);
    
    setStartMode(startMode);
   // setStartMode(0); // force le mode de démarrage a 0

    for( elt_ent = elt.firstChildElement(); 
        !elt_ent.isNull(); 
        elt_ent = elt_ent.nextSiblingElement())
    {
        
        if (elt_ent.nodeName()==QString(CL_LAUNCH_CONFIG_XML_PROPERTY_NODE)) 
        {
            if (
                elt_ent.hasAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_NAME) && 
                elt_ent.hasAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_VALUE)
                )
            {
                QString propertyName = elt_ent.attribute(CL_LAUNCH_CONFIG_XML_PROPERTY_NAME);
                QString propertyValue = elt_ent.attribute(CL_LAUNCH_CONFIG_XML_PROPERTY_VALUE);
                
                /*if (propertyName.compare("StartMode") == 0)
                {
                   bool res = false;
                   int value = propertyValue.toInt(&res);

                   if (res)
                   {
                       SwEnum startMode;
                       startMode.AddKey(0,"Normal Mode");
                       startMode.AddKey(1,"Replay Mode");   
                       startMode.FromInt(value);
                        
                       setStartMode(startMode);
                   }
                }
                else*/
                if (propertyName.compare("AutoStart") == 0)
                {
                    bool res = false;
                    int value = propertyValue.toInt(&res);
                    if (res)
                        setAutoStart(value != 0);
                }
                else
                if (propertyName.compare("RecordActivated") == 0)
                {
                    bool res = false;
                    int value = propertyValue.toInt(&res);
                    if (res)
                        setRecordActivated(value != 0);
                }
                else
                if (propertyName.compare("RecordDirectory") == 0)
                {
                    SwFileDescriptor recordDirectory(SwFileDescriptor::DirectorySelect, "Directory");
                    recordDirectory.setFileName(propertyValue); 
                    setRecordDirectory(recordDirectory);
                }
                else
                if (propertyName.compare("ReplayDirectory") == 0)
                {
                    SwFileDescriptor replayDirectory(SwFileDescriptor::DirectorySelect, "Directory");
                    replayDirectory.setFileName(propertyValue);
                    setReplayDirectory(replayDirectory);
                }
                else
                if (propertyName.compare("ConfigurationFile") == 0)
                {
                    SwFileDescriptor configurationFile(SwFileDescriptor::FileRead, "XML Files (*.xml)");
                    configurationFile.setFileName(propertyValue);
                    setConfigurationFile(configurationFile);
                }
            }
        }
    }

}

void _SwLaunchManager::SaveLaunchProperties(QDomElement & elt,QDomDocument &doc)
{
    QDomElement myElt;
    /*myElt = doc.createElement(CL_LAUNCH_CONFIG_XML_PROPERTY_NODE);
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_NAME,"StartMode");
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_VALUE,getStartMode().ToInt());
    elt.appendChild(myElt);*/
    myElt = doc.createElement(CL_LAUNCH_CONFIG_XML_PROPERTY_NODE);
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_NAME,"AutoStart");
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_VALUE,QString::number(getAutoStart()));
    elt.appendChild(myElt);
    myElt = doc.createElement(CL_LAUNCH_CONFIG_XML_PROPERTY_NODE);
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_NAME,"RecordActivated");
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_VALUE,QString::number(getRecordActivated()));
    elt.appendChild(myElt);

    myElt = doc.createElement(CL_LAUNCH_CONFIG_XML_PROPERTY_NODE);
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_NAME,"RecordDirectory");
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_VALUE,getRecordDirectory().getFileName());
    elt.appendChild(myElt);
    myElt = doc.createElement(CL_LAUNCH_CONFIG_XML_PROPERTY_NODE);
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_NAME,"ReplayDirectory");
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_VALUE,getReplayDirectory().getFileName());
    elt.appendChild(myElt);
    myElt = doc.createElement(CL_LAUNCH_CONFIG_XML_PROPERTY_NODE);
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_NAME,"ConfigurationFile");
    myElt.setAttribute(CL_LAUNCH_CONFIG_XML_PROPERTY_VALUE,getConfigurationFile().getFileName());
    elt.appendChild(myElt);
    
}