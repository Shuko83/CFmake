/*!
\file _SwModelHostSelector.h
\date 12/04/2006
\brief Ihm de selection du model host
\author  Big
\version 1.0
 */

#ifndef __SwModelHostSelector_H
#define __SwModelHostSelector_H
/*
  * INCLUDES GLOBAUX
  */

#include <QDialog>
#include <QList>
#include <QTreeView>
/*
  * INCLUDES LOCAUX
  */

#include "Component/Base/SwComponent_Class.h"
#include "_SwModelHost_Class.h"
#include "_SwModelStreamTreeModel.h"

using namespace StreamWork::SwCore;

/*!
	\class _SwModelHostSelector 
	\brief Ihm de selection du model host
*/
class _SwModelHostSelector : public QDialog
{
    Q_OBJECT
protected:
    /* composant racine selectionne */
    SwComponent_Class * _selected_root_element;
    /* composant modele host selectionne */
    _SwModelHost_Class * _selected_model_host;
    /* List des interfaces d'execution */
    QList<SwComponent_Class *> _roots_list;
    /* Arbre de visualisation */
    QTreeView * _view;
    /* modele courant */
    _SwModelStreamTreeModel * _model;
public:
    /*! \brief Constructeur */
    _SwModelHostSelector(QWidget *parent,SwComponent_Class * selected_root_element,_SwModelHost_Class * selected_model_host);
    /*! \brief Destructeur */
    ~_SwModelHostSelector();

    /*! \brief recuperation du composant racine*/
    SwComponent_Class * GetRootComponent();

    /*! \brief recuperation du model host*/
    _SwModelHost_Class * GetModelHost();
    

private slots:
    /*! \brief Changement combo*/
    void OnComboChange(int index);
    /*! \brief Selection modele hote*/
    void OnTreeViewClicked (const QModelIndex & index);
    /*! \brief sur unbing */
    void OnUnBind();
};

#endif
