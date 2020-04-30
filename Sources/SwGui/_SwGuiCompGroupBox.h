/*!
 \file _SwGuiCompGroupBox.cpp
 \brief Implementation of the Class _SwGuiCompGroupBox generant un QWidget
 \version 1.0
 \date 26-octobre-2009 18:59:26
 \author PGA
*/

#ifndef __SwGuiCompGroupBox_H
#define __SwGuiCompGroupBox_H

/*
  * INCLUDES GLOBAUX
  */
#include <QGroupBox>
#include <QVBoxLayout>
/*
  * INCLUDES LOCAUX
  */
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>

using namespace StreamWork::SwCore;

/*!
    \class _SwGuiCompGroupBox
    \brief _SwGuiCompGroupBox generant un QWidget
*/
class _SwGuiCompGroupBox : public SwComponent_Class,
    public ISwInterfaces_ConsumerObserver
{
protected:
    /* menu */
    QGroupBox * _groupBox;
	QVBoxLayout * _layout;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* interface layout a consommer */
    QWidget * _mainWidget;
    
public:
    /*! \brief Constructeur */
    _SwGuiCompGroupBox();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompGroupBox();
    
    /*! \brief Initialisation des ressources
    \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw( SwException );
    /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange( ISwProperty * property );
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
    /*! \brief Avant changement de la disponibilité de l'interface */
    virtual void BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );
    /*! \brief Apres changement de la disponibilité de l'interface */
    virtual void AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );
    
};

#endif
