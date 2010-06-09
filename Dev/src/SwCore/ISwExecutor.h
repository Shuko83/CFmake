/*!
 \file ISwExecutor.h
 \brief Implementation of the Interface ISwExecutor
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#ifndef _ISwExecutor_H
#define _ISwExecutor_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
/*
  * INCLUDES LOCAUX
  */
#include "SwCoreConstantes.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \class ISwExecutor 
		 \brief Interface definissant une interface d'execution (Application console)
         \todo A revoir
		*/
		class ISwExecutor
		{   
		public:
            /*! \brief Execute le stream
            \return le resultat de l'operation */
			virtual int StreamExecute() =0;
		};
	}
}
#endif 
