/*!
 \file _SwGuiCompVBoxLayout.h
 \brief Implementation of the Class _SwGuiCompVBoxLayout generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompVBoxLayout_H
#define __SwGuiCompVBoxLayout_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QVBoxLayout>
#include <QSpacerItem>
 /*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Consumer_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Properties/ISwProperty.h"

#include <QDebug>
using namespace StreamWork::SwCore;

/*!
	\class _SwGuiCompVBoxLayout 
	\brief _SwGuiCompVBoxLayout generant un QWidget
*/
class _SwGuiCompVBoxLayout : public SwComponent_Class, public ISwInterfaces_ConsumerObserver
{
    Q_OBJECT

    Q_PROPERTY(bool Spacer_enable READ getEnableSpacer WRITE setEnableSpacer)

protected:
    /* menu */
	QWidget * _layoutWidget;
	QVBoxLayout * _layout;
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
    QMap<QString, QWidget *> _widgets;
    /* handle temporaire d'interface widget*/
	QWidget * _tmp_handle_widget;

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
    _SwGuiCompVBoxLayout();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompVBoxLayout();

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
};

#endif 
