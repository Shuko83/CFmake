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
		 \interface ISwDockWidget 
		 \brief interface definissant un QDockWidget
         \ingroup SwGuiGrp
         \swinterface
		*/
        class ISwDockWidget 
		{   
		public:
            /*! \brief Renvoie le dock widget
            \return le dock widget */
			virtual QDockWidget & GetDockWidget()=0;
		};
	}
}
#endif 
