/*!
 \file _SwGuiCompTabWidget.h
 \brief Implementation of the Class _SwGuiCompTabWidget generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompTabWidget_H
#define __SwGuiCompTabWidget_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QTabWidget>
/*
  * INCLUDES LOCAUX
  */
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include "ISwLayout.h"
#include "ISwWidget2.h"
#include "ISwWidget2_Observer.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
    \class _SwGuiCompTabWidget
    \brief _SwGuiCompTabWidget generant un QWidget
*/
class _SwGuiCompTabWidget : public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public ISwWidget2, public ISwWidget2_Observer
{
protected:
    /* menu */
    QTabWidget * _tabWidget;
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
    QMap<QString, ISwWidget *> _widgets;
    /* handle temporaire d'interface widget*/
    ISwWidget * _tmp_handle_widget;
    /* liste des observers */
    QList<ISwWidget2_Observer *> _wObservers;
public:
    /*! \brief Constructeur */
    _SwGuiCompTabWidget();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompTabWidget();
    
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
    //---------------------------------------------------------------------
    // Interface ISwWidget2
    //---------------------------------------------------------------------
    /*! \brief Renvoie le widget
    \return le widget */
    virtual QWidget * GetWidget();
    /** @brief Enregistrement de l'observer */
    virtual void RegisterISwWidgetObserver( ISwWidget2_Observer * o );
    /** @brief Desregistrement de l'observer */
    virtual void UnregisterISwWidgetObserver( ISwWidget2_Observer * o );
    //---------------------------------------------------------------------
    // InterfaceISwWidget2_Observer
    //---------------------------------------------------------------------
    /** @brief mettre en avant le widget si c'est possible */
    virtual void OnBringToFrontRequest( ISwWidget * w );
    
};
#endif
