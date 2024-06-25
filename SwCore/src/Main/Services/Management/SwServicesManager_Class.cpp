/*!
 \file SwServicesManager_Class.h
 \brief Implementation of the Class SwServicesManager_Class
 \version 1.0
 \date 23-août-2006 16:04:57
 \author F.Bighelli
 */
#include <QtGlobal>
#include "Main/SwMacros.h"
#include "Main/Services/Management/SwServicesManager_Class.h"
#include <QCoreApplication>
#include "qmessagebox.h"

using namespace StreamWork::SwCore;


//---------------------------------------------------------------------------------
SwServicesManager_Class::SwServicesManager_Class()
{
    _services.clear();
    _ordered_services.clear();
}

//---------------------------------------------------------------------------------
SwServicesManager_Class::~SwServicesManager_Class()
{
    //LiberateAllServices(); fait par le composant
}

//---------------------------------------------------------------------------------
void SwServicesManager_Class::LiberateAllServices()
{
    QMap<QString, ISwService *>::iterator it;
    ISwService * service;

    for (it = _services.begin(); it != _services.end(); it++)
    {
        service = it.value();
        service->Liberate();
    }
    _services.clear();
    _ordered_services.clear();
}

//---------------------------------------------------------------------------------
QList<QString> SwServicesManager_Class::GetServicesList()
{
    return _ordered_services;
}

//---------------------------------------------------------------------------------
ISwService * SwServicesManager_Class::QueryService(QString service_name)
{
    QMap<QString, ISwService *>::iterator it;

    it = _services.find(service_name);
    if (it != _services.end())
    {
        return it.value();
    }

    // le service n'a pas été trouvé!!
    raiseQueryError(service_name);

    return NULL;
}

//---------------------------------------------------------------------------------
void SwServicesManager_Class::RegisterService(ISwService * service) throw(SwException)
{
    QMap<QString, ISwService *>::iterator it;

    Q_ASSERT(service != NULL);
    it = _services.find(service->GetServiceName());
    if (it != _services.end())
    {
        QString msg = QString("A service with %1 already registered").arg(service->GetServiceName());
        QMessageBox::critical(NULL, "SwCore", msg);
        return;
    }
    _services.insert(service->GetServiceName(), service);
    _ordered_services.push_back(service->GetServiceName());
    service->InitializeService();
    for (int i = 0; i < _servicesObservers.count(); i++)
    {
        _servicesObservers[i]->OnRegisterService(service);
    }
}

//---------------------------------------------------------------------------------
void SwServicesManager_Class::UnregisterService(QString service_name) throw(SwException)
{
    ISwService * service = _services.value(service_name);
    if (!service)
    {
        QString msg = QString("Try to unregister unknown service %1").arg(service_name);
        LAUNCH_SWEXCEPTION("SwCore", msg);
    }    

    // Copie de la liste des observers au cas ou un observer se désenregistre à l'appel de OnUnregisterService
    QList<ISwServicesManager_Listener *> observersCopy = _servicesObservers;
    // On stock la liste des observers détruit suite à l'appel de OnUnregisterService pour ne pas notifier un observer détruit.
    QSet<ISwServicesManager_Listener*> currentDeletedObservers;
    _deletedObservers.insert(&currentDeletedObservers);
    for (ISwServicesManager_Listener* observer : observersCopy)
    {
        if(!currentDeletedObservers.contains(observer))
            observer->OnUnregisterService(service);
    }
    _deletedObservers.remove(&currentDeletedObservers);

    service->Liberate();

    _services.remove(service_name);

    _ordered_services.removeAll(service_name);
}

//---------------------------------------------------------------------------------
void SwServicesManager_Class::AddServicesManagerObserver(ISwServicesManager_Listener * observer)
{
    if (_servicesObservers.indexOf(observer) >= 0)
        return;

    _servicesObservers.push_back(observer);
}

//---------------------------------------------------------------------------------
void SwServicesManager_Class::RemoveServicesManagerObserver(ISwServicesManager_Listener * observer)
{
    //Ajoute un observerSi cette méthode est appelé pendant la boucle de notification de UnregisterService
    for(auto list : _deletedObservers)
        list->insert(observer);
    _servicesObservers.removeAll(observer);
}
