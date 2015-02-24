/*!
 \file ISwWidget.h
 \brief interface widget
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwWidget_H
#define _ISwWidget_H

/*
  * INCLUDES GLOBAUX
  */
#include <QWidget>


namespace StreamWork
{
	namespace SwGui
	{
        /*!
		@interface ISwWidget 
		@brief interface definissant un QWidget
        @ingroup SwGuiGrp
        @swinterface
		*/
        class ISwWidget 
		{   
		public:
            /*! \brief Renvoie le widget
            \return le widget */
			virtual QWidget & GetWidget()=0;
		};
	}
}
#endif 
