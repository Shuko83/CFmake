/*!
 \file _SwGuiCompScrollArea.h
 \brief Implementation of the Class _SwGuiCompScrollArea generant un QScrollArea
 \version 1.0
 \date 08/08/13
 \author EPO
*/

#ifndef __SwGuiCompScrollArea_H
#define __SwGuiCompScrollArea_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QScrollArea>
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
    \class _SwGuiCompScrollArea
    \brief _SwGuiCompScrollArea generant un QScrollArea
*/
class _SwGuiCompScrollArea : public SwComponent_Class, public ISwInterfaces_ConsumerObserver
{
    Q_OBJECT
    Q_PROPERTY( bool fixedWidth READ GetFixedWidth WRITE SetFixedWidth )
    Q_PROPERTY( bool fixedHeight READ GetFixedHeight WRITE SetFixedHeight )
    
protected:
    /* menu */
    QScrollArea * _scrollArea;
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
    
    bool _fixedWidth;
    bool _fixedHeight;
    
public:
    /*! \brief Constructeur */
    _SwGuiCompScrollArea();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompScrollArea();
    
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
    
    bool GetFixedWidth() const { return _fixedWidth; }
    void SetFixedWidth( bool val ) { _fixedWidth = val; }
    
    bool GetFixedHeight() const { return _fixedHeight; }
    void SetFixedHeight( bool val ) { _fixedHeight = val; }
    bool eventFilter( QObject * o, QEvent * e );
};

#endif
