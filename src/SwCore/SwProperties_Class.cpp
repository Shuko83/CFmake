/*!
\file SwProperties_Class.cpp
\date 12/04/2006
\brief implementation la liste des propriétés
\author  Big
\version 1.0
 */
#include <algorithm>
#include <QMetaObject>
#include "_SwPropertyImpl_Class.h"
#include "_SwPropertyImplForQOject_Class.h"
#include "_SwPropertyImplSimpleTypeAdapter_Class.h"
#include "_SwPropertyPersistent_Toolbox.h"
#include "SwProperties_Class.h"
#include "SwException.h"
#include "SwApplication.h"
#include "SwMacros.h"
#include "ISwComplexeTypeAdaptersFactory.h"
#include "ISwComplexeTypeAdapters.h"
#include "ISwController.h"

using namespace StreamWork::SwCore;


/*! \brief Construsteur */
SwProperties_Class::SwProperties_Class(SwComponent_Class * host) {
    _host_component=host;
    _set_properties.clear();
}
/*! \brief Destrusteur */
SwProperties_Class::~SwProperties_Class(){
    Liberate();
}
//---------------------------------------------------------------------
// Interface ISwProperties
//---------------------------------------------------------------------            
/*! \brief Permet de crée une propriété */
ISwProperty * SwProperties_Class::CreatePropertyWithType(int type_id,QString name) {
    _SwPropertyImpl_Class * property;
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;
    QVariant var;
    void * _my_objet;

    if (name.at(0).toAscii()=='!') {
        QString s=QString("Unable to create property %1 in %2 because already defined because its name begin with !").arg(name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",s);                
    }
    it=_map_properties.find(name);
    if (it!=_map_properties.end()) {
        QString s=QString("Unable to create property %1 in %2 because already defined").arg(name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",s);                
    }
    _my_objet=QMetaType::construct(type_id);
    if (_my_objet==NULL) {
        QString s=QString("Unable to create property %1 in %2 because type undefined").arg(name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",s);                
    }
    _OnBeforeChange(this);
    var=QVariant(type_id,_my_objet);
    property=new _SwPropertyImpl_Class(name,this);
    property->SetInternalValue(var);
    QMetaType::destroy(type_id, _my_objet);
    _map_properties.insert(name,property);
    _set_properties.push_back(property);
    _OnCreateProperty(this,property);
    CreateSubProperties(property);
    property->GetOnControlChangeSignal().iconnect(*this,&SwProperties_Class::OnPropertyControlChange);
    PropagateFeaturesToSubProperties(property);
    _OnAfterChange(this);
    return property;
 
}
/*! \brief Permet de crée une propriété issue d'un object Qt*/
ISwProperty * SwProperties_Class::CreatePropertyForQObject(QObject * obj,QString name,QString prefix) {
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;
    _SwPropertyImpl_Class * property;

   if (prefix!=NULL) 
        it=_map_properties.find(QString("%1.%2").arg(prefix).arg(name)); 
   else 
        it=_map_properties.find(name);
   if (it!=_map_properties.end()) {
        QString s=QString("Unable to create property %1 in %2 because already defined").arg(name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",s);                
   }
   if (obj->property(name.toLatin1().data()).userType()==QVariant::Invalid) {
        QString s=QString("Unable to register property %1 from QObject in %2 because undefined").arg(name).arg(_host_component->GetName());
        LAUNCH_SWEXCEPTION("SwCore",s);        
    }
    property=new _SwPropertyImplForQOject_Class(obj,name,this,prefix);
    if (prefix!=NULL) 
        _map_properties.insert(QString("%1.%2").arg(prefix).arg(name),property);
    else
        _map_properties.insert(name,property);
    _OnBeforeChange(this);
    _set_properties.push_back(property);
    _OnCreateProperty(this,property);
    CreateSubProperties(property);
    property->GetOnControlChangeSignal().iconnect(*this,&SwProperties_Class::OnPropertyControlChange);
    PropagateFeaturesToSubProperties(property);
    _OnAfterChange(this);
    return property;
}
/*! \brief Permet de crée toutes les propriétés issues d'un object Qt*/
void SwProperties_Class::CreatePropertiesForQObject(QObject * obj,QString prefix,bool disable_objectName) {
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;
    _SwPropertyImpl_Class * property;
    const QMetaObject  * m_obj=obj->metaObject();
    bool is_valid_property;

    _OnBeforeChange(this);
    while (m_obj!=NULL) {
        for(int i=0;i<m_obj->propertyCount();i++) {
            //Evaluation de la validite de la propriété
            is_valid_property=m_obj->property(i).userType()!=QVariant::Invalid;
            if (is_valid_property && disable_objectName && m_obj->property(i).name()==QString("objectName"))
                is_valid_property=false;
            //Si valide
            if (is_valid_property) {
                if (prefix!=NULL) 
                    it=_map_properties.find(QString("%1.%2").arg(prefix).arg(m_obj->property(i).name()));
                else
                    it=_map_properties.find(QString(m_obj->property(i).name()));
                if (it==_map_properties.end()) {
                    property=new _SwPropertyImplForQOject_Class(obj,m_obj->property(i).name(),this,prefix);
                    if (prefix!=NULL) 
                        _map_properties.insert(QString("%1.%2").arg(prefix).arg(m_obj->property(i).name()),property);
                    else
                        _map_properties.insert(QString(m_obj->property(i).name()),property);
                    _set_properties.push_back(property);
                    _OnCreateProperty(this,property);
                    CreateSubProperties(property);
                    property->GetOnControlChangeSignal().iconnect(*this,&SwProperties_Class::OnPropertyControlChange);
                    PropagateFeaturesToSubProperties(property);
                }
            }
        }
        m_obj=m_obj->superClass();
    }
    _OnAfterChange(this);
}
/*! \brief Permet de detruire une propriété*/
void SwProperties_Class::DestroyProperty(QString name) {
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;

    it=_map_properties.find(name);
    if (it!=_map_properties.end()) {
        _OnBeforeChange(this);
        it.value()->GetOnControlChangeSignal().idisconnect(*this,&SwProperties_Class::OnPropertyControlChange);
        DestroySubProperties(it.value());
        _OnDestroyProperty(this,(ISwProperty *)it.value());
        _set_properties.removeAt(_set_properties.indexOf((ISwProperty *)it.value()));   
        delete it.value();
        _map_properties.erase(it);
        _OnAfterChange(this);
    }    
}
/*! \brief Permet d'acceder a une propriété*/
ISwProperty * SwProperties_Class::GetProperty(QString name) {
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;

    it=_map_properties.find(name);
    if (it!=_map_properties.end()) 
        return it.value();
    return NULL;
}
/*! \brief Permet d'acceder a la liste des propriétés*/
QList<ISwProperty *> & SwProperties_Class::GetProperties() {
    return _set_properties;
}
/*! \brief Change la visibilite de la propriété
\param[in] name nom de la propriété
\param[in] nouvelle valeur de visibilité*/
void SwProperties_Class::ChangePropertyVisibility(QString name,bool is_visible) {
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;

    it=_map_properties.find(name);
    if (it!=_map_properties.end()) {
        _OnBeforeChange(this);
        it.value()->SetIsVisible(is_visible);
        PropagateFeaturesToSubProperties(it.value());
        _OnAfterChange(this);
    }

}
/*! \brief Change l'editabilite de la propriété
\param[in] name nom de la propriété
\param[in] nouvelle valeur de l'edition*/
void SwProperties_Class::ChangePropertyEdition(QString name,bool is_editable) {
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;

    it=_map_properties.find(name);
    if (it!=_map_properties.end()) {
        _OnBeforeChange(this);
        it.value()->SetIsEditable(is_editable);
        PropagateFeaturesToSubProperties(it.value());
        _OnAfterChange(this);
    }

}
/*! \brief Change l'aspect controllable de la propriété
\param[in] name nom de la propriété
\param[in] nouvelle valeur de l'aspect controllable */
void SwProperties_Class::ChangePropertyControllable(QString name,bool is_controllable) {
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;

    it=_map_properties.find(name);
    if (it!=_map_properties.end()) {
        _OnBeforeChange(this);
        it.value()->SetControllable(is_controllable);
        PropagateFeaturesToSubProperties(it.value());
        _OnAfterChange(this);
    }
}
/*! \brief Permet d'indiquer qu'une property a changé */
void SwProperties_Class::SignalPropertyChange(QString name) {
    ISwProperty * p=GetProperty(name);
    if (p!=0) {
        p->MarkAsChanged();
    }
}
/*! \brief methode d'acces au signal avant changement*/
LibIndeSig::iSignal1<ISwProperties *> & SwProperties_Class::GetOnBeforePropertiesChange() {
    return _OnBeforeChange;
}
/*! \brief methode d'acces au signal apres changement*/
LibIndeSig::iSignal1<ISwProperties *> & SwProperties_Class::GetOnAfterPropertiesChange() {
    return _OnAfterChange;
}
/*! \brief methode d'acces au signal de creation de propriété*/
LibIndeSig::iSignal2<ISwProperties *,ISwProperty *> & SwProperties_Class::GetOnCreateProperty() {
    return _OnCreateProperty;
}
/*! \brief methode d'acces au signal de destruction de propriété*/
LibIndeSig::iSignal2<ISwProperties *,ISwProperty *> & SwProperties_Class::GetOnDestroyProperty() {
    return _OnDestroyProperty;
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void SwProperties_Class::Liberate(){
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;

    for (it=_map_properties.begin();it!=_map_properties.end();it++) {
        it.value()->GetOnControlChangeSignal().idisconnect(*this,&SwProperties_Class::OnPropertyControlChange);
        DestroySubProperties(it.value());
		_OnDestroyProperty(this,(ISwProperty *)it.value());
        delete it.value();
    }
    _map_properties.clear();
    _set_properties.clear();

	foreach(SwProperties_ClassHelper * helper, _helpers)
	{
		helper->unValidate();	
	}
	_helpers.clear();
}  
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * SwProperties_Class::GetHostComponent(){
    return _host_component;
}           
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief methode permettant de charger des donnees */
void SwProperties_Class::Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager) {
    for(QDomElement elt_property = elt.firstChildElement(); !elt_property.isNull(); elt_property = elt_property.nextSiblingElement())
    {
        _SwPropertyPersistent_Toolbox::LoadProperty(elt_property,this);
    }
}
/*! \brief methode permettant de sauver des donnees
(dans l'ordre d'enregistrement des données - important si des properties sont crées lors du load) */
void SwProperties_Class::Save(QDomElement & elt,QDomDocument & doc) {
    QList<ISwProperty *>::iterator it;
    QDomElement elt_property;

    for (it=_set_properties.begin();it!=_set_properties.end();it++) {
        _SwPropertyPersistent_Toolbox::SaveProperty(elt,doc,(*it)->GetRealName(),this);
    }

}
/*! \brief Permet de crée les sous propriétés pour un type complexe */
void SwProperties_Class::CreateSubProperties(_SwPropertyImpl_Class * parent_property) {
    //Check s'il existe un fabrique d'adapteurs pour ce type
    ISwComplexeTypeAdaptersFactory *factory=SW_CTA_FACTORIES.GetFactoryForUserType(parent_property->GetValue().userType());
    if (factory==NULL)
        return;
    ISwComplexeTypeAdapters * adapters=factory->CreateAdapters();
    parent_property->SetComplexeTypeAdapters(adapters);
    //Pour chaque adaptateur, on cree une sous propriété
    QList<ISwSimpleTypeAdapter *> * adapters_list=adapters->GetAdapters();
    for (int i=0;i<adapters_list->count();i++) {
        QString p_name=parent_property->GetRealName()+"."+(*adapters_list)[i]->Label();
        _SwPropertyImplSimpleTypeAdapter_Class * property=new  _SwPropertyImplSimpleTypeAdapter_Class((*adapters_list)[i],p_name,this);   
        _map_properties.insert(p_name,property);
        _set_properties.push_back(property);
        _OnCreateProperty(this,property);
        CreateSubProperties(property);
        property->GetOnControlChangeSignal().iconnect(*this,&SwProperties_Class::OnPropertyControlChange);
   }
}
/*! \brief Permet de detruire les sous propriétés pour un type complexe */
void SwProperties_Class::DestroySubProperties(_SwPropertyImpl_Class * parent_property) {
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;
    //Si la propriété a des adaptateurs
    ISwComplexeTypeAdapters * adapters=parent_property->GetComplexeTypeAdapters();
    if (adapters==NULL)
        return; //Non
    //Pour chaque adaptateur, on detruit la sous propriété associé
    QList<ISwSimpleTypeAdapter *> * adapters_list=adapters->GetAdapters();
    for (int i=0;i<adapters_list->count();i++) {
        QString p_name=parent_property->GetRealName()+"."+(*adapters_list)[i]->Label();
        it=_map_properties.find(p_name);
        if (it!=_map_properties.end()) {
            _SwPropertyImplSimpleTypeAdapter_Class * property=(_SwPropertyImplSimpleTypeAdapter_Class *)it.value();
            property->GetOnControlChangeSignal().idisconnect(*this,&SwProperties_Class::OnPropertyControlChange);
            DestroySubProperties(property);
            _OnDestroyProperty(this,(ISwProperty *)it.value());
            _set_properties.removeAt(_set_properties.indexOf((ISwProperty *)it.value()));   
            delete property;
            _map_properties.erase(it);
        }
    }
}
/*! \brief Permet de propager les caracterisques aux sous propriétés */
void SwProperties_Class::PropagateFeaturesToSubProperties(_SwPropertyImpl_Class * parent_property) {
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;
    //Si la propriété a des adaptateurs
    ISwComplexeTypeAdapters * adapters=parent_property->GetComplexeTypeAdapters();
    if (adapters==NULL)
        return; //Non
    //Pour chaque adaptateur, on propage les caracteristiques aux sous propriétés
    QList<ISwSimpleTypeAdapter *> * adapters_list=adapters->GetAdapters();
    for (int i=0;i<adapters_list->count();i++) {
        QString p_name=parent_property->GetRealName()+"."+(*adapters_list)[i]->Label();
        it=_map_properties.find(p_name);
        if (it!=_map_properties.end()) {
            _SwPropertyImpl_Class * property=it.value();
            if (property->IsVisible()!=parent_property->IsVisible()) property->SetIsVisible(parent_property->IsVisible()); 
            if (property->IsEditable()!=parent_property->IsEditable()) property->SetIsEditable(parent_property->IsEditable()); 
            if (property->IsControllable()!=parent_property->IsControllable()) property->SetControllable(parent_property->IsControllable()); 
            PropagateFeaturesToSubProperties(property);
        }
    }    
}
/*! \brief Permet de detruire les controllers et de descativer l'edition des sous propriétés */
void SwProperties_Class::UnAllowControlSubProperties(_SwPropertyImpl_Class * parent_property) {
    QMap<QString,_SwPropertyImpl_Class *>::iterator it;
    //Si la propriété a des adaptateurs
    ISwComplexeTypeAdapters * adapters=parent_property->GetComplexeTypeAdapters();
    if (adapters==NULL)
        return; //Non
    //Pour chaque adaptateur, on propage les caracteristiques aux sous propriétés
    QList<ISwSimpleTypeAdapter *> * adapters_list=adapters->GetAdapters();
    for (int i=0;i<adapters_list->count();i++) {
        QString p_name=parent_property->GetRealName()+"."+(*adapters_list)[i]->Label();
        it=_map_properties.find(p_name);
        if (it!=_map_properties.end()) {
            _SwPropertyImpl_Class * property=it.value();
            if (property->IsControlled()==true) property->GetController()->DetachControl();
            property->SetIsEditable(false);
            property->SetControllable(false);
            UnAllowControlSubProperties(property);
        }
    }    

}
/*! \brief Permet d'activer l'edition des sous propriétés */
void SwProperties_Class::AllowControlSubProperties(_SwPropertyImpl_Class * parent_property){
    PropagateFeaturesToSubProperties(parent_property);
}
/*! \brief Sur changement de controle d'une propriété */
void SwProperties_Class::OnPropertyControlChange(ISwProperty * property,ISwController *new_controller,ISwController *old_controller){
    if (old_controller!=NULL) {
        AllowControlSubProperties(dynamic_cast<_SwPropertyImpl_Class *>(property));    
    }
    if (new_controller!=NULL) {
        UnAllowControlSubProperties(dynamic_cast<_SwPropertyImpl_Class *>(property));    
    }
}


/*
*	GEstion des helpers
*/
//---------------------------------------------------------------------------------
void SwProperties_Class::registerHelper( SwProperties_ClassHelper * helper)
{
	_helpers.insert(helper);
	helper->validate();
}

//---------------------------------------------------------------------------------
void SwProperties_Class::unregisterHelper( SwProperties_ClassHelper * helper)
{
	_helpers.remove(helper);
	helper->unValidate();
}

/********************************************************************************************************************/
/*/		SwProperties_ClassHelper
/********************************************************************************************************************/

//---------------------------------------------------------------------------------
SwProperties_ClassHelper::SwProperties_ClassHelper( SwProperties_Class * properties) : _propertiesClass(properties)
{
	if(isValid())
		_propertiesClass->registerHelper(this);
}

//---------------------------------------------------------------------------------
SwProperties_ClassHelper::~SwProperties_ClassHelper()
{
	if(isValid())
		_propertiesClass->unregisterHelper(this);
}

//---------------------------------------------------------------------------------
bool SwProperties_ClassHelper::isValid()
{
	return _propertiesClass == 0 ? false : true;
}

//---------------------------------------------------------------------------------
void SwProperties_ClassHelper::sendOnBeforeChange()
{
	if(isValid())
	{
		_propertiesClass->_OnBeforeChange(_propertiesClass);
	}
}

//---------------------------------------------------------------------------------
void SwProperties_ClassHelper::sendOnAfterChange()
{
	if(isValid())
	{
		_propertiesClass->_OnAfterChange(_propertiesClass);
	}
}

//---------------------------------------------------------------------------------
void SwProperties_ClassHelper::addProperty( QString prefix, QString name, _SwPropertyImpl_Class * property )
{
	if(isValid())
	{
		QMap<QString,_SwPropertyImpl_Class *>::iterator it;
		if (prefix!="") 
			it=_propertiesClass->_map_properties.find(QString("%1.%2").arg(prefix).arg(name));
		else
			it=_propertiesClass->_map_properties.find(QString(name));
		if (it==_propertiesClass->_map_properties.end()) {
			if (prefix!="") 
				_propertiesClass->_map_properties.insert(QString("%1.%2").arg(prefix).arg(name),property);
			else
				_propertiesClass->_map_properties.insert(QString(name),property);
			_propertiesClass->_set_properties.push_back(property);
			_propertiesClass->_OnCreateProperty(_propertiesClass,property);
			_propertiesClass->CreateSubProperties(property);
			property->GetOnControlChangeSignal().iconnect(*_propertiesClass,&SwProperties_Class::OnPropertyControlChange);
			_propertiesClass->PropagateFeaturesToSubProperties(property);
		}
	}
}

//---------------------------------------------------------------------------------
void SwProperties_ClassHelper::unValidate()
{
	_propertiesClass = 0;
}

//---------------------------------------------------------------------------------
void SwProperties_ClassHelper::validate()
{
	//useless
}

//---------------------------------------------------------------------------------
ISwProperties * SwProperties_ClassHelper::getHost()
{
	return _propertiesClass;
}
