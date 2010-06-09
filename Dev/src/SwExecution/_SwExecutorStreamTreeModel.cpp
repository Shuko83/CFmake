/*!
 \file _SwExecutorStreamTreeModel.cpp
 \brief Modele pour la selection des composants executables
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
/*
  * INCLUDES LOCAUX
  */
#include <SwApplication.h>
#include <QMessageBox>
#include <QMimeData>
#include <SwAddress_ToolBox.h>
#include "_SwExecutorStreamTreeModel.h"
#include "ISwExecutable_Service.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/*! \brief Constructeur */
_SwExecutorStreamTreeModel::_SwExecutorStreamTreeModel(QObject * parent,SwComponent_Class * root_component):
												   QAbstractItemModel(parent){
    _root_component=root_component;
    _font_registered.setStrikeOut(true);
}
/*! \brief Destructeur*/
_SwExecutorStreamTreeModel::~_SwExecutorStreamTreeModel() {
}
/*! \brief Renvoie les capacites du modèle */
Qt::ItemFlags _SwExecutorStreamTreeModel::flags ( const QModelIndex & index ) const {
    SwComponent_Class * cparent;
	
    cparent=(SwComponent_Class*)index.internalPointer();
    ISwExecutable_Service * service=dynamic_cast<ISwExecutable_Service *>(cparent->QueryService(CG_SW_SERVICE_EXECUTABLE));
    if (index.column()==0) {
        if (service!=NULL && _registered_components.find(cparent)==_registered_components.end()) {
            return Qt::ItemFlags(Qt::ItemIsEnabled | Qt::ItemIsDragEnabled| Qt::ItemIsSelectable);    
        }
    }
    return Qt::ItemFlags(Qt::ItemIsEnabled);
}
/*! \brief list mimeTypes */
QStringList _SwExecutorStreamTreeModel::mimeTypes() const{
     QStringList types;
     types << "application/exe_instance";
     return types;
}
/*! \brief acces a la mimeData correspondantes */
QMimeData * _SwExecutorStreamTreeModel::mimeData(const QModelIndexList &indexes) const{
     QMimeData *mimeData = new QMimeData();
     SwComponent_Class * cparent;
     QString text;

     foreach (QModelIndex index, indexes) {
         if (index.isValid()) {
             cparent=(SwComponent_Class*)index.internalPointer();
             text=text+SwAddress_ToolBox::BuildAbsolutePath(cparent);
             text+= ";";
         }
     }
     text+"\n";

     //mimeData->setText(text);
     mimeData->setData("application/exe_instance",text.toLatin1());
     return mimeData;
}

/*! \brief Renvoie le nombre de colonne pour parent donné */
int _SwExecutorStreamTreeModel::columnCount ( const QModelIndex & parent ) const{
    return 2;
}		
/*! \brief Renvoie le nombre de ligne pour un parent donné */
int _SwExecutorStreamTreeModel::rowCount ( const QModelIndex & parent ) const {
    SwComponent_Class * cparent;
    if (_root_component==NULL)
        return 0;
    if (!parent.isValid())
        return 1;
    cparent=(SwComponent_Class *)parent.internalPointer();
    return cparent->GetNbChildren();    
}
/*! \brief Renvoie les données d'entete */
QVariant _SwExecutorStreamTreeModel::headerData ( int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        switch(section) {
            case 0:
                return QVariant(QString("Component"));
                break;
            case 1:
                return QVariant(QString("Description"));
                break;
            default:
                break;
        }
    }
    return QVariant();
}
/*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
QVariant _SwExecutorStreamTreeModel::data ( const QModelIndex & index, int role ) const{
    SwComponent_Class * cparent;
    SwComponent_Class * cparentparent;
     if (!index.isValid())
         return QVariant();
    cparent=(SwComponent_Class*)index.internalPointer();
    ISwExecutable_Service * service=dynamic_cast<ISwExecutable_Service *>(cparent->QueryService(CG_SW_SERVICE_EXECUTABLE));

    if (role == Qt::DisplayRole && index.column()==0) {
        return QVariant(cparent->GetName());
    }
    if (role == Qt::DisplayRole && index.column()==1) {
        return QVariant(cparent->GetDescription());
    }
    if (role == Qt::DecorationRole && index.column()==0) {
        return QVariant(SW_APP->ComponentsBank().GetComponentIcon(cparent->GetFactoryComponentName()));
    }
    if (role == Qt::ForegroundRole) {
        if (service!=NULL) {
            return QVariant(Qt::black);
        } else {
            return QVariant(Qt::gray);
        }
    }
    if (role == Qt::FontRole) {
        if (service!=NULL && _registered_components.find(cparent)!=_registered_components.end()) {
            return QVariant(_font_registered);
        }
    }
    return QVariant();
}
/*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
QModelIndex _SwExecutorStreamTreeModel::index ( int row, int column, const QModelIndex & parent  ) const{
    SwComponent_Class * cparent;
    if (_root_component==NULL)
        return QModelIndex();
    if (!parent.isValid()) 
        return createIndex(row,column,(void *)_root_component);

    cparent=(SwComponent_Class *)parent.internalPointer();
    return createIndex(row,column,(void *)cparent->GetChild(row));
}
/*! \brief Renvoie l'item parent d'un item index donné */
QModelIndex _SwExecutorStreamTreeModel::parent ( const QModelIndex & index ) const{
    SwComponent_Class * cindex;
    SwComponent_Class * parent;
    int index_parent;
    SwComponent_Class * current_comp;
    if (!index.isValid())
        return QModelIndex();
    cindex=(SwComponent_Class *)index.internalPointer();
    parent=cindex->GetParent();
    if (parent==NULL)
        return QModelIndex();
    //Calcul de l'indice du parent
    cindex=parent;
    parent=cindex->GetParent();
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
/*! \brief slot de reception d'une liste de path de composant ajouté */
void _SwExecutorStreamTreeModel::OnExeAdded ( const QStringList & comps ) {
    QString item;
    SwComponent_Class * comp;

    emit layoutAboutToBeChanged();
    foreach(item,comps) {
        comp=SwAddress_ToolBox::FindTarget(item, _root_component);
        if ((comp!=NULL) && (_registered_components.find(comp)==_registered_components.end())) {
            _registered_components.insert(comp);   
        }
    }
    emit layoutChanged();
}
/*! \brief slot de reception d'une liste de path de composant supprimé */
void _SwExecutorStreamTreeModel::OnExeRemoved ( const QStringList & comps ) {
    QString item;
    SwComponent_Class * comp;
    QSet<SwComponent_Class *>::iterator it;

    emit layoutAboutToBeChanged();
    foreach(item,comps) {
        comp=SwAddress_ToolBox::FindTarget(item, _root_component);
        if (comp!=NULL) {
            it=_registered_components.find(comp);
            if (it!=_registered_components.end()) {
                _registered_components.erase(it);  
            }
        }
    }
    emit layoutChanged();

}
