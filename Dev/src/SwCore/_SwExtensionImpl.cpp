/**
@file _SwExtensionImpl.cpp
@brief Extension implementation
@author Big
 */

#include "_SwExtensionImpl.h"
#include "SwApplication.h"
using namespace StreamWork::SwCore;

/** @brief Constructor */
_SwExtensionImpl::_SwExtensionImpl(QString type,QString name,QString componentTypeName){
    _type=type;
    _name=name;
    _componentTypeName=componentTypeName;
    _component=0;
    _status=Abstract;
}
/** @brief Constructor */
_SwExtensionImpl::_SwExtensionImpl(QString type,QString name,QString componentTypeName,SwComponent_Class * component){
    _type=type;
    _name=name;
    _componentTypeName=componentTypeName;
    _component=component;
    _status=Concrete;
}
/** @brief Destructor */
_SwExtensionImpl::~_SwExtensionImpl() {
  //TO DO destruction
}
/** @brief renvoie le status */
_SwExtensionImpl::ExtensionStatus _SwExtensionImpl::getStatus() {
    return _status;
}

/** @brief renvoie le type */
QString _SwExtensionImpl::getType(){
    return _type;
}

/** @brief renvoie le nom */
QString _SwExtensionImpl::getName(){
    return _name;
}

/** @brief renvoie le nom du type de composant*/
QString _SwExtensionImpl::getComponentType(){
    return _componentTypeName;
}

/** @brief renvoie le composant (le status doit etre concret)*/
SwComponent_Class * _SwExtensionImpl::getComponent(){
    return _component;
}

/** @brief renvoie une extension concrete (si elle est concrete, renvoie elle meme)*/
ISwExtension * _SwExtensionImpl::concretise(SwComponent_Class * sourceComponent){
    if (_status==Concrete) {
        return this;
    }
    SwRefPtr<SwComponent_Class> ncomponent=SW_APP->ComponentsBank().CreateComponent(_componentTypeName);
    if (ncomponent.get()!=0) {
        SwComponent_Class * parentComponent=sourceComponent->GetParent();
        if (parentComponent==0) {
            parentComponent=sourceComponent;
        }
        ncomponent->SetName(parentComponent->GetSuggestedNameForChild(ncomponent->GetName()));
        parentComponent->AddChild(ncomponent);
        return new _SwExtensionImpl(_type,_name,_componentTypeName,ncomponent.get());
    } else {
        return 0;
    }
}
