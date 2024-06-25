/*!
 \file _SwConfigurationStreamTreeModel.h
 \brief Modele pour la selection de l'export model
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwConfigurationStreamTreeModel_H
#define __SwConfigurationStreamTreeModel_H

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
#include "Component/Base/SwComponent_Class.h"

using namespace StreamWork::SwCore;

/*!
	\class _SwConfigurationStreamTreeModel 
	\brief Modele pour la selection de l'export model
*/
class _SwConfigurationStreamTreeModel : public QAbstractItemModel{
	Q_OBJECT
public:
    /*! \brief Definition du type d'item */
    typedef enum {
		IT_Property,
		IT_Host,
		IT_Execution,
		IT_Perspective,
		IT_OwnerConfigurable
	} _T_ItemType;
    /*!
	    \class _Item
	    \brief Item pour la selection de l'export model
    */
    class _Item {
    public:
        /*! \brief Type de l'item */     
        _T_ItemType _type;
        /*! \brief Handle sur le host */     
        SwComponent_Class * _host;
        /*! \brief Nom */     
        QString _name;
        /*! \brief Nom exporté*/     
        QString _exported_name;
        /*! \brief Enfants*/     
        QList<_Item *> _childs;
        /*! \brief Parent*/     
        _Item * _parent;
        /*! \brief Constructeur */
        _Item(_Item * parent,SwComponent_Class * host,_T_ItemType type,QString name);
        /*! \brief Destructeur */
        ~_Item();
    };
private:
	/*! \brief composant racine */
	SwComponent_Class * _root_component;
	/*! \brief item racine */
	_Item * _root_item;
	/*! \brief composant racine */
	QSet<SwComponent_Class *> _registered_components;
    /*! font pour les items deja entrés */
    QFont _font_registered;

	QIcon _icoProperty;
	QIcon _icoInterfaceOut;
	QIcon _icoExecutor;
	QIcon _icoOwnerConfigurable;
	QIcon _icoModelHost;
	
	bool _isLimitedToProperty;
public:
    /*! \brief Constructeur */
	_SwConfigurationStreamTreeModel(QObject * parent,SwComponent_Class * root_component, bool islimitedToProperty=false);
    /*! \brief Constructeur */
	virtual ~_SwConfigurationStreamTreeModel();
    /*! \brief Construction des items */
	virtual void BuildItems(SwComponent_Class * comp,_Item * item);

    //-------------------------------------------------------------
    // Interface QAbstractItemModel
    //------------------------------------------------------------
    /*! \brief Renvoie les capacites du modèle */
    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
    /*! \brief list mimeTypes */
    QStringList mimeTypes() const;
    /*! \brief acces a la mimeData correspondantes */
    QMimeData * mimeData(const QModelIndexList &indexes) const;
    /*! \brief Renvoie le nombre de colonnes pour les enfants d'un parent donné */
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;			
        /*! \brief Renvoie le nombre de ligne pour un parent donné */
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    /*! \brief Renvoie les données d'entete */
    virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    /*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    /*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
    virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    /*! \brief Renvoie l'item parent d'un item index donné */
    virtual QModelIndex parent ( const QModelIndex & index ) const;

};
#endif 
