/*!
 \file ISwQMainWindow.h
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
#include "SwDockWidget_MainWindow.h"


namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \interface ISwQMainWindow 
		 \brief interface definissant une QMainWindow
         \ingroup SwGuiGrp
         \swinterface
		*/
        class ISwMainWindow 
		{   
		public:
            /*! \brief Renvoie la fenetre principale
            \return la fenetre principale */
			virtual SwDockWidget_MainWindow & getMainWindow() = 0;

			/*! \brief Renvoie le widget principal */
			//virtual QMainWindow * getQMainWidget() = 0;
		};
	}
}
#endif 
