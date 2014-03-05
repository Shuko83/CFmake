/*!
 \file _SwGuiCompMenu.h
 \brief Implementation of the Class _SwGuiCompMenu generant un QMenu
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompMenu_H
#define __SwGuiCompMenu_H

/*
  * INCLUDES GLOBAUX
  */
/*
  * INCLUDES LOCAUX
  */
#include <QMap>
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include "ISwMenu.h"
#include "ISwAction.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
	\class _SwGuiCompMenu 
	\brief _SwGuiCompMenu generant un QMenu
*/
class _SwGuiCompMenu : public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public ISwMenu
{
protected:
    /* menu */
    QMenu * _menu;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* nombre de menus */
    uint _menus_nb;
    /* propriété nombre de menu */
    ISwProperty * _menus_nb_property;
    /* map des interfaces menus*/
    QMap<QString,ISwMenu *> _menus;
    /* handle temporaire d'interface menus*/
    ISwMenu * _tmp_handle_menu;
    /* nombre d'actions */
    uint _actions_nb;
    /* propriété nombre d'actions*/
    ISwProperty * _actions_nb_property;
    /* map des interfaces actions*/
    QMap<QString,ISwAction *> _actions;
    /* handle temporaire d'interface actions*/
    ISwAction * _tmp_handle_action;
public:
    /*! \brief Constructeur */
    _SwGuiCompMenu();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompMenu();

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
    // Interface ISwQMainWindow
    //---------------------------------------------------------------------
    /*! \brief Renvoie le menu
    \return le menu */
	virtual QMenu & GetMenu();

};
#endif 
