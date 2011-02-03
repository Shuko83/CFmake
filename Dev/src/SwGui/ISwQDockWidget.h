/**
* @file ISwQDockWidget.h
* @brief Interface listener des QDockWidgets
* @version 1.0
* @date 2011/02/01 - 1:2:2011 - 16:00
* @author AAY
*/

#ifndef _ISWQDOCKWIDGET_H
#define _ISWQDOCKWIDGET_H

/*
* INCLUDES GLOBAUX
*/
#include <QList>  
#include <QAction>
#include "ISwQDockWidgetListener.h"

namespace StreamWork
{
	namespace SwGui
	{
		/*!
		\interface ISwQDockWidget 
		\brief Interface listener des QDockWidgets
		\ingroup SwGuiGrp
		\swinterface

		*/
		class ISwQDockWidget 
		{   
		public:

			virtual  ~ISwQDockWidget(){}

			/**
			 * @brief    : Permet de récupérer la liste des QDockWidget initiale
			 * @return   : QList<QDockWidget*> - Liste de pointeur QDockWidget
			 */
			virtual QList<QDockWidget*> get() = 0;  

			/**
			 * @brief    : S'enregistrer en tant que listener
			 * @param	 : ISwQDockWidgetListener * observer - Pointeur sur l'oberserveur
			 */
			virtual void addQDockWidgetListener(ISwQDockWidgetListener * observer) = 0;  

			/**
			 * @brief    : Se désenregistrer 
			 * @param	 : ISwQDockWidgetListener * observer - Pointeur sur l'oberserveur
			 */
			virtual void removeQDockWidgetListener(ISwQDockWidgetListener * observer) = 0;

		};
	}
}
#endif 
