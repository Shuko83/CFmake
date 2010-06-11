/*!
 \file ISwDockWidget.h
 \brief interface dockwidget
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwDockWidget_H
#define _ISwDockWidget_H

/*
  * INCLUDES GLOBAUX
  */
#include <QDockWidget>

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \class ISwDockWidget 
		 \brief interface dockwidget
		*/
        class ISwDockWidget 
		{   
		public:
            /*! \brief Renvoie le menu
            \return le menu */
			virtual QDockWidget & GetDockWidget()=0;
		};
	}
}
#endif 
