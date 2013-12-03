/*!
\file _SwPropertyImpl_Class.cpp
\date 18/04/2006
\brief implementation generale d'une propriete
\author  Big
\version 1.0
 */

/*
 * INCLUDES LOCAUX
 */
#include "_SwPropertyImpl_Class.h"
#include "SwException.h"
#include "SwMacros.h"

#include "SwEnum.h"
#include "SwIntegerEnum.h"
#include "SwInteger.h"
#include "SwDouble.h"
#include "SwString.h"
#include "SwFileDescriptor.h"
#include "SwIconDescriptor.h"
#include "SwIpV4Address.h"
#include "SwUUID.h"

using namespace StreamWork::SwCore;

/*! \brief Constructor */
_SwPropertyImpl_Class::_SwPropertyImpl_Class(QString name,ISwProperties * hosting_service) {
    _hosting_service=hosting_service;
    _name=name;
    _value=QVariant();
    _initialValue=QVariant();
    _hasBeenInitialed=false;
    _is_visible=true;
    _is_editable=true;
    _has_changed=false;
    _adapters=NULL;
    _is_controllable=false;
    _controller=NULL;
}
/*! \brief Destructor */
_SwPropertyImpl_Class::~_SwPropertyImpl_Class(){
    if (_adapters!=NULL) _adapters->Liberate();
    if (_controller!=NULL) _controller->DetachControl();
}
/*! \brief methode de recuperer le service de propriété parent*/
ISwProperties * _SwPropertyImpl_Class::GetHostingService() {
    return _hosting_service;
}
/*! \brief methode de recuperer le nom d'une propriété */
QString _SwPropertyImpl_Class::GetName(){
    return _name;
}
/*! \brief methode de recuperer le nom reel d'une propriété */
QString _SwPropertyImpl_Class::GetRealName() {
    return _name;
}

/*! \brief methode de recuperer la valeur d'une propriété */
QVariant _SwPropertyImpl_Class::GetValue(){
    return GetInternalValue();
}
void _SwPropertyImpl_Class::SetValue (const QVariant & val){
    if (!_is_editable)
        return;

	// Ajout CGD : notif et changement que si la valeur de la property est différente
	if(val != _value )
	{
		// on check si le type est UserType et que si la variable a déja été loadée une fois
  		if(_hasBeenInitialed && _value.userType()!=QVariant::Invalid)
  		{
  			bool ret = checkForUserType(val);
  			if(!ret) // si les valeurs des 2 types custom sont différents
  			{
  				SetInternalValue(val);
  				if (_hasBeenInitialed) {
  					_has_changed=true;
  					_OnChangeValue(this);
  				} else {
  					_initialValue=val;
  					_hasBeenInitialed=true;
  				}
			}
  		}
		// Première initialisation de la variable
  		else
		{
			SetInternalValue(val);

			if (_hasBeenInitialed) {
				_has_changed=true;
				_OnChangeValue(this);
			} else {
				_initialValue=val;
				_hasBeenInitialed=true;
			}
		}
	}
}


bool _SwPropertyImpl_Class::checkForUserType (const QVariant & val)
{
	
	if (_value.userType()!=QVariant::Invalid && _value.userType()!=val.userType()) 
	{
		QString s=QString("Unable to change property %1 because types are different %2!=%3").arg(_name).arg(QString(_value.typeName())).arg(QString(val.typeName()));
		LAUNCH_SWEXCEPTION("SwCore",s);
	}

 	if (val.userType() == qMetaTypeId<SwEnum>()) 
	{
 		return (val.value<SwEnum>() == _value.value<SwEnum>());
	}
 	if (val.userType() == qMetaTypeId<SwIntegerEnum>()) 
	{
		return (val.value<SwIntegerEnum>() == _value.value<SwIntegerEnum>());
	}
 	if (val.userType()==qMetaTypeId<SwInteger>()) 
	{
		return (val.value<SwInteger>() == _value.value<SwInteger>());
	}
 	if (val.userType()==qMetaTypeId<SwString>()) 
	{
		return (val.value<SwString>() == _value.value<SwString>());
 	}
	if (val.userType()==qMetaTypeId<SwDouble>()) 
	{
		return (val.value<SwDouble>() == _value.value<SwDouble>());
 	}
	if (val.userType()==qMetaTypeId<SwFileDescriptor>()) 
	{
		return (val.value<SwFileDescriptor>() == _value.value<SwFileDescriptor>());
 	}
	if (val.userType()==qMetaTypeId<SwIconDescriptor>()) 
	{
		return (val.value<SwIconDescriptor>() == _value.value<SwIconDescriptor>());
 	}
	if (val.userType()==qMetaTypeId<SwIpV4Address>()) 
	{
		return (val.value<SwIpV4Address>() == _value.value<SwIpV4Address>());
 	}
	if (val.userType()==qMetaTypeId<SwUUID>()) 
	{
		return (val.value<SwUUID>() == _value.value<SwUUID>());
	}
	return false;
}

/*! \brief methode permettant de definir la valeur d'une propriété par son controller*/
void _SwPropertyImpl_Class::SetValueByController(const QVariant & val) {
    SetInternalValue(val);
    _has_changed=true;
    _OnChangeValue(this);
}
/*! \brief methode de recuperer la valeur d'une propriété en interne*/
QVariant _SwPropertyImpl_Class::GetInternalValue() {
    return _value;
}
/*! \brief methode permettant de definir la valeur d'une propriété en interne*/
void _SwPropertyImpl_Class::SetInternalValue (const QVariant & val) {
    if (_value.userType()!=QVariant::Invalid && _value.userType()!=val.userType()) {
        QString s=QString("Unable to change property %1 because types are different %2!=%3").arg(_name).arg(QString(_value.typeName())).arg(QString(val.typeName()));
        LAUNCH_SWEXCEPTION("SwCore",s);
    }
    _value=val;
}
/*! \brief methode permettant de savoir si la propriété est visible*/
bool _SwPropertyImpl_Class::IsVisible(){
    return _is_visible;
}
/*! \brief methode permettant de savoir si la propriété est editable*/
bool _SwPropertyImpl_Class::IsEditable(){
    return _is_editable;
}
/*! \brief methode permettant de savoir si la propriété est controllable*/
bool _SwPropertyImpl_Class::IsControllable() {
    return _is_controllable;
}
/*! \brief methode permettant de savoir si la propriété est controlée*/
bool _SwPropertyImpl_Class::IsControlled() {
    return _controller!=NULL;
}
/*! \brief methode est une sous propriété*/
bool _SwPropertyImpl_Class::IsSubProperty() {
    return false;
}
/*! \brief methode permettant d'acceder au controller*/
ISwController * _SwPropertyImpl_Class::GetController() {
    return _controller;
}
/*! \brief methode d'acces au signal de changement*/
LibIndeSig::iSignal1<ISwProperty *> & _SwPropertyImpl_Class::GetOnChangeSignal(){
    return _OnChangeValue;
}	
/*! \brief methode d'acces au signal de changement*/
LibIndeSig::iSignal1<ISwProperty *> & _SwPropertyImpl_Class::GetOnVisibleChangeSignal() {
    return _OnVisibleChangeValue;
}
/*! \brief methode d'acces au signal de changement*/
LibIndeSig::iSignal1<ISwProperty *> & _SwPropertyImpl_Class::GetOnEditableChangeSignal() {
    return _OnEditableChangeValue;
}
/*! \brief methode d'acces au signal de changement de controle*/
LibIndeSig::iSignal3<ISwProperty *,ISwController *,ISwController *> & _SwPropertyImpl_Class::GetOnControlChangeSignal() {
    return _OnControlChange;
}
/*! \brief methode permettant de changer la visibilite de la propriété*/
void _SwPropertyImpl_Class::SetIsVisible(bool is_visible) {
    _is_visible=is_visible;
    _OnVisibleChangeValue(this);
}
/*! \brief methode permettant de changer l'edition de la propriété*/
void _SwPropertyImpl_Class::SetIsEditable(bool is_editable) {
    _is_editable=is_editable;
    _OnEditableChangeValue(this);
}
/*! \brief methode permettant de changer l'aspect controlable de la propriété*/
void _SwPropertyImpl_Class::SetControllable(bool is_controllable) {
    _is_controllable=is_controllable;
}
/*! \brief methode permettant de definir le controller*/
void _SwPropertyImpl_Class::SetController(ISwController * controller) {
    ISwController * old_controller=_controller; 
    _controller=controller;
    _OnControlChange(this,_controller,old_controller);
}
/*! \brief methode permettant de savoir si la propriété a changer par rapport a sa configuration usine*/
bool _SwPropertyImpl_Class::HasChanged() {
    return _has_changed;
}
/*! \brief methode permettant de marquer une propriété comme si elle n'avait pas changer*/
void _SwPropertyImpl_Class::MarkAsUnchanged(){
    _has_changed=false;
}
/*! \brief methode permettant de marquer une propri?t? comme si elle avait  changer et genere le signal associé*/
void _SwPropertyImpl_Class::MarkAsChanged() {
    _has_changed=true;
    _OnChangeValue(this);
}
/*! \brief Definition de la description*/
void _SwPropertyImpl_Class::SetDescription(QString desc) {
    _desc=desc;
}
/*! \brief Acces a la description*/
QString _SwPropertyImpl_Class::GetDescription() {
    return _desc;
}
/*! \brief Icon*/
void _SwPropertyImpl_Class::SetIcon(QIcon & icon) {
    if (!_is_editable)
        return;
    _icon=icon;
    _has_changed=true;
    _OnChangeValue(this);

}
/*! \brief Icon*/
QIcon & _SwPropertyImpl_Class::GetIcon(){
    return _icon;
}
/*! \brief value Icon*/
void _SwPropertyImpl_Class::SetValueIcon(QIcon & icon){
    if (!_is_editable)
        return;
    _valueIcon=icon;
    _has_changed=true;
    _OnChangeValue(this);
}
/*! \brief value Icon*/
QIcon & _SwPropertyImpl_Class::GetValueIcon(){
    return _valueIcon;
}
/*! \brief Assignation d'adapteurs*/
void _SwPropertyImpl_Class::SetComplexeTypeAdapters(ISwComplexeTypeAdapters * adapters) {
    if (_adapters!=NULL) _adapters->Liberate();
    _adapters=adapters;
    if (_adapters!=NULL) _adapters->Initialize(this);
}
/*! \brief Recuperation d'adapteurs*/
ISwComplexeTypeAdapters * _SwPropertyImpl_Class::GetComplexeTypeAdapters() {
    return _adapters;
}
/*! \brief prpriete resettable*/
bool _SwPropertyImpl_Class::isResettable() {
    return true;
}
/*! \brief reset de la propriété*/
void _SwPropertyImpl_Class::reset() {
    SetInternalValue(_initialValue);
    _has_changed=false;
    _OnChangeValue(this);
}


