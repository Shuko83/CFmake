/*!
 \file SwWizardNodeModel.h
 \brief NodeModel
 \version 1.0
 \date Wed May 02 16:54:21 GMT 2007
 \author big
*/

#ifndef _SwWizardNodeModel_H
#define _SwWizardNodeModel_H


#include <Qt>
#include <QAbstractItemModel>

/*
  * INCLUDES LOCAUX
  */
#include <ISwVisitor.h>
#include "SwRefPtr.h"
#include "SwSimpleComponent_Class.h"
#include "NodePropertiesModelImpl.h"

using namespace StreamWork::SwCore;

/*!
	\class SwWizardNodeModel
	\brief NodeModel
*/
class SwWizardNodeModel : public QAbstractItemModel, public ISwVisitor{
	Q_OBJECT
protected:
    typedef enum {VConnexion,VDeconnexion,VNone} _T_VisitAction;
    /*! \brief Root */
    SwRefPtr<SwSimpleComponent_Class> _root;
    /*! \brief Action component */
    SwRefPtr<SwSimpleComponent_Class> _action_comp;
    /*! \brief index sur lequel s'effectue l'action */
    QModelIndex _action_index;
    /*! \brief index sur le parent de l'ajout ou de la suppression*/
    QModelIndex _parent_index;
    /*! \brief index sur l'enfant de l'ajout ou de la suppression*/
    int _index_of_child;
    /*! \brief Action sur la visite */
    _T_VisitAction _visit_action;
    /*! \brief modele pour les propriétés */
    NodePropertiesModelImpl * _model_properties;
protected:
    /*! \brief definition de l'index d'action */
    void SetActionIndex(const QModelIndex & index);

public:
    /*! \brief Constructeur */
    SwWizardNodeModel(QObject * parent,SwSimpleComponent_Class * root,NodePropertiesModelImpl * model_properties);
    /*! \brief Destructeur */
    virtual ~SwWizardNodeModel();
	//-------------------------------------------------------------
	// Interface ISwVisitor
	//------------------------------------------------------------
	/*! \brief methode de visite */
    void Visit(SwComponent_Class * visited);
	//-------------------------------------------------------------
	// Callback swcomponent
	//------------------------------------------------------------
	/*! \brief sur avant l'ajout d'enfant */
    void OnBeforeAddChild(SwComponent_Class *parent,SwComponent_Class *child);
	/*! \brief sur ajout d'enfant */
    void OnAddChild(SwComponent_Class *parent,SwComponent_Class *child);
	/*! \brief sur retrait d'enfant */
    void OnRemoveChild(SwComponent_Class *parent,SwComponent_Class *child);
	/*! \brief sur apres le retrait d'enfant */
    void OnAfterRemoveChild(SwComponent_Class *parent);
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
public slots:
    /*! \brief Sur demande de menu contextuel */
    void OnMenuRequested( const QPoint & globalpos,QModelIndex index);
    /*! \brief Sur click */
    void OnClicked(const QModelIndex & index);

};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
