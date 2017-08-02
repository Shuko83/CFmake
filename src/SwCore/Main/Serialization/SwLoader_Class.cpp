/*!
 \file SwLoader_Class.cpp
 \brief Implementation of the Class SwLoader_Class
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include "SwLoader_Class.h"
#include "SwCoreConstantes.h"
#include "SwApplication.h"
#include "SwMacros.h"
#include "SwSaver_Class.h"
#include "ISwPersistent.h"
#include "QDebug.h"
#include "ISwServiceOwner.h"

using namespace StreamWork::SwCore;


/*! Constructeur */
SwLoader_Class::SwLoader_Class() {
    _finalizations.clear();
	_finalizations2.clear();
}
/*! Destructeur */
SwLoader_Class::~SwLoader_Class(){
    _finalizations.clear();
	_finalizations2.clear();
}
/*! Chargement d'un stream
\param[in] le document xml decrivant le flux
\return le neoud racine*/
SwComponent_ClassPtr SwLoader_Class::Load(QDomDocument & doc) throw(SwException) {
    SwComponent_ClassPtr root_component;  
    QDomElement path_elt;
        
    //Check du header
    QDomElement root_elt=doc.documentElement();
    if (root_elt.tagName()!=QString(CG_SW_XML_DOCUMENT_NODE)) {
        QString msg=QString("Invalid XML stream format");
        LAUNCH_SWEXCEPTION("SwCore",msg)
    }
    //Ajout des paths
    for(QDomElement path_node = root_elt.firstChildElement(QString(CG_SW_XML_PATH_NODE)); !path_node.isNull(); path_node = path_node.nextSiblingElement(QString(CG_SW_XML_PATH_NODE)))
    {
        if (path_node.hasAttribute(CG_SW_XML_PATH_NODE_ATT_VALUE)) {
            SW_APP->ComponentsBank().AddPath(path_node.attribute(CG_SW_XML_PATH_NODE_ATT_VALUE));
        }
    }        
    //Construction du stream
    QDomElement elt=root_elt.firstChildElement(QString(CG_SW_XML_COMPONENT_NODE));
    root_component=BuildStream(elt,NULL);
    //Finalization
    FinalizeUnfinalized();
    //Renvoie la racine du stream crée
    return root_component;
}
/*! Chargement d'un groupe
\param[in] le neoud racine
\param[in] le document xml decrivant le flux*/
void SwLoader_Class::LoadGroup(SwComponent_Class * parent_component,QDomDocument & doc) {
    SwComponent_ClassPtr tmp_parent_component;  
    SwComponent_ClassPtr component;  
    QDomElement path_elt;
    tmp_parent_component=SW_APP->ComponentsBank().CreateComponent("");    

    //Check du header
    QDomElement root_elt=doc.documentElement();
    if (root_elt.tagName()!=QString(CG_SW_XML_GROUP_DOCUMENT_NODE)) {
        QString msg=QString("Invalid XML stream group format");
        LAUNCH_SWEXCEPTION("SwCore",msg)
    }

    //Chargement des enfants
    for(QDomElement child_node = root_elt.firstChildElement(QString(CG_SW_XML_COMPONENT_NODE)); !child_node.isNull(); child_node = child_node.nextSiblingElement(QString(CG_SW_XML_COMPONENT_NODE)))
    {
        BuildStream(child_node,tmp_parent_component,true);   
    }
    //Finalization
    FinalizeUnfinalized();
    //Deplacement des enfants
    component=tmp_parent_component->GetFirstChild();
    while (component!=NULL) {
        tmp_parent_component->RemoveChild(component);
        if (parent_component->GetChild(component->GetName())!=NULL) {
            component->SetName(parent_component->GetSuggestedNameForChild(component->GetName()));
        }
        parent_component->AddChild(component);
        component=tmp_parent_component->GetFirstChild();
    }
    //Finalization suite
    FinalizeUnfinalized();
}
/*! construction du stream
\param[in] neoud courant
\param[in] composant parent*/
SwComponent_ClassPtr SwLoader_Class::BuildStream(QDomElement & node,SwComponent_Class * parent_component,bool create_always) throw(SwException){
    SwComponent_ClassPtr component;
    SwComponent_ClassPtr child_component;
    QString factory_name;
    QString name;
    QString description;
    QDomElement service_node;
    QDomElement child_node;
    QString service_name;
    ISwService * service;
    ISwPersistent * persistent_interface;
    bool use_suggested_name;


    //Si le neoud est vide alors on renvoie null
    if (node.isNull()) {
        return SwComponent_ClassPtr(0);
    }
    //Recuperation du nom
    if (!node.hasAttribute(CG_SW_XML_COMPONENT_NODE_ATT_NAME)) {
        SW_APP->Logger().Log(LogLvl_Critical,"component node at %d,%d hasn't name",node.lineNumber(),node.columnNumber());
        return SwComponent_ClassPtr(0);
    }
    name=node.attribute(CG_SW_XML_COMPONENT_NODE_ATT_NAME);
    component=NULL;
    //On regarde si le parent existe et s'il contient deja le composant ou que le flag toujours cree existe
    if (parent_component!=NULL) 
        component=parent_component->GetChild(name);
    else 
        component=NULL;
    if (!(parent_component!=NULL && component!=NULL) || create_always) {
        //Check si on doit utiliser un nom suggere
        if (parent_component!=NULL && component!=NULL && create_always) {
            use_suggested_name=true;    
        } else {
            use_suggested_name=false;    
        }
        //Non alors on le crée
        //Recuperation nom d'usine
        factory_name=node.attribute(CG_SW_XML_COMPONENT_NODE_ATT_FACTORY_NAME);
        //check du nom d'usine
        if (factory_name.isEmpty()) {
            //Creation du composant par defaut
            component=SW_APP->ComponentsBank().CreateComponent("");
        } else {
            //Creation du composant
            component=SW_APP->ComponentsBank().CreateComponent(factory_name);
        }
        if (component==NULL) {
            SW_APP->Logger().Log(LogLvl_Critical,"At %d,%d , failed to create component of type %s",node.lineNumber(),node.columnNumber(),factory_name.toLatin1().data());
            return SwComponent_ClassPtr(0);       
        }
        if (use_suggested_name) {
            //Nommage du composant avec un nom suggere
            component->SetName(parent_component->GetSuggestedNameForChild(name));
        } else {
            //Nommage du composant
            component->SetName(name);
        }
        //Recuperation de la description
        description=node.attribute(CG_SW_XML_COMPONENT_NODE_ATT_DESC);
        //Si description valide
        if (!description.isEmpty()) {
            component->SetDescription(description);
        }
        //Recupration de l'activation
        QString active=node.attribute(CG_SW_XML_COMPONENT_NODE_ATT_ACTIVE);
        if (!active.isEmpty() && active=="false") {
            component->setActive(false);
        }

        //Ajout de l'enjant au parent
        if (parent_component!=NULL) {
            parent_component->AddChild(component);
        }
    }
    //Chargement des services du composants
    for(QDomElement service_node = node.firstChildElement(QString(CG_SW_XML_SERVICE_NODE)); !service_node.isNull(); service_node = service_node.nextSiblingElement(QString(CG_SW_XML_SERVICE_NODE)))
		{
        if (service_node.hasAttribute(CG_SW_XML_SERVICE_ATT_NAME)) {
			service_name=service_node.attribute(CG_SW_XML_SERVICE_ATT_NAME);
			service=component->QueryService(service_name);
            if (service!=NULL) {
				persistent_interface=dynamic_cast<ISwPersistent *>(service);
                if (persistent_interface!=NULL) {
					persistent_interface->Load(service_node,*((ISwFinalizerManager *)this));   
                } else {
					SW_APP->Logger().Log(LogLvl_Debug,"At %d,%d unable to load service %s, because he hasn't persistent interface",service_node.lineNumber(),service_node.columnNumber(),service_name.toLatin1().data());
				}
            } else {
				SW_APP->Logger().Log(LogLvl_Debug,"At %d,%d unable to load service %s, because he's indefined",service_node.lineNumber(),service_node.columnNumber(),service_name.toLatin1().data());
			}
        } else {
			SW_APP->Logger().Log(LogLvl_Debug,"At %d,%d found service with no name",service_node.lineNumber(),service_node.columnNumber());
		}
	}
    //Chargement des enfants
    for(QDomElement child_node = node.firstChildElement(QString(CG_SW_XML_COMPONENT_NODE)); !child_node.isNull(); child_node = child_node.nextSiblingElement(QString(CG_SW_XML_COMPONENT_NODE)))
    {
        child_component=BuildStream(child_node,component);
    }
    //Fin
    return component;
}
/*! resolution des finalizations manquantes
\note est appelé une fois a la fin du load
\note a appeler a nouveau si le neoud racine est inserer dans un contexte*/
void SwLoader_Class::FinalizeUnfinalized() {
    QMap<quint64,ISwFinalizer *>::iterator it;
    QMap<quint64,ISwFinalizer *>::iterator itd;

    it=_finalizations.begin();
    if (it==_finalizations.end())
        return;
    itd=it;
    do {
        it++;
        //Tente la finalization
        if (itd.value()->Finalize(itd.key())) {
            //Si ok, on supprime la finalisation
            _finalizations.erase(itd);
        }
        //On passe a la finalization suivante
        itd=it;  
    } while (it!=_finalizations.end());


	//finalization des ISwOwnerService :
	QMap<quint64,ISwFinalizer *>::const_iterator cit = _finalizations2.constBegin();
	while( cit != _finalizations2.constEnd())
	{
		cit.value()->Finalize(cit.key());
		++cit;
	}

	_finalizations2.clear();

}
/*! \brief enregistre une finalisation pour un index d'historique donnée
\param[in] historic_index index d'historique de finalisation
\param[in] finaliser a appelé
\exception SwException Historic index already used*/
void SwLoader_Class::RegisterFinalization(quint64 historic_index,ISwFinalizer * finalizer) throw(SwException) {

	ISwServiceOwner * owner = dynamic_cast<ISwServiceOwner *>(finalizer);
    QMap<quint64,ISwFinalizer *>::const_iterator it;

	if( !owner )
	{
    it=_finalizations.find(historic_index);
    if (it!=_finalizations.end()) {
        QString msg=QString("In finalization registration %1 index already registered").arg(historic_index);
			qDebug() << "/!\\ Warning SwCore " << msg;
    }
		_finalizations.insertMulti(historic_index,finalizer);
	}
	else
	{
		it=_finalizations2.find(historic_index);
		if (it!=_finalizations2.end()) {
			QString msg=QString("In finalization2 registration %1 index already registered").arg(historic_index);
			qDebug() << "/!\\ Warning SwCore " << msg;
		}
		_finalizations2.insertMulti(historic_index,finalizer);
	}
}
