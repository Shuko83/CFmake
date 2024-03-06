/*!
 \file _SwGuiCompQDockWidget.h
 \brief Implementation of the Class _SwGuiCompQDockWidget generant un QDockWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompQDockWidget_H
#define __SwGuiCompQDockWidget_H

/*
  * INCLUDES GLOBAUX
  */
/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Consumer_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Properties/ISwProperty.h"
#include "ISwQDockWidget.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
	\class _SwGuiCompQDockWidget 
	\brief _SwGuiCompQDockWidget generant un QDockWidget
*/
class _SwGuiCompQDockWidget : public SwComponent_Class, public ISwInterfaces_ConsumerObserver, public ISwQDockWidget
{
	Q_OBJECT
	Q_PROPERTY(bool forceFloating READ GetForceFloating WRITE SetForceFloating)
	Q_PROPERTY (bool showTitleBar READ getShowTitleBar	WRITE setShowTitleBar);

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
    QWidget * _handle_widget;
	/* Force floating because in Qt, the floating is true until the dock is parent */
	bool _forceFloating;
	bool _showTitleBar;
	QWidget* _titleBar;

public:
    /*! \brief Constructeur */
    _SwGuiCompQDockWidget();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompQDockWidget();	

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
    // Interface ISwQDockWidget
    //---------------------------------------------------------------------
    /*! \brief Renvoie le dockwidget
    \return le dockwidget */
	virtual QDockWidget & GetDockWidget();


	virtual bool GetForceFloating() const { return _forceFloating; }
	virtual void SetForceFloating(bool val) { _forceFloating = val; }

	bool getShowTitleBar();
	void setShowTitleBar(bool val);
};

#endif 
