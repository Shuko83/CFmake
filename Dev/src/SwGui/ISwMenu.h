/*!
 \file ISwMenu.h
 \brief interface menu
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwMenu_H
#define _ISwMenu_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMenu>

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \class ISwMenu 
		 \brief interface menu
		*/
        class ISwMenu 
		{   
		public:
            /*! \brief Renvoie le menu
            \return le menu */
			virtual QMenu & GetMenu()=0;
		};
	}
}
#endif 
