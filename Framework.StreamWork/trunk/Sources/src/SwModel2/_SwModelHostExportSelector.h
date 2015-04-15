/*!
\file _SwModelHostExportSelector.h
\date 12/04/2006
\brief Ihm de selection des entites exportées par le model host
\author  Big
\version 1.0
 */

#ifndef __SwModelHostExportSelector_H
#define __SwModelHostExportSelector_H
/*
  * INCLUDES GLOBAUX
  */

#include <QDialog>
#include <QList>
#include <QTreeView>
/*
  * INCLUDES LOCAUX
  */

#include <SwComponent_Class.h>
#include "_SwModelExportedEntity.h"
#include "_SwModelExportedEntitiesModel.h"

using namespace StreamWork::SwCore;

/*!
	\class _SwModelHostExportSelector 
	\brief  Ihm de selection des entites exportées par le model host
*/
class _SwModelHostExportSelector : public QDialog
{
    Q_OBJECT
protected:
    /* composant racine selectionne */
    SwComponent_Class * _root_comp;
    /* liste des entités */
    QList<_SwModelExportedEntity *> * _entities_list;
    /* model pour l'edition */
    _SwModelExportedEntitiesModel * _entities_model;

public:
    /*! \brief Constructeur */
    _SwModelHostExportSelector(QWidget *parent,SwComponent_Class * root_element,QList<_SwModelExportedEntity *> * entities_list);
    /*! \brief Destructeur */
    ~_SwModelHostExportSelector();
    /*! \brief validation des changement */
    void ValidChange();

 };

#endif
