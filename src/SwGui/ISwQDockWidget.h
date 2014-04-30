/*!
 \file ISwQDockWidget.h
 \brief interface dockwidget
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwQDockWidget_H
#define _ISwQDockWidget_H

/*
  * INCLUDES GLOBAUX
  */
#include <QDockWidget>

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \interface ISwQDockWidget 
		 \brief interface definissant un QDockWidget
         \ingroup SwGuiGrp
         \swinterface
		*/
        class ISwQDockWidget 
		{   
		public:
            /*! \brief Renvoie le dock widget
            \return le dock widget */
			virtual QDockWidget & GetDockWidget()=0;

			/**
			 * @brief	: get Force floating because in Qt, the floating is true until the dock is parent
			 * @return	: bool 
			 */
			virtual bool GetForceFloating() const = 0 ;

		};
	}
}
#endif 
