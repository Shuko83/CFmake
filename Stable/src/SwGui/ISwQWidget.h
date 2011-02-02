/**
* @file ISwQWidget.h
* @brief Interface listener des QWidgets
* @version 1.0
* @date 2011/02/01 - 1:2:2011 - 16:00
* @author AAY
*/

#ifndef _ISWQWIDGET_H
#define _ISWQWIDGET_H

/*
* INCLUDES GLOBAUX
*/
#include <QList>  
#include <QAction>
#include "ISwQWidgetListener.h"

namespace StreamWork
{
	namespace SwGui
	{
		/*!
		\interface ISwQWidget 
		\brief Interface listener des QWidgets
		\ingroup SwGuiGrp
		\swinterface

		*/
		class ISwQWidget 
		{   
		public:

			virtual  ~ISwQWidget(){}

			/**
			 * @brief    : Permet de récupérer la liste des QWidget initiale
			 * @return   : QList<QWidget*> - Liste de pointeur QWidget
			 */
			virtual QList<QWidget*> get() = 0;  

			/**
			 * @brief    : S'enregistrer en tant que listener
			 * @param	 : ISwQWidgetListener * observer - Pointeur sur l'oberserveur
			 */
			virtual void addQWidgetListener(ISwQWidgetListener * observer) = 0;  

			/**
			 * @brief    : Se désenregistrer 
			 * @param	 : ISwQWidgetListener * observer - Pointeur sur l'oberserveur
			 */
			virtual void removeQWidgetListener(ISwQWidgetListener * observer) = 0;

		};
	}
}
#endif 
