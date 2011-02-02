/**
* @file ISwQWidgetListener.h
* @brief interface QWidget listener pour les totes
* @version 1.0
* @date 2011/02/01 - 1:2:2011 - 15:56
* @author AAY
*/

#ifndef _ISWQWIDGETLISTENER_H
#define _ISWQWIDGETLISTENER_H

/*
* INCLUDES GLOBAUX
*/
#include <QList>  
#include <QWidget>

namespace StreamWork
{
	namespace SwGui
	{
		/*!
		\interface ISwQWidgetListener 
		\brief interface de gestion des QWdigets des totes
		\ingroup SwGuiGrp
		\swinterface
		*/
		class ISwQWidgetListener 
		{   
		public:

			virtual ~ISwQWidgetListener(){};

			/**
			 * @brief    : Ajout d'un nouveau QWidget
			 * @param	 : QWidget * - Pointeur vers le QWidget ajouté
			 */
			virtual void addWidget(QWidget * widget) = 0;  

			/**
			 * @brief    : Suppression d'un nouveau QWidget
			 * @param	 : QWidget * widget - Pointeur vers le QWidget supprimé
			 */
			virtual void removeWidget(QWidget *widget) = 0;  

		};
	}
}
#endif 
