/*!
 \file ISwToolBar.h
 \brief interface toolbar
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwToolBar_H
#define _ISwToolBar_H

/*
  * INCLUDES GLOBAUX
  */
#include <QToolBar>


namespace StreamWork
{
	namespace SwGui
	{
        /*!
		@interface ISwToolBar 
		@brief interface toolbar
        @ingroup SwGuiGrp
        @swinterface
		*/
        class ISwToolBar 
		{   
		public:
            /*! \brief Renvoie le menu
            \return le menu */
			virtual QToolBar & GetToolBar()=0;
		};
	}
}
#endif 
