/*!
 \file _SwGuiCompLabel.cpp
 \brief Implementation of the Class _SwGuiCompLabel generant un QWidget
 \version 1.0
 \date 26-octobre-2009 18:59:26
 \author PGA
*/

#ifndef __SwGuiCompLabel_H
#define __SwGuiCompLabel_H

/*
  * INCLUDES GLOBAUX
  */
#include <QLabel>
/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Consumer_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Properties/ISwProperty.h"

using namespace StreamWork::SwCore;

/*!
    \class _SwGuiCompLabel
    \brief _SwGuiCompLabel generant un QWidget
*/
class _SwGuiCompLabel : public SwComponent_Class,
    public ISwInterfaces_ConsumerObserver
{
protected:
    /* menu */
    QLabel * _label;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    
public:
    /*! \brief Constructeur */
    _SwGuiCompLabel();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompLabel();
    
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
