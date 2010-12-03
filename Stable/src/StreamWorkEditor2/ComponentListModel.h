/**
@file ComponentListModel.h
@brief Qt Model pour la liste des composants d'un plugin
@author F.Bighelli
 */

#ifndef _COMPONENTLISTMODEL_H
#define _COMPONENTLISTMODEL_H

#include <QtCore>
#include <SwApplication.h>
#include <SwMacros.h>
#include <SwPluginFactory_Class.h>

/**
@class ComponentListModel
@brief Qt Model pour la liste des composants d'un plugin
*/
class ComponentListModel : public QAbstractListModel {
    Q_OBJECT
public:
	/** @brief Constructor */
	ComponentListModel( QObject * parent = 0);
    /** @brief rowCount */
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const; 
	/** @brief index */
    virtual QModelIndex index ( int row, int column = 0, const QModelIndex & parent = QModelIndex() ) const;
	/** @brief data */
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    /*! \brief Flags du modele*/
    Qt::ItemFlags flags(const QModelIndex &index) const;
    /*! \brief list mimeTypes */
    QStringList mimeTypes() const;
    /*! \brief acces a la mimeData correspondantes */
    QMimeData * mimeData(const QModelIndexList &indexes) const;
public slots:
    /** @brief sur changement du plugin activé */
    void onSelectedPluginChanged(const QModelIndex & index);
private:
    /** @brief Liste des composants */
    QStringList _cList;
    /** @brief related plugin */
    StreamWork::SwCore::SwPluginFactory_Class * _plugin;
};

#endif
