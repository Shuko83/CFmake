/**
* @file ISwListQDockWidgetListener.h
* @brief interface QDockWidget listener pour les totes
* @version 1.0
* @date 2011/02/01 - 1:2:2011 - 15:56
* @author AAY
*/

#ifndef _ISwListQDockWidgetLISTENER_H
#define _ISwListQDockWidgetLISTENER_H

/*
* INCLUDES GLOBAUX
*/
#include <QList>  
#include <QDockWidget>

namespace StreamWork
{
	namespace SwGui
	{
		/*!
		\interface ISwListQDockWidgetListener 
		\brief interface de gestion des QWdigets des totes
		\ingroup SwGuiGrp
		\swinterface
		*/
		class ISwListQDockWidgetListener 
		{   
		public:

			virtual ~ISwListQDockWidgetListener(){};

			/**
			 * @brief    : Ajout d'un nouveau QDockWidget
			 * @param	 : QDockWidget * - Pointeur vers le QDockWidget ajouté
			 */
			virtual void addQDockWidget(QDockWidget * widget) = 0;  

			/**
			 * @brief    : Suppression d'un nouveau QDockWidget
			 * @param	 : QDockWidget * widget - Pointeur vers le QDockWidget supprimé
			 */
			virtual void removeQDockWidget(QDockWidget *widget) = 0;  

		};
	}
}
#endif 
