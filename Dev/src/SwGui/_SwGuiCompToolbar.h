/*!
 \file _SwGuiCompToolBar.h
 \brief Implementation of the Class _SwGuiCompToolBar generant un QToolBar
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompToolBar_H
#define __SwGuiCompToolBar_H

/*
  * INCLUDES GLOBAUX
  */
/*
  * INCLUDES LOCAUX
  */
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include "ISwToolBar.h"
#include "ISwAction.h"
#include "ISwWidget.h"



using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
	\class _SwGuiCompToolBar 
	\brief _SwGuiCompToolBar generant une QToolBar
*/
class _SwGuiCompToolBar : public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public ISwToolBar
{
protected:
    /* menu */
    QToolBar * _toolbar;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* nombre d'actions */
    uint _actions_nb;
    /* propriété nombre d'actions*/
    ISwProperty * _actions_nb_property;
    /* map des interfaces actions*/
    QMap<QString,ISwAction *> _actions;
    /* handle temporaire d'interface action*/
    ISwAction * _tmp_handle_action;
public:
    /*! \brief Constructeur */
    _SwGuiCompToolBar();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompToolBar();

    /*! \brief Initialisation des ressources
    \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
     /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
	/*! \brief Avant changement de la disponibilité de l'interface */
	virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
	/*! \brief Apres changement de la disponibilité de l'interface */
	virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
    //---------------------------------------------------------------------
    // Interface ISwMainWindow
    //---------------------------------------------------------------------
    /*! \brief Renvoie le menu
    \return le menu */
	virtual QToolBar & GetToolBar();

};
#endif 
