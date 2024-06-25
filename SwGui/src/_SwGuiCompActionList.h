/*!
 \file _SwGuiCompActionList.h
 \brief Implementation of the Class _SwGuiCompActionList generant un QMenu
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompActionList_H
#define __SwGuiCompActionList_H

/*
  * INCLUDES GLOBAUX
  */
/*
  * INCLUDES LOCAUX
  */
#include <QMap>
#include "Component/Base/SwComponent_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Consumer_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Properties/ISwProperty.h"
#include "ISwActionList.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
	\class _SwGuiCompActionList 
	\brief _SwGuiCompActionList generant un QMenu
*/
class _SwGuiCompActionList : public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public ISwActionList
{
public:
    /*! \brief Constructeur */
    _SwGuiCompActionList();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompActionList();

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
    // Interface ISwActionList
    //---------------------------------------------------------------------
    /*! \brief Renvoie le menu
    \return le menu */
	virtual QList<QAction * > & GetActions();
			
	virtual void addActionListListener(ISwActionListListener * observer);  
	
	virtual void removeActionListListener(ISwActionListListener * observer);

protected:

    void signalListChanged();
	
    /* menu */
    QMap<int, QAction *> _actionMap;
	QList<QAction *> _actionList;
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
    QMap<QString, QAction *> _actions;
    /* handle temporaire d'interface actions*/
    QAction * _tmp_handle_action;	


    QList<ISwActionListListener *>   _listeners;
};
#endif 
