/*!
 \file _SwGuiCompDockWidget.h
 \brief Implementation of the Class _SwGuiCompDockWidget generant un SwDockWidget
 \version 1.0
 \date
 \author
*/

#ifndef __SwGuiCompSwDockWidget_H
#define __SwGuiCompSwDockWidget_H

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
	\brief _SwGuiCompDockWidget generant un SwDockWidget
*/
class _SwGuiCompDockWidget : public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public ISwDockWidget
{
	Q_OBJECT
	//Q_PROPERTY(bool forceFloating READ GetForceFloating WRITE SetForceFloating) //ISwQDockWidget
	//Q_PROPERTY (bool showTitleBar READ getShowTitleBar	WRITE setShowTitleBar);

protected:
    /* menu */
    SwDockWidget_DockWidget * _dockwidget;
    /* service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* interface widget a consommée */
    ISwWidget * _handle_widget;
	//bool _forceFloating; //ISwQDockWidget
	
	/*Properties*/
	/*bool _showTitleBar;
	QWidget* _titleBar;*/

public:
    /*! \brief Constructeur */
    _SwGuiCompDockWidget();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompDockWidget();	

    /*! \brief Initialisation des ressources
    \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
    //-------------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //-------------------------------------------------------------------------
	/*! \brief Avant changement de la disponibilité de l'interface */
	virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
	/*! \brief Apres changement de la disponibilité de l'interface */
	virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
    //-------------------------------------------------------------------------
    // Interface ISwDockWidget
    //-------------------------------------------------------------------------
    /*! \brief Renvoie le dockwidget
    \return le dockwidget */
	virtual SwDockWidget_DockWidget & GetDockWidget();

	/*virtual bool GetForceFloating() const { return _forceFloating; }
	virtual void SetForceFloating(bool val) { _forceFloating = val; }*/

	//----------------------------------------------------------------------------
	/*Properties*/

	/*bool getShowTitleBar();
	void setShowTitleBar(bool val);*/
};
#endif 
