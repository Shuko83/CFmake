/*!
 \file _SwExecutorStreamTreeModel.h
 \brief Modele pour la selection des composants executables
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwExecutorStreamTreeModel_H
#define __SwExecutorStreamTreeModel_H

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

using namespace StreamWork::SwCore;

/*!
	\class _SwExecutorStreamTreeModel 
	\brief Modele pour la selection des composants executables
*/
class _SwExecutorStreamTreeModel : public QAbstractItemModel{
	Q_OBJECT
private:
	/*! \brief composant racine */
	SwComponent_Class * _root_component;
	/*! \brief composant racine */
	QSet<SwComponent_Class *> _registered_components;
    /*! font pour les items deja entrés */
    QFont _font_registered;
public:
    /*! \brief Constructeur */
	_SwExecutorStreamTreeModel(QObject * parent,SwComponent_Class * root_component);
    /*! \brief Constructeur */
	virtual ~_SwExecutorStreamTreeModel();
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
public slots:
    /*! \brief slot de reception d'une liste de path de composant ajouté */
    void OnExeAdded ( const QStringList & comps );
    /*! \brief slot de reception d'une liste de path de composant supprimé */
    void OnExeRemoved ( const QStringList & comps );

};
#endif 
