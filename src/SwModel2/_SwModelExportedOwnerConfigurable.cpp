/*!
 \file _SwModelExportedOwnerConfigurable.cpp
 \brief Class implementant une entite exportable executable
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "_SwModelExportedOwnerConfigurable.h"
#include "_SwModel_Class.h"
#include "_SwModelHost_Class.h"


/*! \brief Constructeur */
_SwModelExportedOwnerConfigurable::_SwModelExportedOwnerConfigurable():_SwModelExportedEntity() {
	_type=Ent_OwnerConfigurable;
    _internal_ownerConfigurable=NULL;
    _configurationObservers.clear();
}
/*! \brief Destructeur */
_SwModelExportedOwnerConfigurable::~_SwModelExportedOwnerConfigurable() {
    _configurationObservers.clear();
}
/*! \brief Specific bind */
void _SwModelExportedOwnerConfigurable::SpecificBind(){
    _internal_ownerConfigurable=dynamic_cast<ISwServiceOwnerConfigurable *>(_host->QueryService(CG_SW_SERVICE_OWNER_CONFIGURABLE));
    if (_internal_ownerConfigurable==NULL)
        return;
    ISwObservable * observable = dynamic_cast<ISwObservable*> (_internal_ownerConfigurable);
    if (observable != 0)
        observable->Attach(this);
    //Enregistrement des services
    _model->RegisterService(this);
}
/*! \brief Defait la liaison */
void _SwModelExportedOwnerConfigurable::SpecificUnbind(){
    if (_internal_ownerConfigurable==NULL)
        return;
    ISwObservable * observable = dynamic_cast<ISwObservable*> (_internal_ownerConfigurable);
    if (observable != 0)
        observable->Detach(this);
    //Desenregistrement des services
    _model->UnregisterService(this->GetServiceName());
    _internal_ownerConfigurable=NULL;
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwModelExportedOwnerConfigurable::Liberate() {

}  
         
//---------------------------------------------------------------------
// Interface ISwServiceOwnerConfigurable
//---------------------------------------------------------------------
/*! \brief methode permettant de charger des donnees de configuration*/
void _SwModelExportedOwnerConfigurable::LoadConfiguration(QDomElement & element)
{
    if (_internal_ownerConfigurable != 0)
        _internal_ownerConfigurable->LoadConfiguration(element);
}

/*! \brief methode permettant de sauver des donnees de configuration*/
void _SwModelExportedOwnerConfigurable::SaveConfiguration(QDomElement & element ,QDomDocument & doc)
{
    if (_internal_ownerConfigurable != 0)
        _internal_ownerConfigurable->SaveConfiguration(element,doc);    
}
        
//----------------------------------------------------
// Interface ISwServiceOwnerConfigurable pour ISwServiceOwnerConfigurable
//----------------------------------------------------
/*! \brief Permet d'attacher un observer*/
void _SwModelExportedOwnerConfigurable::Attach(ISwObserver * confChangeObserver)
{
    if (!_configurationObservers.contains(confChangeObserver))
    {
        _configurationObservers.push_back(confChangeObserver);
    }
}

/*! \brief Permet de detacher un observer*/
void _SwModelExportedOwnerConfigurable::Detach(ISwObserver * confChangeObserver)
{
    if (_configurationObservers.contains(confChangeObserver))
    {
        _configurationObservers.removeOne(confChangeObserver);
    }
}

/*! \brief Permet notifier aux observers un changement */
void _SwModelExportedOwnerConfigurable::Notify()
{
    QListIterator<ISwObserver *> itObserver(_configurationObservers);
    while (itObserver.hasNext())
    {
        itObserver.next()->Update();
    }
}  
/*! \brief Permet notifier aux observers un changement */
void _SwModelExportedOwnerConfigurable::Update()
{

    Notify();
} 