/*!
 \file ISwMainWindow.h
 \brief interface fenetre principale
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwMainWindow_H
#define _ISwMainWindow_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMainWindow>


namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \interface ISwMainWindow 
		 \brief interface definissant une QMainWindow
         \ingroup SwGuiGrp
         \swinterface
		*/
        class ISwMainWindow 
		{   
		public:
            /*! \brief Renvoie la fenetre principale
            \return la fenetre principale */
			virtual QMainWindow & GetMainWindow()=0;
		};
	}
}
#endif 
