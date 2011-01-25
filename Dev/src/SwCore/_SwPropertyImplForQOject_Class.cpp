/*!
\file _SwPropertyImplForQOject_Class.cpp
\date 18/04/2006
\brief implementation generale d'une propriete d'un QObject
\author  Big
\version 1.0
 */

/*
 * INCLUDES LOCAUX
 */
#include "_SwPropertyImplForQOject_Class.h"
#include "SwException.h"
#include "SwMacros.h"
#include "SwEnum.h"
using namespace StreamWork::SwCore;

/*! \brief Constructor */
_SwPropertyImplForQOject_Class::_SwPropertyImplForQOject_Class(QObject * host,QString name,ISwProperties * hosting_service,QString prefix):_SwPropertyImpl_Class(name,hosting_service) {
    _host=host;
    _iconDesc=0;
    if (prefix!=NULL) {
        _real_name=QString("%1.%2").arg(prefix).arg(name);
    } else {    
        _real_name=_name;
    }
    _metaproperty=_host->metaObject()->property(_host->metaObject()->indexOfProperty(name.toLatin1().data()));
    //_is_editable=_metaproperty.isWritable();
    if (_metaproperty.isEnumType()) {
        QMetaEnum meta_enum=_metaproperty.enumerator();
        //Creation du variant base sur enum
        SwEnum tmp_enum;
        tmp_enum.ChangeFlagStatus(_metaproperty.isFlagType());
        //Remplissage des valeurs de clefs
        for(int i=0;i<meta_enum.keyCount();i++) {
            if (meta_enum.value(i)!=-1) {
                tmp_enum.AddKey(meta_enum.value(i),QString(meta_enum.key(i)));    
            }
        }
        //Affectation de la valeur
        tmp_enum.FromInt(_metaproperty.read(_host).toInt());
        //Enregistrement du variant
        _value.setValue(tmp_enum);
    } else if (_metaproperty.type()==QVariant::Icon) {
        _iconDesc =new SwIconDescriptor();
        //Enregistrement du variant
        _value.setValue(*_iconDesc);
    } else {
        //Enregistrement du variant
        _value=_metaproperty.read(_host);   
    }
    if (!_metaproperty.isWritable()) {
        _is_editable=false;
    }
    
}
/*! \brief Destructor */
_SwPropertyImplForQOject_Class::~_SwPropertyImplForQOject_Class(){
}
/*! \brief methode de recuperer le nom reel d'une propriété */
QString _SwPropertyImplForQOject_Class::GetRealName() {
    return _real_name;
}
/*! \brief methode de recuperer la valeur d'une propriété en interne*/
QVariant _SwPropertyImplForQOject_Class::GetInternalValue() {
    if (_metaproperty.isEnumType()) {
        SwEnum tmp_enum=_value.value<SwEnum>();
        tmp_enum.FromInt(_metaproperty.read(_host).toInt());
        _value.setValue(tmp_enum);
    } else if (_metaproperty.type()==QVariant::Icon) {
        //Enregistrement du variant
        _value.setValue(*_iconDesc);
    } else {
        _value=_metaproperty.read(_host);
    }
    return _value;
}
/*! \brief methode permettant de definir la valeur d'une propriété en interne*/
void _SwPropertyImplForQOject_Class::SetInternalValue (const QVariant & val) {
    if (_value.userType()!=val.userType()) {
         QString s=QString("Unable to change property %1 because types are different %2!=%3").arg(_name).arg(QString(_value.typeName())).arg(QString(val.typeName()));
        LAUNCH_SWEXCEPTION("SwCore",s);       
    }
    _value=val;   
    if (_metaproperty.isEnumType()) {  
        SwEnum tmp_enum=_value.value<SwEnum>();
        _metaproperty.write(_host,QVariant(tmp_enum.ToInt()));
    } else if (_metaproperty.type()==QVariant::Icon && _value.userType()==qMetaTypeId<SwIconDescriptor>()) {
        *_iconDesc=_value.value<SwIconDescriptor>();
        _metaproperty.write(_host,_iconDesc->ToIcon());      
    } else {
        _metaproperty.write(_host,_value);       
    }
}
