/**
@file ComponentListModel.cpp
@brief Qt Model pour la liste des composants d'un plugin
@author F.Bighelli
 */

#include "ComponentListModel.h"

using namespace StreamWork::SwCore;

/** @brief Constructor */
ComponentListModel::ComponentListModel( QObject * parent):QAbstractListModel(parent) {
	_plugin=0;
}
/** @brief rowCount */
int ComponentListModel::rowCount ( const QModelIndex & parent) const{
    return _cList.count();
} 
/** @brief index */
QModelIndex ComponentListModel::index ( int row, int column, const QModelIndex & parent  ) const{
    return createIndex(row,column);
}
/** @brief data */
QVariant ComponentListModel::data ( const QModelIndex & index, int role ) const {
    if (role==Qt::DisplayRole) {
        return QVariant(_cList[index.row()]); 
    }
    if (role==Qt::DecorationRole) {
        return QVariant(_plugin->GetComponentIcon(_cList[index.row()])); 
    }
    if (role==Qt::ToolTipRole) {
        return QVariant(_plugin->GetComponentDescription(_cList[index.row()])); 
    }
    return QVariant();
}
/*! \brief Flags du modele*/
Qt::ItemFlags ComponentListModel::flags(const QModelIndex &index) const {
    return Qt::ItemIsDragEnabled |  QAbstractListModel::flags(index);
}
/*! \brief list mimeTypes */
QStringList ComponentListModel::mimeTypes() const{
     QStringList types;
     types << "application/mod_list";
     types << "application/mod";
     return types;
}
/*! \brief acces a la mimeData correspondantes */
QMimeData * ComponentListModel::mimeData(const QModelIndexList &indexes) const{
     QMimeData *mimeData = new QMimeData();
     QString text;

     if (indexes.count()==1) {
         text=data(indexes.front(), Qt::DisplayRole).toString();
         mimeData->setData("application/mod",text.toLatin1());
         return mimeData;
     }

     foreach (QModelIndex index, indexes) {
         if (index.isValid()) {
             text=text+data(index, Qt::DisplayRole).toString();
             text+= ";";
         }
     }

     //mimeData->setText(text);
     mimeData->setData("application/mod_list",text.toLatin1());
     return mimeData;
}

/** @brief sur changement du plugin activé */
void ComponentListModel::onSelectedPluginChanged(const QModelIndex & index) {
    if (index.isValid()) {
        _plugin=static_cast<SwPluginFactory_Class *>(index.internalPointer());
        _cList=_plugin->GetComponentsList().toList();
        _cList.sort();
        reset();
    } else {
        _plugin=0;
        _cList.clear();
        reset();
    }
}
