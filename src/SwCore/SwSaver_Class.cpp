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
/*! Sauvegarde */
void SwSaver_Class::Save(SwComponent_Class * root_component,QDomDocument & doc) {
    QDomElement streamwork_elt;
    QDomElement path_node;
    QDomComment comment;
    QMap<QString,bool> paths;
    QMap<QString,bool>::const_iterator pathsIt;

    //Ecriture du header
    streamwork_elt=doc.createElement(CG_SW_XML_DOCUMENT_NODE);
    streamwork_elt.setAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_VERSION,CG_STREAMWORK_VERSION);
    //streamwork_elt.setAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_DATE,__DATE__);
    //streamwork_elt.setAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_TIME,__TIME__);
    doc.appendChild(streamwork_elt);
    //Ajout des path
    paths=SW_APP->ComponentsBank().GetPathList();
    for (pathsIt=paths.begin();pathsIt!=paths.end();pathsIt++) {
        if (pathsIt.value()) {
            path_node=doc.createElement(CG_SW_XML_PATH_NODE);
            path_node.setAttribute(CG_SW_XML_PATH_NODE_ATT_VALUE,pathsIt.key());
            streamwork_elt.appendChild(path_node);
        }
    }
    //Construction du flux
    BuildXMLStream(root_component,doc,streamwork_elt);
}
/*! Sauvegarde groupe */
void SwSaver_Class::SaveGroup(QList<SwComponent_Class *> & components,QDomDocument & doc) {
    QDomElement streamwork_elt;
    QDomElement path_node;
    QDomComment comment;
    QMap<QString,bool> paths;
    QMap<QString,bool>::const_iterator pathsIt;

    //Ecriture du header
    streamwork_elt=doc.createElement(CG_SW_XML_GROUP_DOCUMENT_NODE);
    streamwork_elt.setAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_VERSION,CG_STREAMWORK_VERSION);
    streamwork_elt.setAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_DATE,__DATE__);
    streamwork_elt.setAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_TIME,__TIME__);
    doc.appendChild(streamwork_elt);
    
    //Ecritures des composants
    for(int i=0;i<components.count();i++) {
        BuildXMLStream(components[i],doc,streamwork_elt);    
    }
}

/*! Sauvegarde model */
void SwSaver_Class::SaveModel(QList<SwComponent_Class *> & components,QDomDocument & doc) {
    QDomElement streamwork_elt;
    QDomElement path_node;
    QDomComment comment;
    QMap<QString,bool> paths;
    QMap<QString,bool>::const_iterator pathsIt;

    //Ecriture du header
    streamwork_elt=doc.createElement(CG_SW_XML_DOCUMENT_NODE);
    streamwork_elt.setAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_VERSION,CG_STREAMWORK_VERSION);
    streamwork_elt.setAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_DATE,__DATE__);
    streamwork_elt.setAttribute(CG_SW_XML_DOCUMENT_NODE_ATT_TIME,__TIME__);
    doc.appendChild(streamwork_elt);
    //Ajout des path
    paths=SW_APP->ComponentsBank().GetPathList();
    for (pathsIt=paths.begin();pathsIt!=paths.end();pathsIt++) {
        if (pathsIt.value()) {
            path_node=doc.createElement(CG_SW_XML_PATH_NODE);
            path_node.setAttribute(CG_SW_XML_PATH_NODE_ATT_VALUE,pathsIt.key());
            streamwork_elt.appendChild(path_node);
        }
    }
    //Ecritures du root
    QDomElement component_node;
    //Creation du neoud composant
    component_node=doc.createElement(CG_SW_XML_COMPONENT_NODE);
    //Ajout attribut nom
    component_node.setAttribute(CG_SW_XML_COMPONENT_NODE_ATT_NAME,"NoNamed");
    //Ajout root
    streamwork_elt.appendChild(component_node);
    //Ecritures des composants
    for(int i=0;i<components.count();i++) {
        BuildXMLStream(components[i],doc,component_node);    
    }
}


/*! Construction de la definition du stream au format xml */
void SwSaver_Class::BuildXMLStream(SwComponent_Class * component,QDomDocument & doc,QDomElement & parent_node) {
    QDomElement component_node;
    QDomElement service_node;
    QDomComment comment;
    SwComponent_Class * child;
    QList<QString> services_list;
    QList<QString>::const_iterator service_it;
    ISwService * service;
    ISwPersistent * persistent_interface;

    //Creation du neoud composant
    component_node=doc.createElement(CG_SW_XML_COMPONENT_NODE);
    //Ajout attribut nom
    component_node.setAttribute(CG_SW_XML_COMPONENT_NODE_ATT_NAME,component->GetName());
    //Si description, ajout nom
    if (!component->GetDescription().isEmpty()) {
        component_node.setAttribute(CG_SW_XML_COMPONENT_NODE_ATT_DESC,component->GetDescription());
    }
    //Si desactive , ajout
    if (!component->isActive()) {
        component_node.setAttribute(CG_SW_XML_COMPONENT_NODE_ATT_ACTIVE,"false");
    }
    //Ajout attribut nom d'usine
    if (!component->GetFactoryComponentName().isEmpty()) 
        component_node.setAttribute(CG_SW_XML_COMPONENT_NODE_ATT_FACTORY_NAME,component->GetFactoryComponentName());
    //Ajout attribut nom du plugin usine
    service=component->QueryService(CG_SW_SERVICE_PLUGIN_OVERVIEW);
    if (service!=NULL && dynamic_cast<ISwPluginOverview *>(service)!=NULL) {
        component_node.setAttribute(CG_SW_XML_COMPONENT_NODE_ATT_FACTORY,dynamic_cast<ISwPluginOverview *>(service)->GetPluginName());
    }
    //Insertion du neoud
    parent_node.appendChild(component_node);

    //Services
    services_list=component->GetServicesList();
    //s'il y a des services
    if (services_list.size()) {
        //Pour chaque service
        for(service_it=services_list.begin();service_it!=services_list.end();service_it++) {
            service=component->QueryService(*service_it);  
            persistent_interface=dynamic_cast<ISwPersistent *>(service);
            //S'il a une interface persistante
            if (persistent_interface!=NULL) {
                //Enregistrer ses informations
                service_node=doc.createElement(CG_SW_XML_SERVICE_NODE);
                service_node.setAttribute(CG_SW_XML_SERVICE_ATT_NAME,*service_it);
                component_node.appendChild(service_node);
                //Enregistrement des données du service
                persistent_interface->Save(service_node,doc);
            }
        }
    }
    //Enfants comentaire
    child=component->GetFirstChild();
    //S'il y a au moins un enfant
    if (child!=NULL) {
        //Ecriture des enfants
        while (child!=NULL) {
            BuildXMLStream(child,doc,component_node);
            child=component->GetNextChild();
        }
    }
}

