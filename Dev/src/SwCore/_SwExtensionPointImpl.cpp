/**;
@file _SwExtensionPointImpl.cpp
@brief Extension point implementation
@author Big
 */

#include "_SwExtensionPointImpl.h"
#include "ISwInterfaces_Consumer.h" 
#include "ISwInterfaces_Provider.h" 
#include "_SwExtensionImpl.h"

using namespace StreamWork::SwCore;

/** @brief Constructor */
_SwExtensionPointImpl::_SwExtensionPointImpl(QString type,QString name,SwComponent_Class * component) {
	_type=type;
    _name=name;
    _component=component;
}
/** @brief Destructor */
_SwExtensionPointImpl::~_SwExtensionPointImpl() {
  //TO DO destruction
}
/** @brief renvoie le type */
QString _SwExtensionPointImpl::getType(){
    return _type;
}

/** @brief renvoie le nom */
QString _SwExtensionPointImpl::getName(){
    return _name;
}

/** @brief renvoie le composant (le status doit etre concret)*/
SwComponent_Class * _SwExtensionPointImpl::getComponent(){
    return _component;
}

/** @brief renvoie vrai s'il y a une extension derriere le point d'extension*/
ISwExtension * _SwExtensionPointImpl::getExtension() {
    ISwInterfaces_Consumer *iconsumer=dynamic_cast<ISwInterfaces_Consumer *>(_component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (iconsumer!=0) {
        QString itype;
        ISwInterfaces_Provider * provider;
        QString pname;
        QString iname=iconsumer->GetFirstInterface(&itype,&provider,&pname);
        while (!iname.isNull()) {
            if (!iname.isNull() && iname==_name && itype==_type) {
                if (provider!=0) {
                    return new _SwExtensionImpl(_type,_name,provider->GetHostComponent()->GetFactoryComponentName(),provider->GetHostComponent());
                } else {
                    return 0;
                }
            } else {
                iname=iconsumer->GetNextInterface(&itype,&provider,&pname);
            } 
        }
    }
    return 0;
}
/** @brief etends avec l'extension passé en parametre*/
void _SwExtensionPointImpl::extends(ISwExtension * ext){
    ISwInterfaces_Provider *iprovider=dynamic_cast<ISwInterfaces_Provider *>(ext->getComponent()->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    ISwInterfaces_Consumer *iconsumer=dynamic_cast<ISwInterfaces_Consumer *>(_component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (iprovider!=0 && iconsumer!=0) {
        try {
            iconsumer->AttachProvider(iprovider,_name,ext->getName());
        } catch(SwException & /*se*/) {
            //L'application a levé une exception
            //QMessageBox::warning(0,QString("Warning... "),QString(se.what()),QMessageBox::Abort,QMessageBox::NoButton,QMessageBox::NoButton);
        }
    }
}

/** @brief supprimer l'extension*/
ISwExtension * _SwExtensionPointImpl::removeExtension(){
    ISwExtension * ext=getExtension();
    if (ext==0) {
        return 0;
    }
    ISwInterfaces_Consumer *iconsumer=dynamic_cast<ISwInterfaces_Consumer *>(_component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (iconsumer!=0) {
        iconsumer->DetachProvider(_name);
    }
    return ext;
}
