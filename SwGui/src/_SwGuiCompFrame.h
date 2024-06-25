/*!
 \file _SwGuiCompFrame.h
 \brief Implementation of the Class _SwGuiCompFrame generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompFrame_H
#define __SwGuiCompFrame_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QFrame>
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
    \class _SwGuiCompFrame
    \brief _SwGuiCompFrame generant un QWidget
*/
class _SwGuiCompFrame : public SwComponent_Class, public ISwInterfaces_ConsumerObserver
{
protected:
    /* menu */
    QFrame * _frame;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    // --- Widgets ---
    /* nombre de widgets */
    uint _widgets_nb;
    /* propriété nombre de widgets */
    ISwProperty * _widgets_nb_property;
    /* map des interfaces widgets*/
    QMap<QString, QWidget *> _widgets;
    /* handle temporaire d'interface widget*/
	QWidget * _tmp_handle_widget;
    /* integer indiquant le nombre de widgets en gestion directe */
    uint _registered_widgets_nb;
    
public:
    /*! \brief Constructeur */
    _SwGuiCompFrame();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompFrame();
    
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
