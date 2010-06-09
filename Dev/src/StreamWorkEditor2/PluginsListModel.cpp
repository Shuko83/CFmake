/**
@file PluginsListModel.cpp
@brief Qt Model pour la liste des plugins
@author F.Bighelli
 */

#include "PluginsListModel.h"
 
using namespace StreamWork::SwCore;

/** @brief Constructor */
PluginsListModel::PluginsListModel(QObject * parent):QAbstractListModel(parent) {
	_pList=SW_APP->ComponentsBank().GetAllPlugins();
}
/** @brief rowCount */
int PluginsListModel::rowCount ( const QModelIndex & parent) const {
    return _pList->size();
}
/** @brief index */
QModelIndex PluginsListModel::index ( int row, int column, const QModelIndex & parent) const {
    QMap<QString,SwPluginFactory_Class *>::iterator it;
    it=_pList->begin()+ row;   
    return createIndex(row,column,(*it));
}
/** @brief data */
QVariant PluginsListModel::data ( const QModelIndex & index, int role) const {
    QMap<QString,SwPluginFactory_Class *>::iterator it;
    if (role == Qt::DisplayRole) {
        it=_pList->begin();
        it+=index.row();
        QString pname=(*it)->GetPluginName();
        int finsuffix=pname.lastIndexOf(".");
        if (finsuffix>0) {
            pname.truncate(finsuffix);
        }
        return QVariant(QString(pname));
    }
    return QVariant();
}
