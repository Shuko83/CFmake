/*!
 \file SwWizardNodeModel.cpp
 \brief NodeModel
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/

#include <QMetaObject>
#include <QModelIndex>
#include <QMenu>
#include <QMetaMethod>
#include <QAction>
#include <Qt>
#include <SwApplication.h>
#include <SwMacros.h>
#include "SwWizardNodeModel.h"

using namespace StreamWork::SwCore;
/*! \brief Constructeur */
SwWizardNodeModel::SwWizardNodeModel(QObject * parent,SwSimpleComponent_Class * root,NodePropertiesModelImpl * model_properties):QAbstractItemModel(parent) {
    _root=root;
    _action_index=QModelIndex();
    _parent_index=QModelIndex();
    _visit_action=VConnexion;
    _root->AcceptVisitor(this);
    _visit_action=VNone;
    _action_comp=NULL;
    _model_properties=model_properties;
}
/*! \brief Destructeur */
SwWizardNodeModel::~SwWizardNodeModel(){
    _visit_action=VDeconnexion;
    _root->AcceptVisitor(this);
}
//-------------------------------------------------------------
// Interface ISwVisitor
//------------------------------------------------------------
/*! \brief methode de visite */
void SwWizardNodeModel::Visit(SwComponent_Class * visited) {
    switch (_visit_action) {
        case VConnexion:
            visited->OnBeforeAddChild.iconnect(*this,&SwWizardNodeModel::OnBeforeAddChild);
            visited->OnAddChild.iconnect(*this,&SwWizardNodeModel::OnAddChild);
            visited->OnRemoveChild.iconnect(*this,&SwWizardNodeModel::OnRemoveChild);
            visited->OnAfterRemoveChild.iconnect(*this,&SwWizardNodeModel::OnAfterRemoveChild);
            break;
        case VDeconnexion:
            visited->OnBeforeAddChild.idisconnect(*this,&SwWizardNodeModel::OnBeforeAddChild);
            visited->OnAddChild.idisconnect(*this,&SwWizardNodeModel::OnAddChild);
            visited->OnRemoveChild.idisconnect(*this,&SwWizardNodeModel::OnRemoveChild);
            visited->OnAfterRemoveChild.idisconnect(*this,&SwWizardNodeModel::OnAfterRemoveChild);
            break;
        default:
            break;
    }
}
//-------------------------------------------------------------
// Callback swcomponent
//------------------------------------------------------------
/*! \brief sur avant l'ajout d'enfant */
void SwWizardNodeModel::OnBeforeAddChild(SwComponent_Class *parent,SwComponent_Class *child) {
}
/*! \brief sur ajout d'enfant */
void SwWizardNodeModel::OnAddChild(SwComponent_Class *parent,SwComponent_Class *child) {
    if (!_action_index.isValid())
        return;
    if (parent==(SwComponent_Class *)_action_index.internalPointer()) {
        _parent_index=_action_index;    
    }
    if (child==(SwComponent_Class *)_action_index.internalPointer()) {
        _parent_index=this->parent(_action_index);
    }
    if (!_parent_index.isValid())
        return;
    _index_of_child=parent->GetChildIndex(child);
    beginInsertRows(_parent_index,_index_of_child,_index_of_child);
    //emit layoutAboutToBeChanged();
    _visit_action=VConnexion;
    child->AcceptVisitor(this);
    _visit_action=VNone;
    //emit layoutChanged();
    endInsertRows();
    _parent_index=QModelIndex();
}
/*! \brief sur retrait d'enfant */
void SwWizardNodeModel::OnRemoveChild(SwComponent_Class *parent,SwComponent_Class *child) {
    if (!_action_index.isValid())
        return;
    if (parent==(SwComponent_Class *)_action_index.internalPointer()) {
        _parent_index=_action_index;    
    }
    if (child==(SwComponent_Class *)_action_index.internalPointer()) {
        _parent_index=this->parent(_action_index);
    }
    if (_action_comp==child) {
        SetActionIndex(QModelIndex());
    }
    if (!_parent_index.isValid())
        return;
    _index_of_child=parent->GetChildIndex(child);
    beginRemoveRows(_parent_index,_index_of_child,_index_of_child);
    //emit layoutAboutToBeChanged();
    _visit_action=VDeconnexion;
    child->AcceptVisitor(this);
    _visit_action=VNone;
}
/*! \brief sur apres le retrait d'enfant */
void SwWizardNodeModel::OnAfterRemoveChild(SwComponent_Class *parent) {
    if (!_parent_index.isValid())
        return;
    //emit layoutChanged();
    endRemoveRows();
    _parent_index=QModelIndex();
}
//-------------------------------------------------------------
// Interface QAbstractItemModel
//------------------------------------------------------------
/*! \brief Renvoie les capacites du modèle */
Qt::ItemFlags SwWizardNodeModel::flags ( const QModelIndex & index ) const{
	return Qt::ItemFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
}
/*! \brief Renvoie le nombre de colonnes pour les enfants d'un parent donné */
int SwWizardNodeModel::columnCount ( const QModelIndex & parent) const{
	return 1;
}			
 /*! \brief Renvoie le nombre de ligne pour un parent donné */
int SwWizardNodeModel::rowCount ( const QModelIndex & parent) const{
    SwSimpleComponent_Class * cparent;
    if (!parent.isValid())
        return 1;
    cparent=(SwSimpleComponent_Class *)parent.internalPointer();
    return cparent->GetNbChildren();    
}
/*! \brief Renvoie les données d'entete */
QVariant SwWizardNodeModel::headerData ( int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        switch(section) {
            case 0:
                return QVariant(QString("Plugin description"));
            default:
                break;
        }
    }
    return QVariant();
}
/*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
QVariant SwWizardNodeModel::data ( const QModelIndex & index, int role) const{
    SwSimpleComponent_Class * cparent;
    if (!index.isValid())
        return QVariant();
    cparent=(SwSimpleComponent_Class *)index.internalPointer();
    if (role == Qt::DisplayRole && index.column()==0) {
        return QVariant(cparent->GetNodeLabelVisu());
    }
    if (role == Qt::DecorationRole && index.column()==0) {
        return QVariant(SW_APP->ComponentsBank().GetComponentIcon(cparent->GetFactoryComponentName()));
    }
    return QVariant();
}
/*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
QModelIndex SwWizardNodeModel::index ( int row, int column, const QModelIndex & parent) const{
    SwSimpleComponent_Class * cparent;
    if (!parent.isValid()) 
        return createIndex(row,column,(void *)_root);

    cparent=(SwSimpleComponent_Class *)parent.internalPointer();
    return createIndex(row,column,(void *)cparent->GetChild(row));

}
/*! \brief Renvoie l'item parent d'un item index donné */
QModelIndex SwWizardNodeModel::parent ( const QModelIndex & index ) const{
    SwSimpleComponent_Class * cindex;
    SwSimpleComponent_Class * parent;
    int index_parent;

    if (!index.isValid())
        return QModelIndex();
    cindex=(SwSimpleComponent_Class *)index.internalPointer();
    parent=(SwSimpleComponent_Class *)cindex->GetParent();
    if (parent==NULL)
        return QModelIndex();
    //Calcul de l'indice du parent
    cindex=parent;
    parent=(SwSimpleComponent_Class *)cindex->GetParent();
    //Si le parent du parent est null, c'est le composant racine
    if (parent==NULL) {
        return createIndex(0,0,(void *)cindex);    
    } else {
        //Sinon il faut calculer l'index
        index_parent=parent->GetChildIndex(cindex);
        if (index_parent==-1)
            return QModelIndex();
    }
    return createIndex(index_parent,0,(void *)cindex);   
}
/*! \brief Sur demande de menu contextuel */
void SwWizardNodeModel::OnMenuRequested( const QPoint & globalpos,QModelIndex index) {

    SetActionIndex(index);
    const QMetaObject  * m_obj=_action_comp->metaObject();
    QMenu * menu=new QMenu();
    for(int i=0;i<m_obj->methodCount();i++) {
        QMetaMethod methode=m_obj->method(i);
        if (methode.methodType()==QMetaMethod::Slot) {
            QString action_title=QString(methode.signature());
            QString target_method="1"+QString(methode.signature());
            if (action_title.startsWith(QString("Action_"))) {
                QStringList atl=action_title.split("_");
                atl.pop_front();
                action_title=atl.join(" ");
                action_title=action_title.remove(QChar('('));
                action_title=action_title.remove(QChar(')'));
                QAction * action=menu->addAction(action_title);
                action->connect(action,SIGNAL(triggered()),_action_comp,target_method.toAscii().data());
            }
        }
    }
    menu->exec(globalpos);
    
    delete menu;
}
/*! \brief Sur click */
void SwWizardNodeModel::OnClicked(const QModelIndex & index) {
    SetActionIndex(index);
}
/*! \brief definition de l'index d'action */
void SwWizardNodeModel::SetActionIndex(const QModelIndex & index) {
    if (index.isValid()) {
        _action_comp=(SwSimpleComponent_Class *)index.internalPointer();
        _action_index=index;
        _model_properties->SetProperties(dynamic_cast<ISwProperties *>(_action_comp->QueryService(CG_SW_SERVICE_PROPERTIES)),_action_comp->GetNodeLabel());
    } else {
        _action_comp=NULL;
        _action_index=QModelIndex();
        _model_properties->SetProperties(NULL,QString());
    }
}
