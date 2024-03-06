/*!
 \file ISwListDockWidget.h
 \brief interface SwListDockWidget
 \date
 \author
*/

#ifndef _ISwListDockWidget_H
#define _ISwListDockWidget_H

/*
  * INCLUDES GLOBAUX
  */
//#include <QDockWidget>
#include "SwDockWidget_DockWidget.h"
#include "ISwListDockWidgetListener.h"

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \interface ISwListDockWidget 
		 \brief interface definissant un DockWidget
         \ingroup SwGuiGrp
         \swinterface
		*/
        class ISwListDockWidget 
		{   
		public:
            /*! \brief Renvoie le widget
            \return le widget */
			virtual QList<SwDockWidget_DockWidget*> GetListDockWidget() = 0;  

			/**
			 * @brief    : S'enregistrer en tant que listener
			 * @param	 : ISwListDockWidgetListener * observer - Pointeur sur l'oberserveur
			 */
			virtual void addDockWidgetListener(ISwListDockWidgetListener * observer) = 0;

			/**
			 * @brief    : Se désenregistrer 
			 * @param	 : ISwListDockWidgetListener * observer - Pointeur sur l'oberserveur
			 */
			virtual void removeDockWidgetListener(ISwListDockWidgetListener * observer) = 0;

			/**
			 * @brief	Renvoi le nom du widget
			 */
			virtual QString getDockWidgetName() = 0;
		};
	}
}
#endif 
