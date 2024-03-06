/*!
 \file _SwModelStreamTreeModel.h
 \brief Modele pour la selection du model host
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwModelStreamTreeModel_H
#define __SwModelStreamTreeModel_H

/*
  * INCLUDES GLOBAUX
  */
#include <Qt>
#include <QSet>
#include <QAbstractItemModel>
#include <QIcon>
#include <QFont>
#include <QColor>
/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"
#include "_SwModelHost_Class.h"

using namespace StreamWork::SwCore;

/*!
	\class _SwModelStreamTreeModel 
	\brief Modele pour la selection du model host
*/
class _SwModelStreamTreeModel : public QAbstractItemModel{
	Q_OBJECT
private:
	/*! \brief composant racine */
	SwComponent_Class * _root_component;
	/*! \brief composant racine */
    _SwModelHost_Class * _current_selected;
public:
    /*! \brief Constructeur */
	_SwModelStreamTreeModel(QObject * parent,SwComponent_Class * root_component);
    /*! \brief Destructeur */
	virtual ~_SwModelStreamTreeModel();
    /*! \brief Definition du selectionne */
    void SetSelected(_SwModelHost_Class * current_selected);
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
    /*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
    virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    /*! \brief Renvoie l'item parent d'un item index donné */
    virtual QModelIndex parent ( const QModelIndex & index ) const;


};
#endif 
