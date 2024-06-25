/**
@file SwExtensionHelper.cpp
@brief Extension helper for easy management
@author Big
 */

#include "Extensions/SwExtensionHelper.h"
#include "Component/Services/ISwInterfaces_Provider.h"
#include "Component/Services/ISwInterfaces_Provider.h"
#include "Extensions/SwExtensionImpl.h"
#include "Component/Services/ISwInterfaces_Consumer.h"
#include "Extensions/_SwExtensionPointImpl.h"

using namespace StreamWork::SwCore;

/** @brief Constructor */
SwExtensionHelper::SwExtensionHelper(SwComponent_Class * component) {
	_component=component;
}
/** @brief Destructor */
SwExtensionHelper::~SwExtensionHelper() {
  //TO DO destruction
}
/** @brief Renvoie une liste des points d'extension a un instant t, les objects doivent etre liberé apres usages */
QList<ISwExtensionPoint *> SwExtensionHelper::getAllExtensionPoints() {
    QList<ISwExtensionPoint *> liste_extensionPoints;
    ISwInterfaces_Consumer *iconsumer=dynamic_cast<ISwInterfaces_Consumer *>(_component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (iconsumer!=0) {
        QString type;
        QString name=iconsumer->GetFirstInterface(&type,0,0);
        while (!name.isNull()) {
            liste_extensionPoints.push_back(new _SwExtensionPointImpl(type,name,_component));
            name=iconsumer->GetNextInterface(&type,0,0);
        }  
    }
    return liste_extensionPoints;
}  
/** @brief Renvoie un point d'extension donnée */
ISwExtensionPoint * SwExtensionHelper::getExtensionPoint(QString type,QString name) {
    QList<ISwExtensionPoint *> liste_extensionPoints;
    ISwInterfaces_Consumer *iconsumer=dynamic_cast<ISwInterfaces_Consumer *>(_component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (iconsumer!=0) {
        QString itype;
        QString iname=iconsumer->GetFirstInterface(&itype,0,0);
        while (!iname.isNull()) {
            if (!iname.isNull() && iname==name && itype==type) {
                return new _SwExtensionPointImpl(type,name,_component);
            } else {
                iname=iconsumer->GetNextInterface(&itype,0,0);
            } 
        }
    }
    return 0;

}
/** @brief Renvoie une liste des extension a un instant t, les objects doivent etre liberé apres usages */
QList<ISwExtension *> SwExtensionHelper::getAllExtensions() {
    QList<ISwExtension *> liste_extensions;
    ISwInterfaces_Provider *iprovider=dynamic_cast<ISwInterfaces_Provider *>(_component->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    if (iprovider!=0) {
        QString name=iprovider->GetFirstInterface();
        while (!name.isNull()) {
            QString type=iprovider->GetInterfaceType(name);
            liste_extensions.push_back(new SwExtensionImpl(type,name,_component->GetFactoryName(),_component->GetFactoryComponentName(),_component));
            name=iprovider->GetNextInterface();
        }  
    }
    return liste_extensions;
}     
/** @brief Renvoie une extension donnée */
ISwExtension * SwExtensionHelper::getExtension(QString type,QString name) {
    QList<ISwExtension *> liste_extensions;
    ISwInterfaces_Provider *iprovider=dynamic_cast<ISwInterfaces_Provider *>(_component->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    if (iprovider!=0 && iprovider->InterfaceExist(name) && iprovider->GetInterfaceType(name)==type) {
        return new SwExtensionImpl(type,name,_component->GetFactoryName(),_component->GetFactoryComponentName(),_component);
    }
    return 0;

}
