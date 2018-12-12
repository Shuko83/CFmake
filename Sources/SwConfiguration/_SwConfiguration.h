/*!
 \file _SwConfiguration.h
 \brief Implementation d'un hote de model
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwConfiguration_H
#define __SwConfiguration_H

/*
  * INCLUDES GLOBAUX
  */
#include <QList>
/*
  * INCLUDES LOCAUX
  */

#include <SwComponent_Class.h>
#include <ISwAdminSetup.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include <ISwInterfaces_ConsumerObserver.h>
#include <ISwPin_Listener.h>
#include <SwPins_Manager_Class.h>
#include <ISwService.h>
#include <ISwPersistent.h>
#include "_SwConfigurationExportedEntity.h"
#include "ISwConfiguration.h"
#include "ISwConfigurationListener.h"
#include <QList>
#include <_SwConfigurationExportedPerspective.h>
#include <ISwServiceOwnerConfigurable.h>

//Nom du service
#define CG_SW_SERVICE_MODELHOST "MODELHOST"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwConfiguration;

/*!
	\class _SwConfiguration 
	\brief Implementation d'un hote de model
*/
class _SwConfiguration : 
	public SwComponent_Class, 
	public ISwAdminSetup, 
	public ISwService, 
	public ISwPersistent, 
	public ISwFinalizer,
	public ISwConfiguration
{


        Q_OBJECT

        /*Q_PROPERTY( bool IsRecordAvailable
                    READ isRecordAvailable
                    WRITE setRecordAvailable)   */

public:
    /*! \brief Constructeur */
    _SwConfiguration();
    /*! \brief Destructeur */
    virtual ~_SwConfiguration();
    /*! \brief Initialisation des ressources
          \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
    //----------------------------------------------------
    // Interface ISwAdminSetup
    //----------------------------------------------------
	/*! \brief Permet d'acceder aux preferences d'administration*/
	virtual void AdminSetup();
    //----------------------------------------------------
    // Gestion liaison entre model et model_host
    //----------------------------------------------------
	/*! \brief Mise en place du pont entre le model et le model host*/
    void CreateBinding(SwComponent_Class * model);
	/*! \brief Suppression du pont entre le model et le model host*/
    void DestroyBinding();
    //---------------------------------------------------------------------
    // Interface ISwService
    //---------------------------------------------------------------------
    /*! \brief Renvoie le nom du service
    \return le nom du service */
    QString GetServiceName(){return QString(CG_SW_SERVICE_MODELHOST); }
    /*! \brief Renvoie le nom du service reel (le nom de l'interface)
    \return le nom du service reel (le nom de l'interface)
    \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()*/
    QString GetServiceRealName() {return QString(typeid(this).name());}    
    /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
    lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
    void Liberate();  
    //---------------------------------------------------------------------
    // Interface ISwPersistence
    //---------------------------------------------------------------------
	/*! \brief methode permettant de charger des donnees
    \param[in] elt neoud parent
    \param[in] finalizer_manager manager de finalisation
    */
	void Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager);
	/*! \brief methode permettant de sauver des donnees
    \param[in] elt neoud parent
    \param[in] doc document parent
    */
	void Save(QXmlStreamWriter &);

    //---------------------------------------------------------------------
    // Interface ISwFinalizer
    //---------------------------------------------------------------------
    /*! \brief finalize l'operation correspondant a l'index d'historique
    \param[in] historic_index index d'historique
    \return false si la finalisation n'a pas eu lieu et true si ok*/
    virtual bool Finalize(quint64 historic_index);
    //---------------------------------------------------------------------
    // Interface ISwConfiguration
    //---------------------------------------------------------------------
	virtual void setActivated(bool value);

	virtual bool isActivated(); 

    //virtual bool isRecordAvailable(); 

    //virtual void setRecordAvailable(bool value);

	virtual ISwProperties * getProperties();

    /** @brief ajout d un listener */
    virtual void addListener(ISwConfigurationListener *) ;

    /** @brief suppression d un listener */
    virtual void removeListener(ISwConfigurationListener *) ;

	virtual  QList<StreamWork::SwCore::ISwServiceOwnerConfigurable *> getServiceOwnerConfigurable();
	    
	virtual StreamWork::SwExecution::ISwExecution_Service * getExecutionService();
protected:

	void setPerspectiveActivation(
			_SwConfigurationExportedPerspective * perspective,
			bool value);


    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* service de gestion des pins */
    SwPins_Manager_Class * _pins_service;     
    /* Liste des entites exportes */
    QList<_SwConfigurationExportedEntity *> _exported_entities; 
	/* Modele lié */
    SwComponent_Class * _model;

    quint64 h_index;

	void signalActivated();
	void signalDisabled();
	void signalPropertiesChanged();
  //void signalRecordAvailableChanged();

	QList<ISwConfigurationListener *>	_configurationListeners;
	bool							    _activated;
  //  bool                                _recordAvailable;
};
#endif 
