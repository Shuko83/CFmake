/*!
 \file ISwActionList.h
 \brief interface Action List
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwActionList_H
#define _ISwActionList_H

/*
  * INCLUDES GLOBAUX
  */
#include <QList>  
#include <QAction>
#include "ISwActionListListener.h"

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \interface ISwActionList 
		 \brief interface definissant une liste de QActions  
         \ingroup SwGuiGrp
         \swinterface

		*/
    class ISwActionList 
		{   
		public:
		  
		  virtual  ~ISwActionList(){}
            /*! \brief Renvoie le Action
            \return le Action */
			virtual QList<QAction *> & GetActions()=0;
			
			virtual void addActionListListener(ISwActionListListener * observer) = 0;  
			
			virtual void removeActionListListener(ISwActionListListener * observer) = 0;
       
		};
	}
}
#endif 
