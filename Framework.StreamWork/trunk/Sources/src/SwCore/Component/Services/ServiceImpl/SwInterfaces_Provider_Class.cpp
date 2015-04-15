/*!
\file SwInterfaces_Provider_Class.cpp
\date 12/04/2006
\brief implementation du service fournisseur d'interfaces
\author  Big
\version 1.0
 */

#include "_SwProvidedInterfaceContainer_Class.h"
#include "SwInterfaces_Provider_Class.h"
#include "ISwInterfaces_Consumer.h"

using namespace StreamWork::SwCore;


/*! \brief Construsteur */
SwInterfaces_Provider_Class::SwInterfaces_Provider_Class(SwComponent_Class * host) {
    _host_component=host;
    _interfaces.clear();
    _current_interface=_interfaces.end();
    _current_interface_container=NULL;
}
/*! \brief Destrusteur */
SwInterfaces_Provider_Class::~SwInterfaces_Provider_Class(){
    Liberate();
}
/*! \brief Enregistre une interface a fournir */
void SwInterfaces_Provider_Class::RegisterProvidedInterfaceWithType(QString pinterface_name,QString pinterface_type,void * handle_interface) throw(SwException){
    QMap<QString,_SwProvidedInterfaceContainer_Class *>::const_iterator it; 
    _SwProvidedInterfaceContainer_Class * interface_container;

    it=_interfaces.find(pinterface_name);
    if (it!=_interfaces.end()) {
        QString msg=QString("In component %2\nFailed to register provided interface %1 because an other provided interface have same name").arg(pinterface_name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    interface_container=new _SwProvidedInterfaceContainer_Class(this);
    interface_container->Define(pinterface_name,pinterface_type);
    interface_container->SetInterface(handle_interface);
    _interfaces.insert(pinterface_name,interface_container); 
    //Signal ajout d'interface
    for (QSet<ISwInterfaces_ServicesListener *>::const_iterator its=_services_observers.begin();its!=_services_observers.end();its++) {
        (*its)->OnAddInterface(this,pinterface_name);
    }

}
/*! \brief Desenregistre une interface fournie */
void SwInterfaces_Provider_Class::UnregisterProvidedInterface(QString pinterface_name) throw(SwException){
    QMap<QString,_SwProvidedInterfaceContainer_Class *>::iterator it; 

    it=_interfaces.find(pinterface_name);
    if (it==_interfaces.end()) {
        QString msg=QString("In component %2\nFailed to unregister provided interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    //Si on deplace le pointeur courant si c'est celui a supprimer
    if (_current_interface==it) _current_interface++;
    //Si le container courant est celui a supprimer on le remets a zero
    if (_current_interface_container==it.value()) _current_interface_container=NULL;
    //suppression
    delete it.value();
    _interfaces.erase(it);
    //Signal suppression d'interface
    for (QSet<ISwInterfaces_ServicesListener *>::const_iterator its=_services_observers.begin();its!=_services_observers.end();its++) {
        (*its)->OnRemoveInterface(this,pinterface_name);
    }

}
/*! \brief Rend disponible une interface */
void SwInterfaces_Provider_Class::SetInterfaceAvailable(QString pinterface_name,void * new_handle) throw(SwException){
    QMap<QString,_SwProvidedInterfaceContainer_Class *>::iterator it; 

    it=_interfaces.find(pinterface_name);
    if (it==_interfaces.end()) {
        QString msg=QString("In component %2\nFailed to availabilise of interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    if (new_handle!=NULL)
        it.value()->SetInterface(new_handle);
    it.value()->ChangeAvailability(true);
}
/*! \brief Rend undisponible une interface */
void SwInterfaces_Provider_Class::SetInterfaceUnavailable(QString pinterface_name) throw(SwException){
    QMap<QString,_SwProvidedInterfaceContainer_Class *>::iterator it; 

    it=_interfaces.find(pinterface_name);
    if (it==_interfaces.end()) {
        QString msg=QString("In component %2\nFailed to unavailabilise of interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    it.value()->ChangeAvailability(false);
}
/*! \brief Remplacer une interface (DP strategie capability)*/
void SwInterfaces_Provider_Class::SubstituteInterfaceWithType(QString pinterface_name,QString pinterface_type,void * handle_interface) throw(SwException) {
    QMap<QString,_SwProvidedInterfaceContainer_Class *>::iterator it; 

    it=_interfaces.find(pinterface_name);
    if (it==_interfaces.end()) {
        QString msg=QString("In component %2\nFailed to change interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    it.value()->SetInterface(handle_interface);

}
//---------------------------------------------------------------------
// Interface ISwInterfaces_Provider
//---------------------------------------------------------------------            
/*! \brief Ajoute un consommateur a une interface donnée */
void SwInterfaces_Provider_Class::AddConsumer(ISwInterfaces_Consumer * consumer,QString cinterface_name,QString pinterface_name){
    QMap<QString,_SwProvidedInterfaceContainer_Class *>::iterator it; 

    it=_interfaces.find(pinterface_name);
    if (it==_interfaces.end()) {
        QString msg=QString("In component %2\nFailed to add comsumer to interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    it.value()->RegisterConsumedInterface(consumer,cinterface_name);
    //Signal connection d'interface
    for (QSet<ISwInterfaces_ServicesListener *>::const_iterator its=_services_observers.begin();its!=_services_observers.end();its++) {
        (*its)->OnConnectInterface(this,pinterface_name,consumer,cinterface_name);
    }

}
/*! \brief Supprime un consommateur d'une interface donnée */
void SwInterfaces_Provider_Class::RemoveConsumer(ISwInterfaces_Consumer * consumer,QString cinterface_name,QString pinterface_name){
    QMap<QString,_SwProvidedInterfaceContainer_Class *>::iterator it; 

    it=_interfaces.find(pinterface_name);
    if (it==_interfaces.end()) {
        QString msg=QString("In component %2\nFailed to remove comsumer to interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    it.value()->UnregisterConsumedInterface(consumer,cinterface_name);
    //Signal connection d'interface
    for (QSet<ISwInterfaces_ServicesListener *>::const_iterator its=_services_observers.begin();its!=_services_observers.end();its++) {
        (*its)->OnDisconnectInterface(this,pinterface_name,consumer,cinterface_name);
    }
}
/*! \brief Permet de recuperer le type de l'interface (via typeid)*/
QString SwInterfaces_Provider_Class::GetInterfaceType(QString pinterface_name){
    QMap<QString,_SwProvidedInterfaceContainer_Class *>::iterator it; 

    it=_interfaces.find(pinterface_name);
    if (it==_interfaces.end()) {
        QString msg=QString("In component %2\nFailed to get type of interface %1 because it's undefined").arg(pinterface_name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    return it.value()->GetType();
}
/*! \brief Permet de savoir si une interface existe*/
bool SwInterfaces_Provider_Class::InterfaceExist(QString pinterface_name) {
    QMap<QString,_SwProvidedInterfaceContainer_Class *>::iterator it; 

    it=_interfaces.find(pinterface_name);
    if (it==_interfaces.end()) 
        return false;
    else
        return true;
}
    /*! \brief Permet de connaitre la premiere interface( si retour isNull, plus d'interface) */
QString SwInterfaces_Provider_Class::GetFirstInterface() {
    QString result;

    _current_interface=_interfaces.begin();
    if (_current_interface!=_interfaces.end()) {
        result=_current_interface.key();
        _current_interface++;
    } else {
        result=QString();
    }
    return result;
}
/*! \brief Permet de connaitre l'interface suivante ( si retour isNull, plus d'interface) */
QString SwInterfaces_Provider_Class::GetNextInterface() {
    QString result;

    if (_current_interface!=_interfaces.end()) {
        result=_current_interface.key();
        _current_interface++;
    } else {
        result=QString();
    }
    return result;
}
/*! \brief Permet de connaitre le premier consommateur et son interface consommé pour une interface( si retour NULL, plus de consommateur) */
ISwInterfaces_Consumer * SwInterfaces_Provider_Class::GetFirstConsumer(QString pinterface_name,QString * pt_cinterfacename) {
    QMap<QString,_SwProvidedInterfaceContainer_Class *>::iterator it; 

    it=_interfaces.find(pinterface_name);
    if (it==_interfaces.end()) {
        _current_interface_container=NULL;
        return NULL;
    } 
    _current_interface_container=it.value();
    return _current_interface_container->GetFirstConsumer(pt_cinterfacename);
}
/*! \brief Permet de connaitre le consommateur suivant et son interface consommé pour une interface( si retour NULL, plus de consommateur) */
ISwInterfaces_Consumer * SwInterfaces_Provider_Class::GetNextConsumer(QString * pt_cinterfacename) {
    
    if (_current_interface_container!=NULL)
        return _current_interface_container->GetNextConsumer(pt_cinterfacename);
    return NULL;
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_Service
//---------------------------------------------------------------------            
/*! \brief Ajout d'un listener */
void SwInterfaces_Provider_Class::AttachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener) {
    QSet<ISwInterfaces_ServicesListener *>::const_iterator it;

    it=_services_observers.find(listener);
    if (it!=_services_observers.end()) {
        QString msg=QString("In component %2\nFailed to attach an already attached interfaces services listener").arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    _services_observers.insert(listener);
}
/*! \brief Retrait d'un listener */
void SwInterfaces_Provider_Class::DetachInterfacesServices_Listener(ISwInterfaces_ServicesListener * listener) {
    QSet<ISwInterfaces_ServicesListener *>::iterator it;

    it=_services_observers.find(listener);
    if (it==_services_observers.end()) {
        QString msg=QString("In component %2\nFailed to detach an undefined attached interfaces services listener").arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    _services_observers.erase(it);
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void SwInterfaces_Provider_Class::Liberate(){
    QMap<QString,_SwProvidedInterfaceContainer_Class *>::iterator it;
    
    for (it=_interfaces.begin();it!=_interfaces.end();it++) {
        delete it.value();
        //Signal suppression d'interface
        for (QSet<ISwInterfaces_ServicesListener *>::const_iterator its=_services_observers.begin();its!=_services_observers.end();its++) {
            (*its)->OnRemoveInterface(this,it.key());
        }
    }
    _interfaces.clear();
}  
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * SwInterfaces_Provider_Class::GetHostComponent(){
    return _host_component;
}           
