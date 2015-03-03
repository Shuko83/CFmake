/*
\file SwActionToToolButton.h
\brief 
\version 1.0
\author EPO
\date 09/01/2015
*/

#ifndef SWGUI_SWACTIONTOTOOLBUTTON_H
#define SWGUI_SWACTIONTOTOOLBUTTON_H


/*
* INCLUDES LOCAUX
*/
#include "SwAssistedComponent.h"
#include <QToolButton>
#include "ISwWidget.h"


namespace StreamWork
{
	namespace SwGui
	{
		/**
		*	@class SwActionToToolButton
		*	@brief
		*/
		class SwActionToToolButton : public StreamWork::SwFoundation::SwAssistedComponent, public ISwWidget
		{
			Q_OBJECT
		public:

			/*! \brief Constructeur */
			SwActionToToolButton();

			/*! \brief Destructeur */
			virtual ~SwActionToToolButton();

			/**
			* @brief    : Initialisation du composant
			* @note	 : A surcharger
			*/
			virtual void initializeComponent() throw(SwException);


			//----------------------------------------------------
			// Interface de consomation d'interfaces
			//----------------------------------------------------

			/**
			* @brief    : Callback appel?e lors de la disponibilit? de l'interface
			* @param	 : QString interfaceName - Nom de l'interface
			*/
			virtual void interfaceAvailable(QString interface_name);

			/**
			* @brief    : Callback appel?e lors de l'indisponibilit? de l'interface
			* @param	 : QString interfaceName - Nom de l'interface
			*/
			virtual void interfaceUnavailable(QString interface_name);

			//---------------------------------------------------------------------
			// Interface ISwWidget
			//---------------------------------------------------------------------
			/*! \brief Renvoie le widget
			\return le widget */
			virtual QWidget & GetWidget();

		protected slots:
			/**
			* @brief    : Update visibility's ToolButton
			*/
			void updateVisibilityAction();

		private:
			QToolButton  * _toolButton;

		};
	}
}



#endif 
