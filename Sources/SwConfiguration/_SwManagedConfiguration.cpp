/*!
\file _SwManagedConfiguration.cpp
\date 
\brief  
\author  
\version 
 */

#include "_SwManagedConfiguration.h"
#include "SwPropertiesPersistentToolbox.h"

using namespace StreamWork::SwConfiguration;
using namespace StreamWork::SwCore;


/*! \brief Constructeur */
_SwManagedConfiguration::_SwManagedConfiguration()
{
  _model=new SwPropertiesModelImpl(NULL);
  _model->SetProperties(0);
  _name = "";
  _configuration = 0;
  _loaded = false;
  _enabled = false;
  _controller = 0;
  _recordAvailable = true;
}

/*! \brief Destructeur */
_SwManagedConfiguration::~_SwManagedConfiguration()
{
  
  _controller = 0;
  if (_configuration != 0)
  {
    unregisterOwnerConfigurableListener();
    _configuration->removeListener(this);
  }
  _configuration = 0;

  if (_model != 0)
  {
    _model->SetProperties(0);
    delete _model;
  }
  _model = 0;
  
}
void _SwManagedConfiguration::unregisterOwnerConfigurableListener()
{
  if (_configuration != 0)
  {
    QList<StreamWork::SwCore::ISwServiceOwnerConfigurable *> 
        ownerConfigurableList = _configuration->getServiceOwnerConfigurable();
    for (unsigned short cpt = 0; cpt < ownerConfigurableList.size();cpt++)
    {
        ISwServiceOwnerConfigurable * ownerConfigurable = 0;
        ownerConfigurable = ownerConfigurableList.at(cpt);
        ISwObservable * observable = 
            dynamic_cast<StreamWork::SwCore::ISwObservable *> (ownerConfigurable);
        if (observable != 0)
        {
            observable->Detach(this);
        }
    }
    _configuration->removeListener(this);
  }
}
void _SwManagedConfiguration::registerOwnerConfigurableListener()
{
    QList<StreamWork::SwCore::ISwServiceOwnerConfigurable *> 
        ownerConfigurableList = _configuration->getServiceOwnerConfigurable();
    for (unsigned short cpt = 0; cpt < ownerConfigurableList.size();cpt++)
    {
        ISwServiceOwnerConfigurable * ownerConfigurable = 0;
        ownerConfigurable = ownerConfigurableList.at(cpt);
        ISwObservable * observable = 
            dynamic_cast<StreamWork::SwCore::ISwObservable *> (ownerConfigurable);
        if (observable != 0)
        {
            observable->Attach(this);
        }
    }
}
/** @brief sur changement des propriétés*/
void _SwManagedConfiguration::OnPropertiesChanged(ISwConfiguration * configuration)
{
    _model->SetProperties(0);
    if (_configuration != 0)
    {
        _model->SetProperties(_configuration->getProperties());
    }
    unregisterOwnerConfigurableListener();
    registerOwnerConfigurableListener();
}

/** @brief sur changement des propriétés*/
void _SwManagedConfiguration::OnActivate(ISwConfiguration *)
{
    _enabled = true;
}

/** @brief sur changement des propriétés*/
void _SwManagedConfiguration::OnDisable(ISwConfiguration *)
{
    _enabled = false;
}
/** @brief sur changement des propriétés*/
/*void _SwManagedConfiguration::OnRecordAvailableChanged(ISwConfiguration * config)
{
    _recordAvailable = config->isRecordAvailable();
} */
void _SwManagedConfiguration::setConfiguration(ISwConfiguration * configuration,QString name)
{

  if (_configuration != 0)
  {
    unregisterOwnerConfigurableListener();
    _configuration->removeListener(this);
  }

  if (_model != 0)
    _model->SetProperties(0);  

  _configuration = configuration;
   
  if (_configuration != 0)
  {
    _configuration->addListener(this);
    _model->SetProperties(configuration->getProperties());
    configuration->setActivated(false);
    registerOwnerConfigurableListener();
    
  }

  _name = name;
  
}

ISwConfiguration * _SwManagedConfiguration::getConfiguration()
{
  return _configuration;
}

QAbstractItemModel * _SwManagedConfiguration::getModel()
{
  return _model;
}
QString _SwManagedConfiguration::getName()
{
  return _name;
}
void    _SwManagedConfiguration::setName(QString name)
{
  _name = name;
  _model->SetProperties(0);
  if (_configuration != 0)
  {
    _model->SetProperties(_configuration->getProperties());
    _configuration->setActivated(false);
  }
}

   
/*! \brief methode permettant de charger un fichier de propriétés*/
bool _SwManagedConfiguration::Load(QDomElement & elt, QString * errorMessage)
{
    _loaded = false;
    QDomElement elt_ent;

    if (_configuration == 0)
    {
        if (errorMessage != 0)
            *errorMessage = "Configuration Not Defined";
        return false;
    }
    if (_configuration->getProperties() == 0)
    {
        if (errorMessage != 0)
            *errorMessage = "No properties";
        return false;
    }
    // try catch
    SwPropertiesPersistentToolbox::Load(elt,_configuration->getProperties());

    QList<StreamWork::SwCore::ISwServiceOwnerConfigurable *> 
        ownerConfigurableList = _configuration->getServiceOwnerConfigurable();

    for(elt_ent = elt.firstChildElement(); !elt_ent.isNull(); elt_ent = elt_ent.nextSiblingElement())
    {
        if (elt_ent.nodeName()==QString("OwnerConfigurable")) 
        {
            int index = -1;
            if (elt_ent.hasAttribute("index"))
            {
                index = elt_ent.attribute("index").toInt();
                if (index < ownerConfigurableList.size() )
                {
                    ownerConfigurableList.at(index)->LoadConfiguration(elt_ent);
                    
                }
            }
        }
    }

    _loaded = true;
    return _loaded;
}

/*! \brief methode permettant de charger un fichier de propriétés*/
bool _SwManagedConfiguration::Save(QDomElement & elt,QDomDocument & doc, QString * errorMessage)
{
    if (_configuration == 0)
    {
        if (errorMessage != 0)
            *errorMessage = "Configuration Not Defined";
        return false;
    }
    if (_configuration->getProperties() == 0)
    {
        if (errorMessage != 0)
            *errorMessage = "No properties";
        return false;
    }

    // try catch
    SwPropertiesPersistentToolbox::Save(elt,doc,_configuration->getProperties());
    QList<StreamWork::SwCore::ISwServiceOwnerConfigurable *> 
        ownerConfigurableList = _configuration->getServiceOwnerConfigurable();

    QDomElement elt_ent;

    QList<StreamWork::SwCore::ISwServiceOwnerConfigurable *>::iterator it;
    it = ownerConfigurableList.begin();
    int index = 0;
    while(it != ownerConfigurableList.end())
    {
        elt_ent=doc.createElement("OwnerConfigurable");
        elt_ent.setAttribute("index",index);
        (*it)->SaveConfiguration(elt_ent,doc);
        elt.appendChild(elt_ent);
        it ++;
        index++;
    }
    return true;
    
}

void _SwManagedConfiguration::setRecordAvailable( bool value)
{
    _recordAvailable = value;
}

bool _SwManagedConfiguration::isRecordAvailable()
{
    //if (_configuration == 0)
    //    return false;
    return _recordAvailable;//_configuration->isRecordAvailable();
}

bool _SwManagedConfiguration::isEnabled()
{
    if (_configuration == 0)
        return false;
    return _configuration->isActivated();

}

void _SwManagedConfiguration::setEnabled( bool value)
{
    if (_configuration == 0)
        return;
     _configuration->setActivated(value);

}
    
void _SwManagedConfiguration::setConfigurationController(ISwConfigurationController * controller)
{
    _controller = controller;
}

void _SwManagedConfiguration::Update()
{
    if (_controller != 0)
        _controller->setConfigurationEdited(true);
}