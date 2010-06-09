/*!
\file _SwPropertyImplSimpleTypeAdapter_Class.cpp
\date 18/04/2006
\briefimplementation generale d'une propriete base sur un simple type adapter
\author  Big
\version 1.0
 */

/*
 * INCLUDES LOCAUX
 */
#include "_SwPropertyImplSimpleTypeAdapter_Class.h"
#include "SwException.h"
#include "SwMacros.h"
#include "SwEnum.h"
using namespace StreamWork::SwCore;

/*! \brief Constructor */
_SwPropertyImplSimpleTypeAdapter_Class::_SwPropertyImplSimpleTypeAdapter_Class(ISwSimpleTypeAdapter * adapter,QString name,ISwProperties * hosting_service):_SwPropertyImpl_Class(name,hosting_service) {
    _adapter=adapter;   
	_adapter->Initialize(this);	
}
/*! \brief Destructor */
_SwPropertyImplSimpleTypeAdapter_Class::~_SwPropertyImplSimpleTypeAdapter_Class(){
}
/*! \brief methode de recuperer la valeur d'une propriété en interne*/
QVariant _SwPropertyImplSimpleTypeAdapter_Class::GetInternalValue() {    
    _value=_adapter->Get();
    return _value;
}
/*! \brief methode permettant de definir la valeur d'une propriété en interne*/
void _SwPropertyImplSimpleTypeAdapter_Class::SetInternalValue (const QVariant & val) {
    if (_value.userType()!=val.userType()) {
         QString s=QString("Unable to change property %1 because types are different %2!=%3").arg(_name).arg(QString(_value.typeName())).arg(QString(val.typeName()));
        LAUNCH_SWEXCEPTION("SwCore",s);       
    }
    _value=val;
	_adapter->Set(_value);	
}
/*! \brief methode est une sous propriété*/
bool _SwPropertyImplSimpleTypeAdapter_Class::IsSubProperty() {
    return true;
}
