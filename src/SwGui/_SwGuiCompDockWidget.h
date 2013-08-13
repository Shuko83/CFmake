/*!
 \file _SwGuiCompDockWidget.h
 \brief Implementation of the Class _SwGuiCompDockWidget generant un QDockWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompDockWidget_H
#define __SwGuiCompDockWidget_H

/*
  * INCLUDES GLOBAUX
  */
/*
  * INCLUDES LOCAUX
  */
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include "ISwWidget.h"
#include "ISwLayout.h"
#include "ISwDockWidget.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
	\class _SwGuiCompDockWidget 
	\brief _SwGuiCompDockWidget generant un QDockWidget
*/
class _SwGuiCompDockWidget : public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public ISwDockWidget
{
	Q_OBJECT
	Q_PROPERTY(bool forceFloating READ GetForceFloating WRITE SetForceFloating)

protected:
    /* menu */
    QDockWidget * _dockwidget;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* interface widget a consommée */
    ISwWidget * _handle_widget;
	/* Force floating because in Qt, the floating is true until the dock is parent */
	bool _forceFloating;

public:
    /*! \brief Constructeur */
    _SwGuiCompDockWidget();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompDockWidget();

    /*! \brief Initialisation des ressources
    \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
	/*! \brief Avant changement de la disponibilité de l'interface */
	virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
	/*! \brief Apres changement de la disponibilité de l'interface */
	virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
    //---------------------------------------------------------------------
    // Interface ISwDockWidget
    //---------------------------------------------------------------------
    /*! \brief Renvoie le dockwidget
    \return le dockwidget */
	virtual QDockWidget & GetDockWidget();


	virtual bool GetForceFloating() const { return _forceFloating; }
	virtual void SetForceFloating(bool val) { _forceFloating = val; }
};
#endif 
