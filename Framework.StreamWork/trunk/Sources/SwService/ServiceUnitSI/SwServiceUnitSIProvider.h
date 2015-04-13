/*!
\file SwServiceUnitSIProvider.cpp
\brief Implementation of the Class SwServiceUnitSIProvider permettant de provider des properties du service unit SI (sxCore)
\version 1.0
\date 02/04/2014 2:00
\author EPO
*/

#ifndef _SwServiceUnitSIProvider_H
#define _SwServiceUnitSIProvider_H

/*
* INCLUDES QT
*/
#include <QMap>

/*
* INCLUDES LOCAUX
*/
#include <SwAssistedComponent.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include <_SwServiceUnitSI.h>

namespace StreamWork
{
	/*!
	\class _SwGuiServiceUnitSIProvider 
	\brief _SwGuiServiceUnitSIProvider providant les properties du service unit SI
	*/
	class SwServiceUnitSIProvider : public SwFoundation::SwAssistedComponent
	{
	public:
		/*! \brief Constructeur */
		SwServiceUnitSIProvider();
		/*! \brief Destructeur */
		virtual ~SwServiceUnitSIProvider();

		/*! \brief Initialisation des ressources
		\note tous les services du composants doivent être déclarés dans cette methodes*/
		virtual void initializeComponent() throw(StreamWork::SwCore::SwException);

	protected:

		/* service de fourniture d'interface */
		StreamWork::SwCore::_SwServiceUnitSI * _serviceUnitSI;
	};
}

#endif 
