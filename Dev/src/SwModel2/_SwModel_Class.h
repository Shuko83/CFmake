/*!
 \file _SwModel_Class.h
 \brief Implementation d'un model
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwModel_Class_H
#define __SwModel_Class_H

/*
  * INCLUDES GLOBAUX
  */
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
#include <ISwVisitor.h>
#include <ISwActivable.h>

class _SwModelHost_Class;

using namespace StreamWork::SwCore;

/*!
	\class _SwModel_Class 
	\brief Implementation d'un model
*/
class _SwModel_Class : public SwComponent_Class, public ISwAdminSetup, public ISwVisitor
{
protected:
    /*! \brief Nom du modele */
    QString _model_name;
    /*! \brief element racine du stream */
    SwComponent_ClassPtr _root_element;
    /*! \brief Model host path */
    QString _ipath;
     /*! \brief Model host entry */   
    _SwModelHost_Class * _host_entry;
    /*! \brief IsDefault (construit avec le constructeur sans paramètre) */
    bool _isDefault;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* service de gestion des pins */
    SwPins_Manager_Class * _pins_service;
    /* activation */
    ISwActivable * _activationDelegate;
public:
    /*! \brief Constructeur */
    _SwModel_Class();
    /*! \brief Constructeur */
    _SwModel_Class(QString model_name,SwComponent_Class * root_element,QString ipath);
    /*! \brief Destructeur */
    virtual ~_SwModel_Class();
    /*! \brief Initialisation des ressources
          \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
	/*! \brief Permet de definir le nom de l'instance nomme*/
	virtual void SetName(const QString & new_name) throw(SwException);
	/*! \brief La racine du parent racine est mise a jour*/
	virtual void RootParentNameHasChanged();
    //----------------------------------------------------
    // Interface ISwAdminSetup
    //----------------------------------------------------
	/*! \brief Permet d'acceder aux preferences d'administration*/
	void AdminSetup();
    //----------------------------------------------------
    // ISwVisitor
    //----------------------------------------------------
    /*! \brief methode de visite */
    virtual void Visit(SwComponent_Class * visited);
    //----------------------------------------------------
    // Gestion liaison entre model et model_host
    //----------------------------------------------------
	/*! \brief Mise en place du pont entre le model et le model host*/
    void CreateBinding();
	/*! \brief Suppression du pont entre le model et le model host*/
    void DestroyBinding();
    //----------------------------------------------------
    // ISwActivation
    //----------------------------------------------------
    virtual bool isActive();
    virtual void setActive(bool value);
    //----------------------------------------------------
    // delegue l activation
    //----------------------------------------------------    
    void setActivationDelegate (ISwActivable *  activable);
};
#endif 
