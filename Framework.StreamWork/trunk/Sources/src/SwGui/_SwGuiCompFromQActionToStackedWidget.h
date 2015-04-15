/*!
 \file _SwGuiCompFromQActionToStackedWidget.h
 \brief Implementation of the Class _SwGuiCompFromQActionToStackedWidget generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SWGUICOMPFROMQACTIONTOSTACKEDWIDGET_H
#define __SWGUICOMPFROMQACTIONTOSTACKEDWIDGET_H

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
#include "ISwStackedWidget_Controler.h"
#include "ISwAction.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*!
	\class _SwGuiCompFromQActionToStackedWidget 
	\brief _SwGuiCompFromQActionToStackedWidget 
*/
class _SwGuiCompFromQActionToStackedWidget :  public SwComponent_Class, public virtual ISwInterfaces_ConsumerObserver,  public virtual ISwStackedWidget_Observer
{
	Q_OBJECT
		Q_ENUMS(SLOTCONNECTION)
		/** @brief Property value gridProjection */
		Q_PROPERTY (SLOTCONNECTION SlotType READ getTypeOfConnection WRITE setTypeOfConnection);

public:
	   /**
		* @brief type de connection de slot :
		* - TOGGLED : toggled
		* - TRIGGERED : triggered
		*/ 
		typedef enum
		{
			TOGGLED,
			TRIGGERED,
		} SLOTCONNECTION;

    /*! \brief Constructeur */
    _SwGuiCompFromQActionToStackedWidget();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompFromQActionToStackedWidget();

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
	// Interface ISwStackedWidget_Observer
	//---------------------------------------------------------------------
	
	/*! \brief notify the observer when the interface change */
	virtual void notify();

	SLOTCONNECTION getTypeOfConnection() const { return _typeOfConnection; }
	void setTypeOfConnection(SLOTCONNECTION val) { _typeOfConnection = val; }


protected:

	/* service de fourniture d'interface */
	SwInterfaces_Provider_Class * _provider_service;
	/* service de consommation d'interface */
	SwInterfaces_Consumer_Class * _consumer_service;
	/* service de gestion des propriétés */
	SwProperties_Class * _properties_service;

	// --- Widgets ---

	/* map des interfaces widgets*/
	QMap<QString,ISwAction *> _actions;

	/* handle temporaire d'interface widget*/
	ISwStackedWidget_Controler * _tmp_handle_StackedWidget;

	/* handle temporaire d'interface action*/
	ISwAction * _tmp_handle_action;

	SLOTCONNECTION _typeOfConnection;

protected slots :
	void switchStackedWidget(bool);
};
#endif 
