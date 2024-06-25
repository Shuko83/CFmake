/*!
 \file _SwConfigurationExportedOwnerConfigurable.cpp
 \brief Class implementant une entite exportable executable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "_SwConfigurationExportedOwnerConfigurable.h"
#include "Component/Services/ISwServiceOwnerConfigurable.h"


/*! \brief Constructeur */
_SwConfigurationExportedOwnerConfigurable::_SwConfigurationExportedOwnerConfigurable():_SwConfigurationExportedEntity() {
	_type=Ent_OwnerConfigurable;
    _internal_ownerConfigurable=NULL;
    _configurationObservers.clear();
}
/*! \brief Destructeur */
_SwConfigurationExportedOwnerConfigurable::~_SwConfigurationExportedOwnerConfigurable() {
    _internal_ownerConfigurable=NULL;
    _configurationObservers.clear();
}
/*! \brief Specific bind */
void _SwConfigurationExportedOwnerConfigurable::SpecificBind(){
    _internal_ownerConfigurable=dynamic_cast<ISwServiceOwnerConfigurable *>(_host->QueryService(CG_SW_SERVICE_OWNER_CONFIGURABLE));
    if (_internal_ownerConfigurable==NULL)
        return;
    ISwObservable * observable = dynamic_cast<ISwObservable*> (_internal_ownerConfigurable);
    if (observable != 0)
        observable->Attach(this);
    //Enregistrement des services
    //_model->RegisterService(this);
}
/*! \brief Defait la liaison */
void _SwConfigurationExportedOwnerConfigurable::SpecificUnbind(){
    if (_internal_ownerConfigurable==NULL)
        return;
    ISwObservable * observable = dynamic_cast<ISwObservable*> (_internal_ownerConfigurable);
    if (observable != 0)
        observable->Detach(this);
    //Desenregistrement des services
    //_model->UnregisterService(this->GetServiceName());
    _internal_ownerConfigurable=NULL;
}

      
//---------------------------------------------------------------------
// InterfaceISwServiceOwnerConfigurable
//---------------------------------------------------------------------

void _SwConfigurationExportedOwnerConfigurable::Liberate()
{
    
}      
		
/*! \brief methode permettant de charger des donnees de configuration*/
void _SwConfigurationExportedOwnerConfigurable::LoadConfiguration(QDomElement & element)
{
    if (_internal_ownerConfigurable != 0)
        _internal_ownerConfigurable->LoadConfiguration(element);
}   
/*! \brief methode permettant de sauver des donnees de configuration*/
void _SwConfigurationExportedOwnerConfigurable::SaveConfiguration(QXmlStreamWriter & writer)
{
    if (_internal_ownerConfigurable != 0)
        _internal_ownerConfigurable->SaveConfiguration(writer);
    
}     
//----------------------------------------------------
// Interface ISwServiceOwnerConfigurable pour ISwServiceOwnerConfigurable
//----------------------------------------------------
/*! \brief Permet d'attacher un observer*/
void _SwConfigurationExportedOwnerConfigurable::Attach(ISwObserver * confChangeObserver)
{
    if (!_configurationObservers.contains(confChangeObserver))
    {
        _configurationObservers.push_back(confChangeObserver);
    }
}

/*! \brief Permet de detacher un observer*/
void _SwConfigurationExportedOwnerConfigurable::Detach(ISwObserver * confChangeObserver)
{
    if (_configurationObservers.contains(confChangeObserver))
    {
        _configurationObservers.removeOne(confChangeObserver);
    }
}

/*! \brief Permet notifier aux observers un changement */
void _SwConfigurationExportedOwnerConfigurable::Notify()
{
    QListIterator<ISwObserver *> itObserver(_configurationObservers);
    while (itObserver.hasNext())
    {
        itObserver.next()->Update();
    }
}  
/*! \brief Permet notifier aux observers un changement */
void _SwConfigurationExportedOwnerConfigurable::Update(StreamWork::SwCore::ISwObservable *)
{

    Notify();
} 
