/**
@file PluginsListModel.cpp
@brief Qt Model pour la liste des plugins
@author F.Bighelli
 */

#include "PluginsListModel.h"
 
using namespace StreamWork::SwCore;

//-----------------------------------------------------------------------
PluginsListModel::PluginsListModel(QObject * parent) :QAbstractListModel(parent)
{
	QMap<QString,StreamWork::SwCore::SwPluginFactory_Class *> *tmpList=SW_APP->ComponentsBank().GetAllPlugins();
    _pList=new QMap<QString,StreamWork::SwCore::SwPluginFactory_Class *>();
    QMap<QString,SwPluginFactory_Class *>::iterator it=tmpList->begin();
    while(it!=tmpList->end()) 
	{
		if (it.value()->GetComponentsList().size() != 0)
			_pList->insert(it.key().toLower(),it.value());

        it++;
    }
}

//-----------------------------------------------------------------------
PluginsListModel::~PluginsListModel()
{
    delete _pList;
}

//-----------------------------------------------------------------------
int PluginsListModel::rowCount(const QModelIndex & parent) const
{
    return _pList->size();
}

//-----------------------------------------------------------------------
QModelIndex PluginsListModel::index(int row, int column, const QModelIndex & parent) const
{
    QMap<QString,SwPluginFactory_Class *>::iterator it;
    it=_pList->begin()+ row;   
    return createIndex(row,column,(*it));
}

//-----------------------------------------------------------------------
QVariant PluginsListModel::data(const QModelIndex & index, int role) const
{
    QMap<QString,SwPluginFactory_Class *>::iterator it;
    if (role == Qt::DisplayRole) 
	{
        it=_pList->begin();
        it+=index.row();
        QString pname=(*it)->GetPluginName();
        int finsuffix=pname.lastIndexOf(".");

        if (finsuffix>0) 
            pname.truncate(finsuffix);
        return QVariant(QString(pname));
    }

    if (role == Qt::ToolTipRole) 
	{
        it=_pList->begin();
        it+=index.row();
        QString pname=(*it)->GetPath();
        return QVariant(QString(pname));
    }
    return QVariant();
}
