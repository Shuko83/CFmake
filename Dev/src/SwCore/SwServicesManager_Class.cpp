/*!
 \file SwServicesManager_Class.h
 \brief Implementation of the Class SwServicesManager_Class
 \version 1.0
 \date 23-août-2006 16:04:57
 \author F.Bighelli
*/
#include <QtGlobal>
#include "SwServicesManager_Class.h"

using namespace StreamWork::SwCore;


/*! \brief Constructeur */
SwServicesManager_Class::SwServicesManager_Class() {
    _services.clear();
    _ordered_services.clear();
}
/*! \brief Destructeur */
SwServicesManager_Class::~SwServicesManager_Class(){
    //LiberateAllServices(); fait par le composant
}
/*! \brief Liberation de tous les services enregistres*/
void SwServicesManager_Class::LiberateAllServices() {
    QMap<QString,ISwService *>::iterator it;
    ISwService * service;

    for(it=_services.begin();it!=_services.end();it++) {
        service=it.value();
        //qDebug("------------------ >Destroy service: %s",service->GetServiceName().toLatin1().data());
        service->Liberate();
    }
    _services.clear();
    _ordered_services.clear();
}
/*! \brief Renvoie la liste des noms des services enregistrés
\return la liste des noms des services*/
QList<QString> SwServicesManager_Class::GetServicesList(){
    return _ordered_services;
}
/*! \brief Demande un service
\param[in] service_name nom du service enregistré
\return handle sur le service ou NULL si inconnu*/
ISwService * SwServicesManager_Class::QueryService(QString service_name){
    QMap<QString,ISwService *>::iterator it;

    it=_services.find(service_name);
    if (it!=_services.end()) {
        return it.value();
    }
    return NULL;
}
/*! \brief Enregistre un service
\param[in] service service a enregistrer
\exception SwException A service already registered*/
void SwServicesManager_Class::RegisterService(ISwService * service) throw(SwException){
    QMap<QString,ISwService *>::iterator it;

    Q_ASSERT(service!=NULL) ;
    it=_services.find(service->GetServiceName());
    if (it!=_services.end()) {
        QString msg=QString("A service with %1 already registered").arg(service->GetServiceName());
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    _services.insert(service->GetServiceName(),service);
    _ordered_services.push_back(service->GetServiceName());
    service->InitializeService();
    for(int i=0;i<_servicesObservers.count();i++) {
        _servicesObservers[i]->OnRegisterService(service);
    }
}
/*! \brief Desenregistre un service
\param[in] service_name nom du service a desenregistrer
\exception SwException Try to unregister unknown service */
void SwServicesManager_Class::UnregisterService(QString service_name) throw(SwException){
    QMap<QString,ISwService *>::iterator it;
    QList<QString>::iterator itd;

    it=_services.find(service_name);
    if (it==_services.end()) {
        QString msg=QString("Try to unregister unknown service %1").arg(service_name);
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    //qDebug("------- > UnregisterService: %s",it.value()->GetServiceName().toLatin1().data());
    for(int i=0;i<_servicesObservers.count();i++) {
        _servicesObservers[i]->OnUnregisterService(it.value());
    }
    it.value()->Liberate();
    _services.erase(it);
    itd=_ordered_services.begin();
    while (itd!=_ordered_services.end()) {
        if ((*itd)==service_name) {
            _ordered_services.erase(itd);
            return;
        }
        itd++;
    }

}
/*! \brief ajoute un observer */
void SwServicesManager_Class::AddServicesManagerObserver(ISwServicesManager_Listener * observer) {
    if (_servicesObservers.indexOf(observer)>=0) {
        QString msg=QString("Try to register a service manager observer already registered");
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    _servicesObservers.push_back(observer);
}
/*! \brief supprime un observer */
void SwServicesManager_Class::RemoveServicesManagerObserver(ISwServicesManager_Listener * observer) {
    int index=_servicesObservers.indexOf(observer);
    if (index>0) {
        _servicesObservers.removeAt(index);
    }
}

