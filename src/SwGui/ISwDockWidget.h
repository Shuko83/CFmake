/*!
 \file ISwDockWidget.h
 \brief interface SwDockWidget
 \date
 \author
*/

#ifndef _ISwDockWidget_H
#define _ISwDockWidget_H

/*
  * INCLUDES GLOBAUX
  */
//#include <QDockWidget>
#include "SwDockWidget_DockWidget.h"

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \interface ISwDockWidget 
		 \brief interface definissant un DockWidget
         \ingroup SwGuiGrp
         \swinterface
		*/
        class ISwDockWidget 
		{   
		public:
            /*! \brief Renvoie le widget
            \return le widget */
			virtual SwDockWidget_DockWidget & GetDockWidget()=0;
		};
	}
}
#endif 
