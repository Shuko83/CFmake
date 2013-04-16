/*!
\file ISwStackedWidget_Controler.h
\brief interface StackedWidget
\date 18-Janvier-2011
\author A.Aubry
*/

#ifndef _ISWSTACKEDWIDGET_CONTROLER_H
#define _ISWSTACKEDWIDGET_CONTROLER_H

/*
* INCLUDES GLOBAUX
*/
#include "ISwStackedWidget_Observer.h"


namespace StreamWork
{
	namespace SwGui
	{
		/*!
		@interface ISwStackedWidget_Controler 
		@brief interface definissant un QStackedWidget
		@ingroup SwGuiGrp
		@swinterface
		*/
		class ISwStackedWidget_Controler 
		{   
		public:

			/*! \brief Renvoie le nombre de page disponible
			/*	\return un int définissant le nombre de page (0 étant la première)
			*/ 
			virtual int getNbPage()=0;

			/*! \brief Permet de mettre le StackedWidget sur la page de l'index voulu
			*! \param int pageIndex  : L'index de la page voulue
			*/
			virtual void setCurrentIndex(int pageIndex)=0;

			/*! \brief Renvoie l'index courrant
			/*  \return l'index courrant du QStackedWidget
			*/
			virtual int getCurrentIndex()=0;

			/*! \brief Renvoie le nom du widget pour une page donnée
			/*  \param l'index de la page contenant le widget
			/*  \return le nom du widget
			*/
			virtual QString getWidgetName(int pageIndex)=0;

			/*! \brief Renvoie l'icon du widget pour une page donnée
			/*  \param l'index de la page contenant le widget
			/*  \return l'icon du widget
			*/
			virtual QIcon getWidgetIcon( int pageIndex )=0;

			/*! \brief Permet d'enregistrer un observer
			*! \param pointeur sur l'observer
			*/
			virtual void registerStackedWidgetObserver(ISwStackedWidget_Observer *o) = 0;

			/*! \brief Permet de désenregistrer un observer
			*! \param pointeur sur l'observer
			*/
			virtual void unRegisterStackedWidgetObserver(ISwStackedWidget_Observer *o) = 0;


		};
	}
}
#endif 
