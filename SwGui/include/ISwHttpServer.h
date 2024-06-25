/*!
 \file ISwHttpServer.h
 \brief interface serveur http
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwHttpServer_H
#define _ISwHttpServer_H

/*
  * INCLUDES GLOBAUX
  */
#include "ISwHttpPart.h"

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \interface ISwHttpServer 
		 \brief interface serveur http
         \ingroup SwGuiGrp
         \swinterface

		*/
        class ISwHttpServer 
		{   
		public:
            /*! \brief Permet d'enregistrer une partie de serveur http */
			virtual void registerHttpPart(ISwHttpPart *)=0;
            /*! \brief Permet de desenregistrer une partie de serveur http */
			virtual void unregisterHttpPart(ISwHttpPart *)=0;
		};
	}
}
#endif 
