/*!
 \file ISwExecutor2.h
 \brief Implementation of the Interface ISwExecutor2
 \date 23-ao t-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#ifndef _ISwExecutor2_H
#define _ISwExecutor2_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
/*
  * INCLUDES LOCAUX
  */
#include "ISwExecutor.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \interface ISwExecutor2
		 \brief Interface definissant une interface d'execution (Application console)   stoppable
        @ingroup SwCoreGrp
        @swinterface
         \todo A revoir
		*/
		class ISwExecutor2 : public virtual ISwExecutor
		{   
		public:
            /*! \brief Arrete l'execution du stream
            \return le resultat de l'operation */
			virtual int StreamStop() =0;
		};
	}
}
#endif 
