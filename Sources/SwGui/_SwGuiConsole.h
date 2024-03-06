/*!
\file _SwGuiConsole.h
\brief 
\version 1.0
\date 10/01/2011
\author Arnaud AUBRY
*/

#ifndef _SX_BASICS_UI__SWGUICONSOLE_H
#define _SX_BASICS_UI__SWGUICONSOLE_H

/*
* INCLUDES LOCAUX
*/
#include "Component/Base/Component.h"
#include "Component/Interfaces/ISwAdminSetup.h"

//Check namespace needed
using namespace StreamWork::SwCore;

/*!
\class _SwGuiConsole
\brief Permet de modifier le style de la QAPP
*/
namespace StreamWork
{
	namespace SwGui
	{
		/**
		*	@class _SwGuiConsole 
		*	@brief Permet d'afficher une console
		*	@ingroup StreamWork::SwGui
		*/     
		class _SwGuiConsole : public Component
		{
			Q_OBJECT

		public:

			/*! \brief Constructeur */
			_SwGuiConsole();
			/*! \brief Destructeur */
			virtual ~_SwGuiConsole();

			/*! \brief Initialisation du composant*/
			virtual void initializeComponent() throw(SwException);

			/*! \brief terminaison du composante*/
			virtual void terminateComponent();

			/*! \brief evenement de changement de propriete*/
			virtual void eventPropertyChange(ISwProperty * property);

			/*! \brief evenement avant changement de la disponibilité de l'interface*/
			virtual void eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);

			/*! \brief evenement apres changement de la disponibilité de l'interface*/
			virtual void eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);

			/*! \brief evenement sur reception d'une data*/
			virtual void eventReceiveData(SwPin * src,SwData_Class * data);

			/*! \brief evenement sur changement d'activation*/
			virtual void eventActivationChanged();

		protected:

			//--------------------------------------------------------------
			//Properties
			//--------------------------------------------------------------

			/** @brief Property value Stylesheet as QString */
			//Q_PROPERTY (QString StyleFile READ getStyle WRITE setStyle);

			//--------------------------------------------------------------
			//Handle interfaces
			//--------------------------------------------------------------

			
		private:
			
		};

	}
}
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
