/*!
 \file ISwStackedWidget_Observer.h
 \brief interface StackedWidget
 \date 18-Janvier-2011
 \author A.Aubry
*/

#ifndef _ISWSTACKEDWIDGET_OBSERVER_H
#define _ISWSTACKEDWIDGET_OBSERVER_H

/*
  * INCLUDES GLOBAUX
  */


namespace StreamWork
{
	namespace SwGui
	{
        /*!
		@interface ISwStackedWidget_Observer 
		@brief interface observer d'un QStackedWidget
        @ingroup SwGuiGrp
        @swinterface
		*/
        class ISwStackedWidget_Observer 
		{   
		public:

            /*! \brief notify the observer when the interface change */
			virtual void notify()=0;

		};
	}
}
#endif 
