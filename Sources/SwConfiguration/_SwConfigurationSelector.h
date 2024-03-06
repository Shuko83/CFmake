/*!
\file _SwModelHostExportSelector.h
\date 12/04/2006
\brief Ihm de selection des entites exportées par le model host
\author  Big
\version 1.0
 */

#ifndef __SwConfigurationSelector_H
#define __SwConfigurationSelector_H
/*
  * INCLUDES GLOBAUX
  */

#include <QDialog>
#include <QList>
#include <QTreeView>
#include "SwConfigurationConstantes.h"
/*
  * INCLUDES LOCAUX
  */

#include "Component/Base/SwComponent_Class.h"
#include "_SwConfigurationExportedEntity.h"


using namespace StreamWork::SwCore;

class _SwConfigurationEntitiesModel;

/*!
	\class _SwConfigurationExportSelector 
	\brief  Ihm de selection des entites exportées par la configuration
*/
class SWCONFIGURATION_EXPORT _SwConfigurationSelector : public QDialog
{
    Q_OBJECT
protected:
    /* composant racine selectionne */
    SwComponent_Class * _root_comp;
    /* liste des entités */
    QList<_SwConfigurationExportedEntity *> * _entities_list;
    /* model pour l'edition */
    _SwConfigurationEntitiesModel * _entities_model;

public:
    /*! \brief Constructeur */
    _SwConfigurationSelector(
          QWidget *parent,
          SwComponent_Class * root_element,
          QList<_SwConfigurationExportedEntity *> * entities_list,
		  bool isLimitedToProperty=false);
          
    /*! \brief Destructeur */
    ~_SwConfigurationSelector();

    /*! \brief validation des changement */
    void ValidChange();
 };

#endif
