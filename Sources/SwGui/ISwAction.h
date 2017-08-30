/*!
 \file ISwAction.h
 \brief interface Action
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwAction_H
#define _ISwAction_H

/*
  * INCLUDES GLOBAUX
  */
#include <QAction>

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \interface ISwAction 
		 \brief interface definissant une QAction
         \ingroup SwGuiGrp
         \swinterface

		*/
        class ISwAction 
		{   
		public:
            /*! \brief Renvoie le Action
            \return le Action */
			virtual QAction & GetAction()=0;
		};
	}
}
#endif 
