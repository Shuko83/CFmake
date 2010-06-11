/*!
 \file ISwLayout.h
 \brief interface Layout
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwLayout_H
#define _ISwLayout_H

/*
  * INCLUDES GLOBAUX
  */
#include <QLayout>

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \class ISwLayout 
		 \brief interface Layout
		*/
        class ISwLayout 
		{   
		public:
            /*! \brief Renvoie le Layout
            \return le Layout */
			virtual QLayout & GetLayout()=0;
            /*! \brief Liberation du layout (doit etre appele lors de la liberation du layout)
            Attention, le layout est (et doit) etre detruit par cette methode*/
			virtual void LiberateLayout()=0;
		};
	}
}
#endif 
