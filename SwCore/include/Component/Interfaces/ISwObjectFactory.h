/*!
 \file ISwObjectFactory.h
 \brief Interface de fabrication d'objet
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#ifndef _ISwObjectFactory_H
#define _ISwObjectFactory_H

/*
  * INCLUDES LOCAUX
  */

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \interface ISwObjectFactory
		 \brief Interface de fabrication d'objet
        @ingroup SwCoreGrp
        @swinterface
		*/
        class ISwObjectFactory 
		{
		public:
            /*! \brief Renvoie une instance de l'objet creé
			le parametre est a l'usage de l'implementeur
			*/
            virtual void * CreateObject(void *)=0;
		};
	}
}
#endif
