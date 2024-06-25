/*!
 \file ISwActionListListener.h
 \brief interface Action List Listener
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwActionListListener_H
#define _ISwActionListListener_H

/*
  * INCLUDES GLOBAUX
  */
#include <QList>  
#include <QAction>

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \interface ISwActionListListener 
		 \brief interface definissant une liste de QActions  
         \ingroup SwGuiGrp
         \swinterface

		*/
    class ISwActionListListener 
		{   
		public:
			
		  virtual ~ISwActionListListener(){}
		  
			virtual void listChanged(QList<QAction *> list) = 0;  
       
		};
	}
}
#endif 
