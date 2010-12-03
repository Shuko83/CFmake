/*!
 \file _SwGuiCompHBoxLayout.h
 \brief Implementation of the Class _SwGuiCompHBoxLayout generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompHBoxLayout_H
#define __SwGuiCompHBoxLayout_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QHBoxLayout>
#include <QSpacerItem>
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
	\class _SwGuiCompHBoxLayout 
	\brief _SwGuiCompHBoxLayout generant un QWidget
*/
class _SwGuiCompHBoxLayout : public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public ISwLayout
{
    Q_OBJECT

    Q_PROPERTY(bool Spacer_enable READ getEnableSpacer WRITE setEnableSpacer)
protected:
    /* menu */
    QHBoxLayout * _layout;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    //Layout properties
    //margin
    int _margin;
    ISwProperty * _margin_property;
    //spacing
    int _spacing;
    ISwProperty * _spacing_property;
    // --- Widgets ---
    /* nombre de widgets */
    uint _widgets_nb;
    /* propriété nombre de widgets */
    ISwProperty * _widgets_nb_property;
    /* map des interfaces widgets*/
    QMap<QString,ISwWidget *> _widgets;
    /* handle temporaire d'interface widget*/
    ISwWidget * _tmp_handle_widget;
    // --- Layout ---
    /* nombre de widgets */
    uint _layouts_nb;
    /* propriété nombre de widgets */
    ISwProperty * _layouts_nb_property;
    /* handle temporaire d'interface widget*/
    ISwLayout * _tmp_handle_layout;
    /* map des interfaces widgets*/
    QMap<QString,ISwLayout *> _layouts;
    //Spacer
    bool _enableSpacer;
    QSpacerItem * _spacer;
    // --- Commun ---
    //Ordered building list
    QList<QString> _ordered_childrens;
    //-- compteur d'objet enfants
    int _nb_childs;
public:
    /*! \brief Constructeur */
    _SwGuiCompHBoxLayout();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompHBoxLayout();

    /*! \brief Initialisation des ressources
    \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
     /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    /*! \brief getter*/
    bool getEnableSpacer();
    /*! \brief setter*/
    void setEnableSpacer(bool enable);
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
	/*! \brief Avant changement de la disponibilité de l'interface */
	virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
	/*! \brief Apres changement de la disponibilité de l'interface */
	virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
    //---------------------------------------------------------------------
    // Interface ISwLayout
    //---------------------------------------------------------------------
    /*! \brief Renvoie le Layout
            \return le Layout */
	virtual QLayout & GetLayout();
    /*! \brief Liberation du layout */
	virtual void LiberateLayout();

};
#endif 
