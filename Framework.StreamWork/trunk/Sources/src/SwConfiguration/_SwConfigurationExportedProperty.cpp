/*!
 \file _SwConfigurationExportedProperty.cpp
 \brief Class implementant une entite exportable property
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "_SwConfigurationExportedProperty.h"

/*! \brief Constructeur */
_SwConfigurationExportedProperty::_SwConfigurationExportedProperty():_SwConfigurationExportedEntity() {
	_type=Ent_Property;
    _internal_properties=NULL;
    _external_properties=NULL;
    _internal_property=NULL;
    _external_property=NULL;
    _propagate_change_on=true;
}
/*! \brief Destructeur */
_SwConfigurationExportedProperty::~_SwConfigurationExportedProperty() {

}
/*! \brief Specific bind */
void _SwConfigurationExportedProperty::SpecificBind(){
    _internal_properties=dynamic_cast<ISwProperties *>(_host->QueryService(CG_SW_SERVICE_PROPERTIES));    
    _external_properties=dynamic_cast<SwProperties_Class *>(_model->QueryService(CG_SW_SERVICE_PROPERTIES)); 
    if (_internal_properties==NULL || _external_properties==NULL) {
        return;
    }
    //Connection des signaux
    _internal_properties->GetOnCreateProperty().iconnect(*this,&_SwConfigurationExportedProperty::OnCreateProperty);
    _internal_properties->GetOnDestroyProperty().iconnect(*this,&_SwConfigurationExportedProperty::OnDestroyProperty);
    //Creation de la propriété si elle existe
    OnCreateProperty(_internal_properties,_internal_properties->GetProperty(_name));
}
/*! \brief Defait la liaison */
void _SwConfigurationExportedProperty::SpecificUnbind(){
    if (_internal_properties==NULL || _external_properties==NULL) {
        return;
    }
    //Destruction de la propriété
    OnDestroyProperty(_internal_properties,_internal_property);
    //Deconnection des signaux
    _internal_properties->GetOnCreateProperty().idisconnect(*this,&_SwConfigurationExportedProperty::OnCreateProperty);
    _internal_properties->GetOnDestroyProperty().idisconnect(*this,&_SwConfigurationExportedProperty::OnDestroyProperty);

}
/*! \brief Slot de connexion */
void _SwConfigurationExportedProperty::OnInternalChange(ISwProperty * p) {
    if (!_propagate_change_on)
        return;
    _propagate_change_on=false;
    _external_property->SetValue(_internal_property->GetValue());    
    _propagate_change_on=true;
}
/*! \brief Slot de connexion */
void _SwConfigurationExportedProperty::OnInternalVisibleChange(ISwProperty * p) {
    _external_properties->ChangePropertyVisibility(_exported_name,_internal_property->IsVisible());
}
/*! \brief Slot de connexion */
void _SwConfigurationExportedProperty::OnInternalEditableChange(ISwProperty * p) {
    _external_properties->ChangePropertyEdition(_exported_name,_internal_property->IsEditable());
}
/*! \brief Slot de connexion */
void _SwConfigurationExportedProperty::OnExternalChange(ISwProperty * p) {
    if (!_propagate_change_on)
        return;
    _propagate_change_on=false;
    _internal_property->SetValue(_external_property->GetValue());    
     QVariant v=_internal_property->GetValue();
     _external_property->SetValue(v);
    _propagate_change_on=true;
}
/*! \brief Sur creation d'une propriété */
void _SwConfigurationExportedProperty::OnCreateProperty(ISwProperties *ps,ISwProperty *p) {
    if (_internal_property==NULL && p!=NULL && p->GetRealName()==_name) {
        _internal_property=p;
        QVariant v=_internal_property->GetValue();
        _external_property=_external_properties->CreatePropertyWithType(v.userType(),_exported_name);
        _external_property->SetValue(v);
        _external_properties->ChangePropertyVisibility(_exported_name,_internal_property->IsVisible());
        _external_properties->ChangePropertyEdition(_exported_name,_internal_property->IsEditable());
        //Etablissement des connections
        _internal_property->GetOnChangeSignal().iconnect(*this,&_SwConfigurationExportedProperty::OnInternalChange);
        _internal_property->GetOnVisibleChangeSignal().iconnect(*this,&_SwConfigurationExportedProperty::OnInternalVisibleChange);
        _internal_property->GetOnEditableChangeSignal().iconnect(*this,&_SwConfigurationExportedProperty::OnInternalEditableChange);
        _external_property->GetOnChangeSignal().iconnect(*this,&_SwConfigurationExportedProperty::OnExternalChange);
    }
}
/*! \brief Sur destruction d'une propriété */
void _SwConfigurationExportedProperty::OnDestroyProperty(ISwProperties *ps,ISwProperty *p) {
    if (_internal_property!=NULL && p==_internal_property) {
        //Si la propriété existe on la detruit
        _internal_property->GetOnChangeSignal().idisconnect(*this,&_SwConfigurationExportedProperty::OnInternalChange);
        _internal_property->GetOnVisibleChangeSignal().idisconnect(*this,&_SwConfigurationExportedProperty::OnInternalVisibleChange);
        _internal_property->GetOnEditableChangeSignal().idisconnect(*this,&_SwConfigurationExportedProperty::OnInternalEditableChange);
        _internal_property=NULL;
        _external_property->GetOnChangeSignal().idisconnect(*this,&_SwConfigurationExportedProperty::OnExternalChange);
        _external_properties->DestroyProperty(_exported_name);
        _external_property=NULL;
    }
}
