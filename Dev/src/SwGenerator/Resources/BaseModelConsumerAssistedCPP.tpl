/*!
 \file {{COMPONENT_NAME}}.cpp
 \brief {{DESCRIPTION}}
 \version 1.0
 \author {{AUTHOR}}
*/

/**
  * Includes Globaux
  */
#include <QObject>
#include <SwApplication.h>
#include <SwMacros.h>
#include <QString>
#include <QMessageBox>

/**
  * Includes Locaux
  */
#include "{{COMPONENT_NAME}}.h"
{{#SECTION_HAS_PIN}}
#include <tdlcommon\data\SwParts\_SwData_InterComponentMessageData.h>
{{/SECTION_HAS_PIN}}

using namespace {{NAMESPACE_1}}::{{NAMESPACE_2}}::{{NAMESPACE_3}};

//-------------------------------------------------------------------------
{{COMPONENT_NAME}}::{{COMPONENT_NAME}}(): BaseModelConsumerAssisted()
{
	{{#SECTION_ISWEXECUTABLE_SERVICE}}setExecutableServiceAvaibility(true);{{/SECTION_ISWEXECUTABLE_SERVICE}}{{#SECTION_HAS_PIN}}
	setPinServiceAvaibility(true);{{/SECTION_HAS_PIN}}{{#SECTION_ISWPERSISTENTCONFIGURABLE}}
	setOwnerConfigurableServiceAvaibility(true);{{/SECTION_ISWPERSISTENTCONFIGURABLE}}{{#SECTION_ISWPERSISTENT}}
	setOwnerServiceAvaibility(true);{{/SECTION_ISWPERSISTENT}}
	
	{{#SECTION_HAS_PIN}}{{#SECTION_ISWPIN_OUTPUT}}_pinOut = NULL;
	{{/SECTION_ISWPIN_OUTPUT}}{{#SECTION_ISWPIN_LISTENER}}_pinIn = NULL;{{/SECTION_ISWPIN_LISTENER}}
	
	{{/SECTION_HAS_PIN}}{{#SECTION_INTERFACECONSUMED}}{{INTERFACE_TYPE}}*	{{CONSUMED_VAR_NAME}} = NULL;
	{{/SECTION_INTERFACECONSUMED}}{{#SECTION_INTERFACEPRODUCTED}}{{INTERFACE_TYPE}}*	{{PRODUCTED_VAR_NAME}} = NULL;
	{{/SECTION_INTERFACEPRODUCTED}}{{#SECTION_HAS_PROPERTIES}}
	//---------------------------------------------------
	//				Initialisation des proprietes
	{{#FOR_COMPONENT_PROPERTIES}}{{INIT}}
	{{/FOR_COMPONENT_PROPERTIES}}{{/SECTION_HAS_PROPERTIES}}{{CONSTRUCTOR}}
}

//-------------------------------------------------------------------------
{{COMPONENT_NAME}}::~{{COMPONENT_NAME}}()
{
	{{#SECTION_HAS_PIN}}//---------------------------------------------------
	//				Gestion des pins
	//---------------------------------------------------
	
	{{#SECTION_ISWPIN_OUTPUT}}unregisterPin(_pinOut);{{/SECTION_ISWPIN_OUTPUT}}
	{{#SECTION_ISWPIN_LISTENER}}unregisterPin(_pinIn);{{/SECTION_ISWPIN_LISTENER}}
	
	{{/SECTION_HAS_PIN}}{{#SECTION_HAS_INTERFACES}}//---------------------------------------------------
	//				Gestion des Interfaces
	//---------------------------------------------------
	
	{{#SECTION_INTERFACECONSUMED}}unconsummeInterface("{{INTERFACE_NAME}}");
	{{/SECTION_INTERFACECONSUMED}}
	{{#SECTION_INTERFACEPRODUCTED}}unprovideInterface("{{INTERFACE_NAME}}");
	{{/SECTION_INTERFACEPRODUCTED}}
	{{/SECTION_HAS_INTERFACES}}{{#SECTION_ISWSHORTCUT}}//---------------------------------------------------
	//				Gestion des raccourcis
	//---------------------------------------------------
	
	{{#SECTION_ADD_SHORTCUT}}removeShortcut("{{SHORTCUT_NAME}}");
	{{/SECTION_ADD_SHORTCUT}}{{/SECTION_ISWSHORTCUT}}{{DESTRUCTOR}}
}

//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::initializeBaseModelConsumer() throw(SwException)
{
	{{#SECTION_HAS_PIN}}//---------------------------------------------------
	//				Gestion des pins
	//---------------------------------------------------
	{{#SECTION_ISWPIN_OUTPUT}}// La pin d'emission n'est pas listener de l'interface
	_pinOut=registerPin(QString("Out"),"SwData_InterComponentMessageData",false);{{/SECTION_ISWPIN_OUTPUT}}
	
	{{#SECTION_ISWPIN_LISTENER}}// La pin de reception est listener de l'interface
	_pinIn=registerPin(QString("In"),"SwData_InterComponentMessageData",true);{{/SECTION_ISWPIN_LISTENER}}
	
	{{/SECTION_HAS_PIN}}{{#SECTION_ISWSHORTCUT}}//---------------------------------------------------
	//				Gestion des raccourcis
	//---------------------------------------------------
	
	//Definir le nom du composant (version graphique)
	setComponentNameForShortcut("{{SHORTCUT_COMPONENT_NAME}}");
	
	{{#SECTION_ADD_SHORTCUT}}addShortcut("{{SHORTCUT_NAME}}");
	{{/SECTION_ADD_SHORTCUT}}
	{{/SECTION_ISWSHORTCUT}}{{#SECTION_HAS_INTERFACES}}//---------------------------------------------------
	//				Gestion des Interfaces
	//---------------------------------------------------

	{{#SECTION_INTERFACECONSUMED}}consummeInterface<{{INTERFACE_TYPE}}>("{{INTERFACE_NAME}}");
	{{/SECTION_INTERFACECONSUMED}}
	{{#SECTION_INTERFACEPRODUCTED}}provideInterface("{{INTERFACE_NAME}}",{{PRODUCTED_VAR_NAME}});{{/SECTION_INTERFACEPRODUCTED}}
	{{/SECTION_HAS_INTERFACES}}
}
{{#SECTION_ISCONSUMED}}
//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::interfaceIsUnavailable( QString interface_name )
{
	{{#SECTION_INTERFACECONSUMED}}if(interface_name == "{{INTERFACE_NAME}}")
	{
		{{CONSUMED_VAR_NAME}} = NULL;
		QMessageBox::information(NULL,"Interface is unavailable","The interface {{INTERFACE_NAME}} is unavailable.");
	}
	{{#SECTION_INTERFACECONSUMED_separator}}else {{/SECTION_INTERFACECONSUMED_separator}}{{/SECTION_INTERFACECONSUMED}}
}

//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::interfaceIsAvailable( QString interface_name )
{
	{{#SECTION_INTERFACECONSUMED}}if(interface_name == "{{INTERFACE_NAME}}")
	{
		{{CONSUMED_VAR_NAME}} = getInterface<{{INTERFACE_TYPE}}>("{{INTERFACE_NAME}}");
		QMessageBox::information(NULL,"Interface is available","The interface {{INTERFACE_NAME}} is available.");
	}
	{{#SECTION_INTERFACECONSUMED_separator}}else {{/SECTION_INTERFACECONSUMED_separator}}{{/SECTION_INTERFACECONSUMED}}
}
{{/SECTION_ISCONSUMED}}{{#SECTION_ISWEXECUTABLE_SERVICE}}
//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::Initialize( double start_time,ISwExecution_Service * executor ) throw (SwException)
{

}

//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::Start( double current_time ) throw (SwException)
{

}

//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::Execute( double current_time,bool is_first_call ) throw (SwException)
{
	
}

//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::Stop( double current_time )
{
	
}
{{/SECTION_ISWEXECUTABLE_SERVICE}}{{#SECTION_ISWPIN_LISTENER}}
//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::eventReceiveData( SwPin * src,SwData_Class * data )
{
	if ( src == _pinIn ) 
	{
		//Check si le type est valide
		if ( data->GetType() == "SwData_InterComponentMessageData" ) 
		{
			//Traiter la donnee
			SwRefPtr<tdlcommon::data::_SwData_InterComponentMessageData>  baseMessageData = dynamic_pointer_cast<tdlcommon::data::_SwData_InterComponentMessageData> (data);

			if (baseMessageData != NULL) 
			{
				PlgArray<unsigned char> receivedData;
				if (baseMessageData->message != NULL)
				{
                    baseMessageData->message->encode(&receivedData);							

					//Traiter le message
				}
			}
		}
	}
}
{{/SECTION_ISWPIN_LISTENER}}{{#SECTION_ISWPIN_OUTPUT}}
//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::sendMessage( tdlcommon::interfaces::IBaseMessage * baseMessage, double time )
{
	if (_pinOut != NULL)
	{
		SwRefPtr<tdlcommon::data::_SwData_InterComponentMessageData> swData = dynamic_pointer_cast<tdlcommon::data::_SwData_InterComponentMessageData>(SW_CREATE_DATA(CG_SW_DATA_TDLCOMMON_DATA_INTERCOMPONENTMESSAGEDATA_ID));
		swData->message = baseMessage;
        swData->Time = time;
        _pinOut->SendData(swData); 
    }        
}
{{/SECTION_ISWPIN_OUTPUT}}{{#SECTION_ISWSHORTCUT}}
//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::processCommand(QString name)
{
	{{#SECTION_ADD_SHORTCUT}}if(name == "{{SHORTCUT_NAME}}")
	{
		QMessageBox::information(NULL,"{{SHORTCUT_NAME}}","{{SHORTCUT_NAME}}");
	}
	{{#SECTION_ADD_SHORTCUT_separator}}else {{/SECTION_ADD_SHORTCUT_separator}}{{/SECTION_ADD_SHORTCUT}}
}
{{/SECTION_ISWSHORTCUT}}{{#SECTION_ISWPERSISTENTCONFIGURABLE}}
//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::LoadConfiguration( QDomElement &elm )
{
	//Example : 
	QDomElement elt_path = elm.firstChildElement("NewElement");
	if ( elt_path.hasAttribute("Attribute") )
	{
		QString resultString = elt_path.attribute("Attribute");
	}
}

//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::SaveConfiguration( QDomElement &elm,QDomDocument &doc )
{
	//Example : 
	QDomElement elt_ent;
	elt_ent=doc.createElement("NewElement");
	elt_ent.setAttribute("Attribute","Value");
	elm.appendChild(elt_ent);
}
{{/SECTION_ISWPERSISTENTCONFIGURABLE}}{{#SECTION_ISWPERSISTENT}}
//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::Load( QDomElement & elt,ISwFinalizerManager & finalizer_manager )
{
	//Example : 
	QDomElement elt_path = elt.firstChildElement("NewElement");
	if (elt_path.hasAttribute("Attribute") )
	{
		QString resultString = elt_path.attribute("Attribute");
	}

	bool result;
    qint64 _historicIndex=elt.attribute("_idhost").toULongLong(&result);
    if (result == false)
	{       
        _historicIndex=SW_APP->GetHistoricCpt();
    }
    
	//Todo if necessary
	//finalizer_manager.RegisterFinalization(_historicIndex,this);
}

//-------------------------------------------------------------------------
void {{COMPONENT_NAME}}::Save( QDomElement & elt,QDomDocument &doc )
{
	//Example : 
	QDomElement elt_ent;
	elt_ent=doc.createElement("NewElement");
	elt_ent.setAttribute("Attribute","Value");
	elt.appendChild(elt_ent);

	qint64 _historicIndex=SW_APP->GetHistoricCpt();
	elt.setAttribute("_idhost",_historicIndex);
}
{{/SECTION_ISWPERSISTENT}}