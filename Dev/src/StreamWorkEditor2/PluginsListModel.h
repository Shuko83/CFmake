/**
@file PluginsListModel.h
@brief Qt Model pour la liste des plugins
@author F.Bighelli
 */

#ifndef _PLUGINSLISTMODEL_H
#define _PLUGINSLISTMODEL_H

#include <QtCore>
#include <SwApplication.h>
#include <SwMacros.h>
#include <SwPluginFactory_Class.h>
/**
@class PluginsListModel
@brief Qt Model pour la liste des plugins
*/
class PluginsListModel : public QAbstractListModel {
    Q_OBJECT
public:
	/** @brief Constructor */
	PluginsListModel( QObject * parent = 0);
	/** @brief Destructor */
	~PluginsListModel();
    /** @brief rowCount */
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const; 
	/** @brief index */
    virtual QModelIndex index ( int row, int column = 0, const QModelIndex & parent = QModelIndex() ) const;
	/** @brief data */
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

private:
    /** @brief map des plugins */
    QMap<QString,StreamWork::SwCore::SwPluginFactory_Class *> * _pList;
    /** @brief list des noms de plugins */
    QStringList _nList; 
};

#endif
