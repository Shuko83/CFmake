/*!
 \file ISwEventObserver.h
 \brief interface
 \date 16/01/2012
 \author LCP
*/

#ifndef _ISwEventObserver_H
#define _ISwEventObserver_H

/*
  * INCLUDES GLOBAUX
  */
#include <QEvent>


namespace StreamWork
{
	namespace SwGui
	{
        class ISwEventObserver 
		{   
		public:
			virtual void onEvent ( QEvent * event )=0;
		};
	}
}
#endif 
