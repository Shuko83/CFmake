/*!
 \file _SwConfigurationEntitiesModel.h
 \brief Modele et conteneur des données a exporter
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwConfigurationEntitiesModel_H
#define __SwConfigurationEntitiesModel_H

/*
  * INCLUDES GLOBAUX
  */
#include <Qt>
#include <QSet>
#include <QList>
#include <QAbstractItemModel>
#include <QIcon>
#include <QFont>
#include <QColor>
/*
  * INCLUDES LOCAUX
  */
#include <SwComponent_Class.h>
#include "_SwModelHost_Class.h"
#include "_SwConfigurationExportedEntity.h"


using namespace StreamWork::SwCore;

/*!
	\class _SwConfigurationEntitiesModel 
	\brief Modele et conteneur des données a exporter
*/
class _SwConfigurationEntitiesModel : public QAbstractItemModel{
	Q_OBJECT
public:
        /*! \brief Definition du type d'item */
    typedef enum {
		IT_Property,
		IT_Host,
		IT_Execution,
		IT_Perspective,
        IT_OwnerConfigurable
	} _T_ItemType;/*!
	    \class _Item
	    \brief Item pour la selection de l'export model
    */
    class _Item {
    public:
        /*! \brief Type de l'item */     
        _T_ItemType _type;
        /*! \brief path sur le host */     
        QString _host_path;
        /*! \brief Nom */     
        QString _name;
        /*! \brief Nom exporté*/     
        QString _exported_name;
        /*! \brief Constructeur */
        _Item(_T_ItemType type,QString name,QString exported_name,QString host_path) {
            _type=type;
            _name=name;
            _exported_name=exported_name;
            _host_path=host_path;
        }
    };
private:
	/*! \brief composant racine */
	SwComponent_Class * _root_component;
	/*! \brief liste des entites initiales */
    QList<_SwConfigurationExportedEntity *> * _entities_list;
	/*! \brief liste des items par category */
    QList<_Item *> _properties;
    QList<_Item *> _perspectives;
    QList<_Item *> _executions;
    QList<_Item *> _ownerConfigurables;

	bool _isLimitedToProperties;
public:
    /*! \brief Constructeur */
	_SwConfigurationEntitiesModel(QObject * parent,SwComponent_Class * root_component,QList<_SwConfigurationExportedEntity *> * entities_list);
    /*! \brief Destructeur */
	virtual ~_SwConfigurationEntitiesModel();
    /*! \brief Transformation liste entites en modele */
	void TransformEntitiesToModel();
    /*! \brief Transformation modele en liste entites */
	void TransformModelToEntities();
    /*! \brief Suppression des elements selectionnées */
	void RemoveSelectedItems(QModelIndexList list_to_remove);

	virtual void limitToProperty();

    //-------------------------------------------------------------
    // Interface QAbstractItemModel
    //------------------------------------------------------------
    /*! \brief Renvoie les capacites du modèle */
    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
    /*! \brief Renvoie le nombre de colonnes pour les enfants d'un parent donné */
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;			
        /*! \brief Renvoie le nombre de ligne pour un parent donné */
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    /*! \brief Renvoie les données d'entete */
    virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    /*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    /*! \brief Edition*/
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    /*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
    virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    /*! \brief Renvoie l'item parent d'un item index donné */
    virtual QModelIndex parent ( const QModelIndex & index ) const;
    /*! \brief Action de drop */
    virtual bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent ); 

};
#endif 
