/*!
 \file ISwEventObservable.h
 \brief interface
 \date 16/01/2012
 \author LCP
*/

#ifndef _ISwEvent_H
#define _ISwEvent_H

/*
  * INCLUDES GLOBAUX
  */
#include <QEvent>
#include "ISwEventObserver.h"

namespace StreamWork
{
	namespace SwGui
	{

		class ISwEvent 
		{   
		public:
			virtual void addObserver(ISwEventObserver * obs) =0;
			virtual void removeObserver(ISwEventObserver * obs) =0;
			virtual void notify(QEvent * event) =0;
		};
	}
}
#endif 
