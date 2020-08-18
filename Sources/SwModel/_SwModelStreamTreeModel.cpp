/*!
 \file _SwModelStreamTreeModel.cpp
 \brief Modele pour la selection du model host
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
#include "_SwModelStreamTreeModel.h"
#include "_SwModelHost_Class.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwModelStreamTreeModel::_SwModelStreamTreeModel(QObject * parent,SwComponent_Class * root_component):
												   QAbstractItemModel(parent){
    _root_component=root_component;
    _current_selected=NULL;
}
/*! \brief Destructeur*/
_SwModelStreamTreeModel::~_SwModelStreamTreeModel() {
}
/*! \brief Definition du selectionne */
void _SwModelStreamTreeModel::SetSelected(_SwModelHost_Class * current_selected) {
    _current_selected=current_selected;
    emit layoutChanged();
}
/*! \brief Renvoie les capacites du modèle */
Qt::ItemFlags _SwModelStreamTreeModel::flags ( const QModelIndex & index ) const {
    SwComponent_Class * cparent;

    cparent=(SwComponent_Class*)index.internalPointer();
    if (index.column()==0) {
        if (dynamic_cast<_SwModelHost_Class *>(cparent)!=NULL) {
            //return Qt::ItemFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            return Qt::ItemFlags(Qt::ItemIsEnabled);
        }
    }
    return Qt::ItemFlags();
}
/*! \brief Renvoie le nombre de colonne pour parent donné */
int _SwModelStreamTreeModel::columnCount ( const QModelIndex & parent ) const{
    return 1;
}
/*! \brief Renvoie le nombre de ligne pour un parent donné */
int _SwModelStreamTreeModel::rowCount ( const QModelIndex & parent ) const {
    SwComponent_Class * cparent;
    if (_root_component==NULL)
        return 0;
    if (!parent.isValid())
        return 1;
    cparent=(SwComponent_Class *)parent.internalPointer();
    return cparent->GetNbChildren();
}
/*! \brief Renvoie les données d'entete */
QVariant _SwModelStreamTreeModel::headerData ( int section, Qt::Orientation orientation, int role) const {
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
QVariant _SwModelStreamTreeModel::data ( const QModelIndex & index, int role ) const{
    SwComponent_Class * cparent;
     if (!index.isValid())
         return QVariant();
    cparent=(SwComponent_Class*)index.internalPointer();

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
        if (dynamic_cast<_SwModelHost_Class *>(cparent)!=NULL) {
            if (_current_selected==dynamic_cast<_SwModelHost_Class *>(cparent)) {
                return QVariant(QColor(Qt::red));
            } else {
				return QVariant(QColor(Qt::black));
            }
        } else {
			return QVariant(QColor(Qt::gray));
        }
    }
    return QVariant();
}
/*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
QModelIndex _SwModelStreamTreeModel::index ( int row, int column, const QModelIndex & parent  ) const{
    SwComponent_Class * cparent;
    if (_root_component==NULL)
        return QModelIndex();
    if (!parent.isValid())
        return createIndex(row,column,(void *)_root_component);

    cparent=(SwComponent_Class *)parent.internalPointer();
    return createIndex(row,column,(void *)cparent->GetChild(row));
}
/*! \brief Renvoie l'item parent d'un item index donné */
QModelIndex _SwModelStreamTreeModel::parent ( const QModelIndex & index ) const{
    SwComponent_Class * cindex;
    SwComponent_Class * parent;
    int index_parent;
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
