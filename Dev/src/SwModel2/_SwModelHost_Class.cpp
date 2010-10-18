/*!
 \file _SwModelHost_Class.cpp
 \brief Implementation d'un hote de model
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QMessageBox>
#include <SwApplication.h>
#include <SwMacros.h>
#include <SwMacros.h>
#include <QUuid>
#include <QRegExp>
#include <SwAddress_ToolBox.h>
#include <QMessageBox>
#include "_SwModelHost_Class.h"
#include "_SwModel_Class.h"
#include "_SwModelHostExportSelector.h"
#include "_SwModelExportedExecutable.h"

#define CL_XML_NODE_PROPERTY "property"
#define CL_XML_NODE_INTERFACE_C "iconsume"
#define CL_XML_NODE_INTERFACE_P "iprovide"
#define CL_XML_NODE_PIN "pin"
#define CL_XML_NODE_EXE "executable"    
#define CL_XML_NODE_ACTIVABLE "activable"
#define CL_XML_NODE_OWNERCONFIG "ownerConfig"
#define CL_XML_NODE_ATT_NAME "name"
#define CL_XML_NODE_ATT_EXP_NAME "ename"
#define CL_XML_NODE_ATT_PATH "path"
#define CL_XML_NODE_ATT_ITYPE "itype"
#define CL_XML_NODE_ATT_IDX "_idhost"


using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwModelHost_Class::_SwModelHost_Class(): SwComponent_Class(){
    _model=NULL;
}
/*! \brief Destructeur */
_SwModelHost_Class::~_SwModelHost_Class(){

    if (_model==this)
        DestroyBinding();

    //Desenregistrement des services
    this->UnregisterService(_pins_service->GetServiceName());
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    this->UnregisterService(GetServiceName());
    //Destruction des services
    delete _pins_service;
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;

    //Nettoyage des entites
/*    for(int i=0;i<_exported_entities.count();i++) {
        _SwModelExportedExecutable * executable=dynamic_cast<_SwModelExportedExecutable *>(_exported_entities[i]);
        if (executable!=0) {
            executable->Unbind();
        }
        delete _exported_entities[i];   
    }
    _exported_entities.clear(); */
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwModelHost_Class::InitializeResources() throw(SwException) {

    //Creation des services
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    _pins_service=new SwPins_Manager_Class(this) ;
    //Enregistrement des services
    this->RegisterService(this);
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);
    this->RegisterService(_pins_service);

    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of _SwModelHost_Class done\n"));
        
    BuildEntities();

}
//----------------------------------------------------
// Interface ISwAdminSetup
//----------------------------------------------------
/*! \brief Permet d'acceder aux preferences d'administration*/
void _SwModelHost_Class::AdminSetup() {
    SwComponent_Class *root;
    bool adminDone=false;    

    root=this;
    while (root->GetParent()!=NULL) root=root->GetParent();

    _SwModelHostExportSelector * selector=new _SwModelHostExportSelector(0,root,&_exported_entities);
    if (selector->exec()==QDialog::Accepted) {
        DestroyEntities();
        selector->ValidChange();
        adminDone=true;
    }
    delete selector;
    if (adminDone) {
        BuildEntities();
    }


}
//----------------------------------------------------
// Gestion liaison entre model et model_host
//----------------------------------------------------
/*! \brief Mise en place du pont entre le model et le model host*/
void _SwModelHost_Class::CreateBinding(SwComponent_Class * model) {
    if(_model!=NULL) {
        DestroyBinding();
    }
    _model=model;
    if (_model==NULL)
        return;
    for(int i=0;i<_exported_entities.count();i++) {
        _exported_entities[i]->Bind(_model);    
    }
    h_index=SW_APP->GetHistoricCpt();
}
/*! \brief Suppression du pont entre le model et le model host*/
void _SwModelHost_Class::DestroyBinding(){
    if (_model==NULL)
        return;
    for(int i=0;i<_exported_entities.count();i++) {
        _exported_entities[i]->Unbind();    
    }
    _model=NULL;
}
/*! \brief Creation reelle des entités*/
void _SwModelHost_Class::BuildEntities() {
    //Si un modele est connecté on le deconnecte
    if(_model!=NULL) {
        DestroyBinding();
    }
    for(int i=0;i<_exported_entities.count();i++) {
        _exported_entities[i]->Build(this);    
    }
}
/*! \brief Suppression reelle des entites*/
void _SwModelHost_Class::DestroyEntities() {
    if(_model!=NULL) {
        DestroyBinding();
    }
    for(int i=0;i<_exported_entities.count();i++) {
        _exported_entities[i]->Destroy();    
    }
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwModelHost_Class::Liberate() {

}
//---------------------------------------------------------------------
// Interface ISwPersistence
//---------------------------------------------------------------------
/*! \brief methode permettant de charger des donnees
\param[in] elt neoud parent
\param[in] finalizer_manager manager de finalisation
*/
void _SwModelHost_Class::Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager) {
    QDomElement elt_ent;
    _SwModelExportedEntity *entity;
    DestroyEntities();
    for(elt_ent = elt.firstChildElement(); !elt_ent.isNull(); elt_ent = elt_ent.nextSiblingElement())
    {
        entity=NULL;
        if (elt_ent.nodeName()==QString(CL_XML_NODE_PROPERTY)) {
            entity=_SwModelExportedEntity::NewEntity(Ent_Property);
        }
        if (elt_ent.nodeName()==QString(CL_XML_NODE_INTERFACE_C)) {
            entity=_SwModelExportedEntity::NewEntity(Ent_InterfaceC);
        }
        if (elt_ent.nodeName()==QString(CL_XML_NODE_INTERFACE_P)) {
            entity=_SwModelExportedEntity::NewEntity(Ent_InterfaceP);
        }
        if (elt_ent.nodeName()==QString(CL_XML_NODE_PIN)) {
            entity=_SwModelExportedEntity::NewEntity(Ent_Pin);
        }
        if (elt_ent.nodeName()==QString(CL_XML_NODE_EXE)) {
            entity=_SwModelExportedEntity::NewEntity(Ent_Executable);
        }
        if (elt_ent.nodeName()==QString(CL_XML_NODE_OWNERCONFIG)) {
            entity=_SwModelExportedEntity::NewEntity(Ent_OwnerConfigurable);
        }      
        if (elt_ent.nodeName()==QString(CL_XML_NODE_ACTIVABLE)) {
            entity=_SwModelExportedEntity::NewEntity(Ent_Activable);
        } 
        if (entity!=NULL &&
            elt_ent.hasAttribute(CL_XML_NODE_ATT_NAME) &&
            elt_ent.hasAttribute(CL_XML_NODE_ATT_EXP_NAME) &&
            (elt_ent.hasAttribute(CL_XML_NODE_ATT_PATH) ||
            elt_ent.hasAttribute(CL_XML_NODE_ATT_ITYPE))) 
        {
            entity->_name=elt_ent.attribute(CL_XML_NODE_ATT_NAME);    
            entity->_exported_name=elt_ent.attribute(CL_XML_NODE_ATT_EXP_NAME);    
            entity->_host_path=elt_ent.attribute(CL_XML_NODE_ATT_PATH);   
            entity->_itype=elt_ent.attribute(CL_XML_NODE_ATT_ITYPE);   
            _exported_entities.push_back(entity);
        } else {
            delete entity;
        }
    }
    bool result;
    BuildEntities();
}
/*! \brief methode permettant de sauver des donnees
\param[in] elt neoud parent
\param[in] doc document parent
*/
void _SwModelHost_Class::Save(QDomElement & elt,QDomDocument &doc) {
    QDomElement elt_ent;
    
    for(int i=0;i<_exported_entities.count();i++) {
        //Creation du neoud
        switch (_exported_entities[i]->_type) {
            case Ent_Property:
                elt_ent=doc.createElement(CL_XML_NODE_PROPERTY);
                break;
            case Ent_OwnerConfigurable:
                elt_ent=doc.createElement(CL_XML_NODE_OWNERCONFIG);
                break; 
            case Ent_InterfaceC:
                elt_ent=doc.createElement(CL_XML_NODE_INTERFACE_C);
                break;
            case Ent_InterfaceP:
                elt_ent=doc.createElement(CL_XML_NODE_INTERFACE_P);
                break;
            case Ent_Pin:
                elt_ent=doc.createElement(CL_XML_NODE_PIN);
                break;
            case Ent_Executable:
                elt_ent=doc.createElement(CL_XML_NODE_EXE);
                break;
            case Ent_Activable:
                elt_ent=doc.createElement(CL_XML_NODE_ACTIVABLE);
                break;                
            default:
                return;
                break;
        }
        //Affectation des attributs
        elt_ent.setAttribute(CL_XML_NODE_ATT_NAME,_exported_entities[i]->_name);
        elt_ent.setAttribute(CL_XML_NODE_ATT_EXP_NAME,_exported_entities[i]->_exported_name);
        elt_ent.setAttribute(CL_XML_NODE_ATT_PATH,_exported_entities[i]->_host_path);
        elt_ent.setAttribute(CL_XML_NODE_ATT_ITYPE,_exported_entities[i]->_itype);
        //Attachement du neoud au parent
        elt.appendChild(elt_ent);
        elt.setAttribute(CL_XML_NODE_ATT_IDX,h_index);
    }
}
