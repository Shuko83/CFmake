/*!
 \file _SwGuiCompSplitterWidget.h
 \brief Implementation of the Class _SwGuiCompSplittableWidget generant un QSplitter
 \version 1.0
 \date 14-mai-2009
 \author PGA
*/

#ifndef __SwGuiCompSplitterWidget_H
#define __SwGuiCompSplitterWidget_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QPair>
#include <QSplitter>

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
    \class _SwGuiCompSplitterWidget
    \brief _SwGuiCompSplitterWidget generant un QSplitter
*/
class _SwGuiCompSplitterWidget
    : public SwComponent_Class,
      public ISwInterfaces_ConsumerObserver
{
protected:
    /* main */
    QSplitter * _splitterWidget;
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
    /* map des proprietes des tailles des widgets */
    typedef QMap<QString, ISwProperty *> WidgetSizes;
    WidgetSizes _widgetSizes;
    /* handle temporaire d'interface widget*/
	QWidget * _tmp_handle_widget;
    
    /*! \brief reset de la taille des widgets  */
    void resetWidgetSizes();
    
public:
    /*! \brief Constructeur */
    _SwGuiCompSplitterWidget();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompSplitterWidget();
    
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
