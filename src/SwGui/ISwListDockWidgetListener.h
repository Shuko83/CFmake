/**
* @file ISwListDockWidgetListener.h
* @brief
* @version 1.0
* @date
* @author
*/

#ifndef _ISwListDockWidgetListener_H
#define _ISwListDockWidgetListener_H

/*
* INCLUDES GLOBAUX
*/
#include <QList>  
#include <ISwDockWidget.h>

namespace StreamWork
{
	namespace SwGui
	{
		/*!
		\interface ISwListDockWidgetListener 
		\brief interface de gestion des QWdigets des totes
		\ingroup SwGuiGrp
		\swinterface
		*/
		class ISwListDockWidgetListener 
		{   
		public:

			virtual ~ISwListDockWidgetListener(){};

			/**
			 * @brief    : Ajout d'un nouveau DockWidget
			 * @param	 : DockWidget * - Pointeur vers le DockWidget ajouté
			 */
			virtual void addDockWidget(ISwDockWidget * widget) = 0;

			/**
			 * @brief    : Suppression d'un DockWidget
			 * @param	 : DockWidget * widget - Pointeur vers le DockWidget supprimé
			 */
			virtual void removeDockWidget(ISwDockWidget *widget) = 0;

		};
	}
}
#endif 
