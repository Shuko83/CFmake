/*!
 \file SwSimpleComponent_Class.cpp
 \brief Component simple
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/


#include <QVariant>
#include <QList>
#include <QDomText>
#include <QDomImplementation>
#include <SwEnum.h>
#include <SwApplication.h>
#include <SwMacros.h>
#include <ISwProperty.h>
#include "SwSimpleComponent_Class.h"
#include <QMessageBox>

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
SwSimpleComponent_Class::SwSimpleComponent_Class():SwComponent_Class() {
    _properties_service=NULL;
	
}
/*! \brief Destructeur */
SwSimpleComponent_Class::~SwSimpleComponent_Class() {
    this->UnregisterService(_properties_service->GetServiceName());
    delete _properties_service;
}
/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void SwSimpleComponent_Class::InitializeResources() throw(SwException) {
    _properties_service=new SwProperties_Class(this);
    this->RegisterService(_properties_service);
    _properties_service->CreatePropertiesForQObject(this,QString(),true);
}
/* internal save */
QDomElement SwSimpleComponent_Class::InternalSave(QDomDocument & doc) {
    QDomElement node_elt=doc.createElement(GetNodeLabel());
    //Sauvegarde des attributs
    InternalSaveAttributs(node_elt,doc);
    //Sauvegarde des enfants
    InternalSaveChildrens(node_elt,doc);
    //Fin
    return node_elt;
}
/* internal save attributs*/
void SwSimpleComponent_Class::InternalSaveAttributs(QDomElement & parent,QDomDocument & doc) {
    QVariant var,tmp;
    QList<ISwProperty *> plist=_properties_service->GetProperties();

    for (int i=0;i<plist.count();i++) {
        bool save_done=false;
        var=plist[i]->GetValue();
        //Type stringlist
        if (var.type()==QVariant::StringList) {
            QDomText text_node=doc.createTextNode(var.toStringList().join(QString("\n")));
            parent.appendChild(text_node);
            save_done=true;
        }
        //Type string
        if (!save_done && var.type()==QVariant::String) {
            parent.setAttribute(plist[i]->GetName(),var.toString());   
            save_done=true;
        }
        //Type bool
        if (!save_done && var.type()==QVariant::Bool) {
            parent.setAttribute(plist[i]->GetName(),var.toString());   
            save_done=true;
        }
        //Type enum
        if (!save_done && var.userType()==qMetaTypeId<SwEnum>()) {
            SwEnum enum_value=var.value<SwEnum>();
            parent.setAttribute(plist[i]->GetName(),enum_value.ToString());
            save_done=true;
        }
    }
}
/* internal save childrens*/
void SwSimpleComponent_Class::InternalSaveChildrens(QDomElement & parent,QDomDocument & doc) {
    SwComponent_Class * comp= GetFirstChild();
    SwSimpleComponent_Class * scomp;
    QDomElement node_elt;
    while (comp!=NULL) {
        scomp=dynamic_cast<SwSimpleComponent_Class *>(comp);
        if (scomp!=NULL) {
            parent.appendChild(scomp->InternalSave(doc));        
        }
        comp=GetNextChild();
    }
}
/*! \brief Sauvegarde*/
void SwSimpleComponent_Class::Save(QDomDocument & doc) {
    if (QDomImplementation::invalidDataPolicy()!=QDomImplementation::ReturnNullNode) {
        QDomImplementation::setInvalidDataPolicy(QDomImplementation::ReturnNullNode);
    }
    doc.appendChild(InternalSave(doc));
}
/*! label noeud*/
QString  SwSimpleComponent_Class::GetNodeLabel() {
    return QString("Undefined");
}
/*! label noeud*/
QString SwSimpleComponent_Class::GetNodeLabelVisu() {
    return GetNodeLabel();
}
/*! slot destruction enfants*/
void SwSimpleComponent_Class::Action_Delete() {
    if (GetParent()!=NULL) {
        GetParent()->RemoveChild(this);
    } else {
        QMessageBox::warning(NULL,"Wizard Alert","Unable to delete root");
    }
}
