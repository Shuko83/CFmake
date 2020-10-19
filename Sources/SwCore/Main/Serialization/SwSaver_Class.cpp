/*!
 \file SwSaver_Class.cpp
 \brief Implementation of the Class SwSaver_Class
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QMap>

#include "SwCoreConstantes.h"
#include "SwApplication.h"
#include "SwMacros.h"
#include "SwSaver_Class.h"
#include "ISwPersistent.h"
#include "ISwPluginOverview.h"

using namespace StreamWork::SwCore;


/*! Constructeur */
SwSaver_Class::SwSaver_Class() {

}
/*! Destructeur */
SwSaver_Class::~SwSaver_Class(){

}

void SwSaver_Class::Save(SwComponent_Class * root_component, QXmlStreamWriter & writer)
{
	//Ecriture du header	
	writer.writeStartElement(CG_SW_XML_STREAM_NODE);
		
	//Ajout des path
	QMap<QString, bool> paths = SW_APP->ComponentsBank().GetPathList();
	for (auto pathsIt = paths.begin(); pathsIt != paths.end(); ++pathsIt)
	{
		if (pathsIt.value())
		{
			writer.writeStartElement(CG_SW_XML_COMPONENT_NODE);
			writer.writeAttribute(CG_SW_XML_COMPONENT_NODE_ATT_NAME, pathsIt.key());
			writer.writeEndElement();
		}
	}
	//Construction du flux
	BuildXMLStream(root_component, writer);

	writer.writeEndElement();
}
/*! Sauvegarde groupe */
void SwSaver_Class::SaveGroup(QList<SwComponent_Class *> & components, QXmlStreamWriter & writer)
{
	//Ecriture du header
	writer.writeStartElement(CG_SW_XML_GROUP_DOCUMENT_NODE);
	writer.writeAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_VERSION, CG_STREAMWORK_VERSION);
	writer.writeAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_DATE, __DATE__);
	writer.writeAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_TIME, __TIME__);
	
	//Ecritures des composants
	for (int i = 0; i < components.count(); i++)
	{
		BuildXMLStream(components[i], writer);
	}

	writer.writeEndElement();
}

/*! Sauvegarde model */
void SwSaver_Class::SaveModel(QList<SwComponent_Class *>& components, QXmlStreamWriter& writer)
{	
	//Ecriture du header
	writer.writeStartElement(CG_SW_XML_DOCUMENT_NODE);
	writer.writeAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_VERSION, CG_STREAMWORK_VERSION);
	writer.writeAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_DATE, __DATE__);
	writer.writeAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_TIME, __TIME__);

	//Ajout des path
	QMap<QString, bool> paths = SW_APP->ComponentsBank().GetPathList();
	for (auto pathsIt = paths.cbegin(); pathsIt != paths.cend(); pathsIt++)
	{
		if (pathsIt.value())
		{
			writer.writeStartElement(CG_SW_XML_PATH_NODE);
			writer.writeAttribute(CG_SW_XML_PATH_NODE_ATT_VALUE, pathsIt.key());
			writer.writeEndElement();
		}
	}
	//Ecritures du root
	//Creation du noeud composant
	writer.writeStartElement(CG_SW_XML_COMPONENT_NODE);
	//Ajout attribut nom
	writer.writeAttribute(CG_SW_XML_COMPONENT_NODE_ATT_NAME, "NoNamed");
	//Ecritures des composants
	for (int i = 0; i < components.count(); i++)
	{
		BuildXMLStream(components[i], writer);
	}
	//Ajout root
	writer.writeEndElement();

	writer.writeEndElement();
}

/*! Construction de la definition du stream au format xml */
void SwSaver_Class::BuildXMLStream(SwComponent_Class * component, QXmlStreamWriter& writer)
{
	writer.writeStartElement(CG_SW_XML_COMPONENT_NODE);
	writer.writeAttribute(CG_SW_XML_COMPONENT_NODE_ATT_NAME, component->GetName());
	if (!component->GetDescription().isEmpty())
	{
		writer.writeAttribute(CG_SW_XML_COMPONENT_NODE_ATT_DESC, component->GetDescription());
	}
	if (!component->isActive())
	{
		writer.writeAttribute(CG_SW_XML_COMPONENT_NODE_ATT_ACTIVE, "false");
	}
	//Ajout attribut nom d'usine
	if (!component->GetFactoryComponentName().isEmpty() && !component->GetFactoryName().isEmpty())
	{
		writer.writeAttribute(CG_SW_XML_COMPONENT_NODE_ATT_FACTORY_NAME, component->GetFactoryComponentName());
		writer.writeAttribute(CG_SW_XML_COMPONENT_NODE_ATT_PLUGIN_NAME, component->GetFactoryName());
	}

	//  Auparavant, on enregistrait ici le nom de DLL.

	QList<QString> services_list = component->GetServicesList();
	std::sort(services_list.begin(), services_list.end());
	for (auto service_it = services_list.begin(); service_it != services_list.end(); service_it++)
	{
		ISwService * service = component->QueryService(*service_it);
		ISwPersistent* persistent_interface = dynamic_cast<ISwPersistent *>(service);
		//S'il a une interface persistante
		if (persistent_interface != NULL)
		{
			writer.writeStartElement(CG_SW_XML_SERVICE_NODE);
			writer.writeAttribute(CG_SW_XML_SERVICE_ATT_NAME, *service_it);
			persistent_interface->Save(writer);
			writer.writeEndElement();
		}
	}

	//Enfants comentaire
	SwComponent_Class * child = component->GetFirstChild();
	//Ecriture des enfants
	while (child != NULL)
	{
		BuildXMLStream(child, writer);
		child = component->GetNextChild();
	}

	writer.writeEndElement();
}

