/*!
 \file SwEnumModel.cpp
 \brief Classe pour la gestion des enumeres
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli

*/

#include "SwEnumModel.h"
#include "SwException.h"
#include "SwMacros.h"
#include "_SwEnumKeys_Class.h"

using namespace StreamWork::SwCore;


/*! \brief Constructeur de base */
SwEnumModel::SwEnumModel(SwEnum * source,QObject * parent):QAbstractItemModel(parent) {
    _content=*source;
    _bridge_list=_content.GetValues()->keys(); 
}
/*! \brief Destructeur */
SwEnumModel::~SwEnumModel() {
}
/*! \brief Acces valeur interne enumere*/
SwEnum  SwEnumModel::GetEnum()  const {
    return _content;
}
/*! \brief Acces valeur pour un index donné*/
int SwEnumModel::GetVal(int index) const {
    //Test  validite index et validite numero de ligne 
    if (index<0 || index>=_bridge_list.count()) {
        return -1;    
    }
    return _bridge_list[index];   
}
/*! \brief Acces index pour une valeur donné*/
int SwEnumModel::GetIndex(int val) const {
    return _bridge_list.indexOf(val);  
}
//-----------------------------------------------------------------------------------
// Implementation QAbstractItemModel
//-----------------------------------------------------------------------------------
/*! \brief Compteur de ligne */
int SwEnumModel::rowCount(const QModelIndex &parent) const {
    return !parent.isValid() ? _bridge_list.count() : 0;
}
/*! \brief Compteur de colonne */
int SwEnumModel::columnCount(const QModelIndex &parent) const{
    return 1;
}
/*! \brief Acces Parent */
QModelIndex SwEnumModel::parent(const QModelIndex &index) const{
    return QModelIndex();
}
/*! \brief Acces Index */
QModelIndex SwEnumModel::index(int row, int column, const QModelIndex &parent) const{
    return createIndex(row, column);
}
/*! \brief Acces contenu */
QVariant SwEnumModel::data(const QModelIndex &index, int role) const{
    QMap<int,QString>::const_iterator it;
    QVariant val;

    //Test  validite index et validite numero de ligne 
    if (!index.isValid() || index.row()>=_bridge_list.count()) {
        return QVariant();    
    }
    //Acces a la valeur
    it=_content.GetValues()->find(_bridge_list[index.row()]);
    
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return it.value();
    case Qt::CheckStateRole:
        if (_content.IsFlag()) {
            if (it.key()!=0) {
                int et_op=_content.ToInt() & it.key();
                return (et_op==it.key()) ? Qt::Checked : Qt::Unchecked;
            } else {
                return (_content.ToInt()==0) ? Qt::Checked : Qt::Unchecked; 
            }
        }
        return QVariant();  
    default:
        break;
    } // end switch
    return QVariant();
}
///*! \brief Definition contenu */
bool SwEnumModel::setData(const QModelIndex &index, const QVariant &value, int role){
    QMap<int,QString>::const_iterator it;

    //Test  validite index et validite numero de ligne 
    if (!index.isValid() || index.row()>=_bridge_list.count()) {
        return false;    
    }
    //Acces a la valeur
    it=_content.GetValues()->find(_bridge_list[index.row()]);

    switch (role) {
    case Qt::CheckStateRole:
        if (_content.IsFlag()) {
            Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
            if (state == Qt::Unchecked) {
                _content.FromInt(_content.ToInt() & ~it.key());    
            } else {
                if (it.key()==0) 
                    _content.FromInt(0);
                else
                    _content.FromInt(_content.ToInt() | it.key());                    
            }
            dataChanged(index,index);
            //dataChanged(createIndex(0,index.column(), 0),createIndex(_bridge_list.count()-1,index.column(), 0));
        }
    default:
        break;
    } // end switch
    return true;

}
/*! \brief Flags items */
Qt::ItemFlags SwEnumModel::flags(const QModelIndex &index) const{
    //Test  validite index et validite numero de ligne 
    //if (!index.isValid() || index.row()>=_bridge_list.count()) {
    //    return 0;    
    //}

    //if (_content.IsFlag()) { 
    //    return Qt::ItemIsEnabled  | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable; 
    //} else {
    //    return Qt::ItemIsEnabled;
    //}
    if (_content.IsFlag()) { 
        return QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable; 
    } else {
        return QAbstractItemModel::flags(index) ;
    }
}
