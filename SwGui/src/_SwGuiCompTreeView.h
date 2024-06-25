/*!
 \file _SwGuiCompTreeView.h
 \brief Implementation of the Class _SwGuiCompTreeView generant un QTreeView pour un modele fournit
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompTreeView_H
#define __SwGuiCompTreeView_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QTreeView>
/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Consumer_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Properties/ISwProperty.h"
#include "ISwQAbstractItemViewSlots.h"
#include "_SwQAbstractItemViewSlots_Catcher.h"
#include <QAbstractItemModel>
#include <QStandardItemModel>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
    \class _SwGuiCompTreeView
    \brief Implementation of the Class _SwGuiCompTreeView generant un QTreeView pour un modele fournit
*/
class _SwGuiCompTreeView : public SwComponent_Class, public ISwInterfaces_ConsumerObserver
{
protected:
    /* TreeView */
    QTreeView * _treeview;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /*handle model */
    QAbstractItemModel * _handle;
    /*default model */
    QStandardItemModel * _default_model;
    /*slots handle*/
    ISwQAbstractItemViewSlots * _slots_handles;
    /*signals catcher*/
    _SwQAbstractItemViewSlots_Catcher * _signals_catcher;
public:
    /*! \brief Constructeur */
    _SwGuiCompTreeView();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompTreeView();
    
    /*! \brief Initialisation des ressources
          \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw( SwException );
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
    /*! \brief Avant changement de la disponibilité de l'interface */
    virtual void BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );
    /*! \brief Apres changement de la disponibilité de l'interface */
    virtual void AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );

};

#endif
