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

/*
* INCLUDES LOCAUX
*/
#include "SwDockWidget_DockWidget.h"

namespace StreamWork
{
	namespace SwGui
	{
		/*!
		\interface ISwListDockWidgetListener 
		\brief interface de gestion des liste de SwDockWidget
		\ingroup SwGuiGrp
		\swinterface
		*/
		class ISwListDockWidgetListener 
		{   
		public:

			virtual ~ISwListDockWidgetListener(){};

			/**
			 * @brief    : Ajout d'un nouveau DockWidget
			 * @param	 : widget - Pointeur vers le DockWidget ajouté
			 * @param	 : menuName - Nom de la liste pour affichage dans un menu
			 */
			virtual void addDockWidget(SwDockWidget_DockWidget * widget, QString menuName) = 0;

			/**
			 * @brief    : Suppression d'un DockWidget
			 * @param	 : DockWidget * widget - Pointeur vers le DockWidget supprimé
			 */
			virtual void removeDockWidget(SwDockWidget_DockWidget *widget) = 0;

		};
	}
}
#endif 
