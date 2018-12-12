/*!
\file SwController_Class.cpp
\date 30/05/2007
\brief implementation du service controller
\author  Big
\version 1.0
*/

#include "SwController_Class.h"
#include "_SwPropertyImpl_Class.h"
#include "SwAddress_ToolBox.h"
#include "SwApplication.h"
#include "SwMacros.h"

using namespace StreamWork::SwCore;

#define CL_XML_NODE "property"
#define CL_XML_ATT_NAME "name"
#define CL_XML_ATT_HOSTPATH "host"
#define CL_XML_ATT_IDX "idx"

/*! \brief Constructeur */
SwController_Class::SwController_Class(SwComponent_Class * host){
	_host = host;
	_related_property = nullptr;
	_related_properties_service = nullptr;
}
/*! \brief Destructeur */
SwController_Class::~SwController_Class(){
	if (_related_property == nullptr)
		return;
	_related_property->GetOnChangeSignal().idisconnect(*this, &SwController_Class::_OnRelatedPropertyChange);
	_SwPropertyImpl_Class * p = dynamic_cast<_SwPropertyImpl_Class *>(_related_property);
	p->SetController(nullptr);
	_related_property = nullptr;
	_related_properties_service = nullptr;
}
/*! \brief callback de changement de la propriété par le parent*/
void SwController_Class::_OnRelatedPropertyChange(ISwProperty *p) {
	OnParentPropertyChange();
}
/*! \brief Acces a la valeur de la propriété du parent */
QVariant SwController_Class::GetParentPropertyValue(){
	if (_related_property == nullptr) return QVariant();
	return _related_property->GetValue();
}
/*! \brief Definition de la valeur de la propriété du parent */
void SwController_Class::SetParentPropertyValue(QVariant val){
	if (_related_property == nullptr) return;
	_SwPropertyImpl_Class * p = dynamic_cast<_SwPropertyImpl_Class *>(_related_property);
	p->SetValueByController(val);
}
//---------------------------------------------------------------------
// Interface ISwController
//---------------------------------------------------------------------
/*! \brief Permet de crée une propriété */
void SwController_Class::InitializeControl(ISwProperties * properties, QString property_name){
	_related_properties_service = properties;
	_related_property = _related_properties_service->GetProperty(property_name);

	_SwPropertyImpl_Class * p = dynamic_cast<_SwPropertyImpl_Class *>(_related_property);
	p->SetController(this);
	_related_property->GetOnChangeSignal().iconnect(*this, &SwController_Class::_OnRelatedPropertyChange);
	_historic_index = SW_APP->GetHistoricCpt();
	OnParentPropertyAttach();
}
/*! \brief le controle est supprimer au niveau de la propriété */
void SwController_Class::DetachControl() {
	if (_related_property == nullptr)
		return;
	OnParentPropertyDetach();
	_related_property->GetOnChangeSignal().idisconnect(*this, &SwController_Class::_OnRelatedPropertyChange);
	_SwPropertyImpl_Class * p = dynamic_cast<_SwPropertyImpl_Class *>(_related_property);
	p->SetController(nullptr);
	_related_property = nullptr;
	_related_properties_service = nullptr;
	_host->GetParent()->RemoveChild(_host);
};
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void SwController_Class::Liberate(){
}
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * SwController_Class::GetHostComponent(){
	return _host;
}
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief methode permettant de charger des donnees */
void SwController_Class::Load(QDomElement & elt, ISwFinalizerManager & finalizer_manager) {
	bool result;
	QDomElement p_elt = elt.firstChildElement(QString(CL_XML_NODE));
	if (p_elt.isNull())
		return;
	if (p_elt.hasAttribute(CL_XML_ATT_IDX) &&
		p_elt.hasAttribute(CL_XML_ATT_NAME) &&
		p_elt.hasAttribute(CL_XML_ATT_HOSTPATH)) {
		_historic_index_on_load = p_elt.attribute(CL_XML_ATT_IDX).toULongLong(&result);
		_property_name_on_load = p_elt.attribute(CL_XML_ATT_NAME);
		_host_path_on_load = p_elt.attribute(CL_XML_ATT_HOSTPATH);
		finalizer_manager.RegisterFinalization(_historic_index_on_load, this);
	}
}

/*! \brief methode permettant de sauver des donnees */
void StreamWork::SwCore::SwController_Class::Save(QXmlStreamWriter& writer)
{
	writer.writeStartElement(CL_XML_NODE);
	writer.writeAttribute(CL_XML_ATT_NAME, QString(_related_property->GetRealName()));
	QString path;
	try
	{
		//On tente un lien relatif
		path = SwAddress_ToolBox::BuildRelativePath(_host, _related_properties_service->GetHostComponent());
	}
	catch (SwException &)
	{
		//Impossible (pas le meme parent)
		//On recupere un lien universel
		path = SwAddress_ToolBox::BuildUniversalPath(_related_properties_service->GetHostComponent());
	}
	writer.writeAttribute(CL_XML_ATT_HOSTPATH, path);
	writer.writeAttribute(CL_XML_ATT_IDX, QString::number(_historic_index));
	writer.writeEndElement();
}
//---------------------------------------------------------------------
// Interface ISwFinalizer
//---------------------------------------------------------------------
/*! \brief finalize l'operation correspondant a l'index d'historique
\return false si la finalisation n'a pas eu lieu et true si ok*/
bool SwController_Class::Finalize(quint64 historic_index) {
	if (historic_index == _historic_index_on_load) {
		SwComponent_Class * comp = SwAddress_ToolBox::FindTarget(_host_path_on_load, _host);
		if (comp == nullptr)
			return true;
		_related_properties_service = dynamic_cast<ISwProperties *>(comp->QueryService(CG_SW_SERVICE_PROPERTIES));
		InitializeControl(_related_properties_service, _property_name_on_load);
		return true;
	}
	return false;
}