/*!
 \file _SwPerspectiveList_H.h
 \brief Implementation of the Class _SwPerspectiveList realisant la fonction d'executeur
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwPerspectiveList_H
#define __SwPerspectiveList_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
/*
  * INCLUDES LOCAUX
  */
#include <SwProperties_Class.h>
#include <SwPins_Manager_Class.h>
#include <ISwPerspective.h>
#include <ISwProperty.h>
#include <SwInterfaces_Consumer_Class.h>


using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;
using namespace StreamWork::SwConfiguration;  

/*!
	\class _SwPerspectiveList 
	\brief Implementation of the Class _SwPerspectiveList generant un router de données basique
*/
class _SwPerspectiveList : 
    public ISwInterfaces_ConsumerObserver

{
    Q_OBJECT

public:
    /*! \brief Constructeur */
    _SwPerspectiveList(
                SwProperties_Class * propertiesService,  
                SwInterfaces_Consumer_Class * _consumer_service);
    
    /*! \brief Destructeur */
    virtual ~_SwPerspectiveList();
    
    /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
    
    /*! \brief Avant changement de la disponibilité de l'interface */
    virtual void BeforeInterfaceAvailabilityChange(
                                QString interface_name,
                                SwComponent_Class * provider_host);
    
    /*! \brief Apres changement de la disponibilité de l'interface */
    virtual void AfterInterfaceAvailabilityChange(
                                QString interface_name,
                                SwComponent_Class * provider_host);



protected:

    /* service de gestion des propriétés */
    SwProperties_Class *         _properties_service;

    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;

    /* nombre de perspectives */
    uint _perspectives_nb;
    
    /* propriété nombre de perspectives */
    ISwProperty * _perspectives_nb_property;
    
    /* map des perspectives widgets*/
    QMap<QString,ISwPerspective *> _perspectives;
    
    /* handle temporaire d'interface perspectives*/
    ISwPerspective * _tmp_handle_perspective;
    
    /* handle PerspectiveCourante*/
    ISwPerspective * _currentPerspective;
    
public slots:
    
	/*! \brief sur fin*/
    void onQuit();
    
};
#endif 
