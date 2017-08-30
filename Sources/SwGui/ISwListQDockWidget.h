/**
* @file ISwListQDockWidget.h
* @brief Interface listener des QDockWidgets
* @version 1.0
* @date 2011/02/01 - 1:2:2011 - 16:00
* @author AAY
*/

#ifndef _ISwListQDockWidget_H
#define _ISwListQDockWidget_H

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
		\interface ISwListQDockWidget 
		\brief Interface listener des QDockWidgets
		\ingroup SwGuiGrp
		\swinterface

		*/
		class ISwListQDockWidget
		{   
		public:

			virtual  ~ISwListQDockWidget(){}

			/**
			 * @brief    : Permet de récupérer la liste des QDockWidget initiale
			 * @return   : QList<QDockWidget*> - Liste de pointeur QDockWidget
			 */
			virtual QList<QDockWidget*> get() = 0;  

			/**
			 * @brief    : S'enregistrer en tant que listener
			 * @param	 : ISwListQDockWidgetListener * observer - Pointeur sur l'oberserveur
			 */
			virtual void addQDockWidgetListener(ISwListQDockWidgetListener * observer) = 0;  

			/**
			 * @brief    : Se désenregistrer 
			 * @param	 : ISwListQDockWidgetListener * observer - Pointeur sur l'oberserveur
			 */
			virtual void removeQDockWidgetListener(ISwListQDockWidgetListener * observer) = 0;

		};
	}
}
#endif 
