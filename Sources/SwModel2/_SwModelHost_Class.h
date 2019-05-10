/*!
 \file _SwModelHost_Class.h
 \brief Implementation d'un hote de model
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwModelHost_Class_H
#define __SwModelHost_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QList>
#include <QMap>
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
#include "_SwModelExportedEntity.h"
#include "ISwModelHostModifier.h"
//Nom du service
#define CG_SW_SERVICE_MODELHOST "MODELHOST"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwModel;

class TargetInterface {
public:
    QString provider;
    QString name;
    TargetInterface(QString provider,QString name) {
        this->provider=provider;
        this->name=name;
    }
};

/*!
	\class _SwModelHost_Class 
	\brief Implementation d'un hote de model
*/
class _SwModelHost_Class : public SwComponent_Class, public ISwAdminSetup, public ISwService, public ISwPersistent, public ISwModelHostModifier
{
protected:
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* service de gestion des pins */
    SwPins_Manager_Class * _pins_service;     
    /* Liste des entites exportes */
    QList<_SwModelExportedEntity *> _exported_entities;
    /* Modele lié */
    SwComponent_Class * _model;

    /* Last Providers */
    QMap<QString,TargetInterface *> _lastProviders;
    /* Last Providers */
    QMap<QString,QList<TargetInterface *>> _lastConsumers;
public:
    /*! \brief Constructeur */
    _SwModelHost_Class();
    /*! \brief Destructeur */
    virtual ~_SwModelHost_Class();
    /*! \brief Initialisation des ressources
          \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
    //----------------------------------------------------
    // Interface ISwAdminSetup
    //----------------------------------------------------
	/*! \brief Permet d'acceder aux preferences d'administration*/
	void AdminSetup();
    //----------------------------------------------------
    // Gestion liaison entre model et model_host
    //----------------------------------------------------
	/*! \brief Mise en place du pont entre le model et le model host*/
    void CreateBinding(SwComponent_Class * model);
	/*! \brief Suppression du pont entre le model et le model host*/
    void DestroyBinding();
	/*! \brief Creation reelle des entités*/
    void BuildEntities();
	/*! \brief Suppression reelle des entites*/
    void DestroyEntities();
    //---------------------------------------------------------------------
    // Interface ISwService
    //---------------------------------------------------------------------
    /*! \brief Renvoie le nom du service
    \return le nom du service */
    QString GetServiceName(){return QString(CG_SW_SERVICE_MODELHOST); }
    /*! \brief Renvoie le nom du service reel (le nom de l'interface)
    \return le nom du service reel (le nom de l'interface)
    \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
    QString GetServiceRealName() {return QString(typeid(*this).name());}    
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
	void Save(QXmlStreamWriter& writer);
    //---------------------------------------------------------------------
    // Interface ISwModelHostModifier
    //---------------------------------------------------------------------
    /*! \brief Add provider interface*/
    virtual QString addProviderInterface(QString name,SwComponent_Class * component,QString itype);     
    /*! \brief Add consumer interface*/
    virtual QString addConsumerInterface(QString name,SwComponent_Class * component,QString itype);       
    /*! \brief Add connector*/
    virtual QString addConnector(QString name,SwComponent_Class * component,QString itype);      
    /*! \brief update model host*/
    virtual void updateModelHost();     
};
#endif 
