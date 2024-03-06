/*!
 \file _SwGuiCompPushButtonToQAction.cpp
 \brief Implementation of the Class _SwGuiCompPushButtonToQAction generant un QWidget
 \version 1.0
 \date 21-novembre-2011
 \author ATN
*/

#ifndef __SwGuiCompPushButtonToQAction_H
#define __SwGuiCompPushButtonToQAction_H

/*
  * INCLUDES GLOBAUX
  */
#include <QPushButton>

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
    \class _SwGuiCompPushButtonToQAction
    \brief _SwGuiCompPushButtonToQAction generant un QWidget
*/
class _SwGuiCompPushButtonToQAction : public SwComponent_Class,
    public ISwInterfaces_ConsumerObserver
{
    Q_OBJECT
protected:
    /* menu */
    QPushButton * pushButton;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    
    /* interface action a consommée */
    QAction * _handle_action;
    
    /** @brief Interface Widget*/
    //QWidget * _i_Widget;
public:
    /*! \brief Constructeur */
    _SwGuiCompPushButtonToQAction();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompPushButtonToQAction();
    
    /*! \brief Initialisation des ressources
    \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw( SwException );
    /*! \brief Callback sur les changements de propriétés*/
    //void OnPropertyChange(ISwProperty * property);
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
    //*! \brief Avant changement de la disponibilité de l'interface */
    virtual void BeforeInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );
    /*! \brief Apres changement de la disponibilité de l'interface */
    virtual void AfterInterfaceAvailabilityChange( QString interface_name, SwComponent_Class * provider_host );
    
    //--------------------------------------------------------------
    //Slots
    //--------------------------------------------------------------
public slots :
    void commandApply( bool state );
    
};

#endif
