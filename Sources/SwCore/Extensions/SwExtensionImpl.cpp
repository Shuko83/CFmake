/**
@file SwExtensionImpl.cpp
@brief Extension implementation
@author Big
 */

#include "SwExtensionImpl.h"
#include "SwMacros.h"
using namespace StreamWork::SwCore;

/** @brief Constructor */
SwExtensionImpl::SwExtensionImpl(QString type,QString name,QString pluginName,QString componentTypeName){
    _type=type;
    _name=name;
	_pluginName=pluginName;
	_componentTypeName=componentTypeName;
    _component=0;
    _status=Abstract;
}
/** @brief Constructor */
SwExtensionImpl::SwExtensionImpl(QString type,QString name,QString pluginName,QString componentTypeName,SwComponent_Class * component){
    _type=type;
    _name=name;
	_pluginName=pluginName;
	_componentTypeName=componentTypeName;
    _component=component;
    _status=Concrete;
}
/** @brief Destructor */
SwExtensionImpl::~SwExtensionImpl() {
  //TO DO destruction
}
/** @brief renvoie le status */
SwExtensionImpl::ExtensionStatus SwExtensionImpl::getStatus() {
    return _status;
}

/** @brief renvoie le type */
QString SwExtensionImpl::getType(){
    return _type;
}

/** @brief renvoie le nom */
QString SwExtensionImpl::getName(){
    return _name;
}

/** @brief renvoie le nom du plugin*/
QString SwExtensionImpl::getPluginName() {
	return _pluginName;
}

/** @brief renvoie le nom du type de composant*/
QString SwExtensionImpl::getComponentType(){
    return _componentTypeName;
}

/** @brief renvoie le composant (le status doit etre concret)*/
SwComponent_Class * SwExtensionImpl::getComponent(){
    return _component;
}

/** @brief renvoie une extension concrete (si elle est concrete, renvoie elle meme)*/
ISwExtension * SwExtensionImpl::concretise(SwComponent_Class * sourceComponent){
    if (_status==Concrete) {
        return this;
    }
    SwRefPtr<SwComponent_Class> ncomponent=SW_APP->ComponentsBank().CreateComponent(_pluginName, _componentTypeName);
    if (ncomponent.get()!=0) {
        SwComponent_Class * parentComponent=sourceComponent->GetParent();
        if (parentComponent==0) {
            parentComponent=sourceComponent;
        }
        ncomponent->SetName(parentComponent->GetSuggestedNameForChild(ncomponent->GetName()));
        parentComponent->AddChild(ncomponent);
        return new SwExtensionImpl(_type,_name,_pluginName,_componentTypeName,ncomponent.get());
    } else {
        return 0;
    }
}
