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
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include "ISwLayout.h"
#include "ISwWidget.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
	\class _SwGuiCompFrame 
	\brief _SwGuiCompFrame generant un QWidget
*/
class _SwGuiCompFrame : public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public ISwWidget
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
    QMap<QString,ISwWidget *> _widgets;
    /* handle temporaire d'interface widget*/
    ISwWidget * _tmp_handle_widget;
    /* integer indiquant le nombre de widgets en gestion directe */
    uint _registered_widgets_nb;
    // --- Layout ---
    /* interface layout a consommée */
    ISwLayout * _handle_layout;
    /* Flag indiquant une gestion par un layout */
    bool _is_layout_mode;

public:
    /*! \brief Constructeur */
    _SwGuiCompFrame();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompFrame();

    /*! \brief Initialisation des ressources
    \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
     /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
	/*! \brief Avant changement de la disponibilité de l'interface */
	virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
	/*! \brief Apres changement de la disponibilité de l'interface */
	virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
    //---------------------------------------------------------------------
    // Interface ISwWidget
    //---------------------------------------------------------------------
    /*! \brief Renvoie le widget
    \return le widget */
	virtual QWidget & GetWidget();

};
#endif 
