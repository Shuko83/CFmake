/*!
\file _SwGuiCssEditor.h
\brief 
\version 1.0
\date 10/01/2011
\author Arnaud AUBRY
*/

#ifndef _SX_BASICS_UI__SWGUICSSEDITOR_H
#define _SX_BASICS_UI__SWGUICSSEDITOR_H

/*
* INCLUDES LOCAUX
*/
#include <Component.h>
#include <QFileSystemWatcher>
#include <ISwAdminSetup.h>

//Check namespace needed
using namespace StreamWork::SwCore;

/*!
\class _SwGuiCssEditor
\brief Permet de modifier le style de la QAPP
*/
namespace StreamWork
{
	namespace SwGui
	{
		/**
		*	@class _SwGuiCssEditor 
		*	@brief Permet de modifier le style de la QApplication
		*	@ingroup StreamWork::SwGui
		*/     
		class _SwGuiCssEditor : public Component, virtual public StreamWork::SwCore::ISwAdminSetup 
		{
			Q_OBJECT;
			Q_PROPERTY(QString StyleFile READ getStyle WRITE setStyle);

		public:

			/*! \brief Constructeur */
			_SwGuiCssEditor();
			/*! \brief Destructeur */
			virtual ~_SwGuiCssEditor();

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

			/*! \brief Permet d'acceder aux preferences d'administration via StreamWorkEditor*/
			virtual void AdminSetup();

			QString getStyle() ; 
			
		public slots:
				void applyStyle();
				void setStyle(QString val);

		private:
			QString _styleFile;
			QFileSystemWatcher _cssWatcher;	
			
		};
	}
}
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
