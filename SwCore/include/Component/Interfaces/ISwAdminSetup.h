/**
	@file : ISwAdminSetup.h
	@date : 18/04/2006
	@brief : interface pour la configuration administrateur
	@author : Big
	@version : 1.0
 */

#ifndef _ISwAdminSetup_H
#define _ISwAdminSetup_H


namespace StreamWork 
{
	namespace SwCore
	{
        /**
          @interface ISwAdminSetup
          @brief Interface pour la configuration administrateur. Est appel&eacute; par StreamWorkEditor lors de la commande en popup AdminSetup
          @ingroup SwCoreGrp
          @swinterface
        */
        class ISwAdminSetup  {
        public:
	        /** @brief : Permet d'acceder aux preferences d'administration via StreamWorkEditor*/
	        virtual void AdminSetup()=0;
        };
    }
}

#endif
