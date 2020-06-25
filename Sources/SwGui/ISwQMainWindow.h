/*!
 \file ISwQMainWindow.h
 \brief interface fenetre principale
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwQMainWindow_H
#define _ISwQMainWindow_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMainWindow>


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
        class ISwQMainWindow 
		{   
		public:
            /*! \brief Renvoie la fenetre principale
            \return la fenetre principale */
			virtual QMainWindow & GetMainWindow()=0;
			virtual int getCloseMode() = 0;
			virtual Qt::WindowStates getWindowState() = 0;

			virtual void saveStateGeometry(const QString& pathTo) const = 0;
			virtual void restoreStateGeometry(const QString& pathFrom) = 0;
		};
	}
}
#endif 
