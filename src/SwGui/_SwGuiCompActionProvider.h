/*!
\file _SwGuiCompActionProvider.cpp
\brief Implementation of the Class _SwGuiCompActionProvider permettant de générer des QAction
\version 1.0
\date 01/09/2013 2:00
\author AAY
*/

#ifndef __SwGuiCompActionProvider_H
#define __SwGuiCompActionProvider_H

/*
* INCLUDES QT
*/
#include <QMap>

/*
* INCLUDES LOCAUX
*/
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include "ISwAction.h"
#include "SwQActionAdapter.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


/*!
\class _SwGuiCompActionProvider 
\brief _SwGuiCompActionProvider generant un QMenu
*/
class _SwGuiCompActionProvider : public SwComponent_Class
{
public:
	/*! \brief Constructeur */
	_SwGuiCompActionProvider();
	/*! \brief Destructeur */
	virtual ~_SwGuiCompActionProvider();

	/*! \brief Initialisation des ressources
	\note tous les services du composants doivent être déclarés dans cette methodes*/
	virtual void InitializeResources() throw(SwException);
	/*! \brief Callback sur les changements de propriétés*/
	void OnPropertyChange(ISwProperty * property);

protected:

	void signalListChanged();

	/* service de fourniture d'interface */
	SwInterfaces_Provider_Class * _provider_service;

	/* service de gestion des propriétés */
	SwProperties_Class * _properties_service;

	/* nombre d'actions */
	uint _actions_nb;

	/* propriété nombre d'actions*/
	ISwProperty * _actions_nb_property;

	/* map des interfaces actions*/
	QMap<QString,SwQActionAdapter *> _actions;

};




#endif 
