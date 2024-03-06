/*!
 \file ISwService.h
 \brief Implementation of the Interface ISwService
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwService_H
#define _ISwService_H

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
		 \interface ISwService 
		 \brief Interface definissant un service
        @ingroup SwCoreGrp
        @swinterface

         Cette interface doit être implementé par tout service

         3 methodes doivent être définies:
         - la methode liberate permet de signaler au service qu'ils est desenregistrer du service manager.
           Habituellement, cette methode détruit le service
         - la methode GetServiceName permet d'identifier le service
         - la methode GetServiceRealName permet d'identifier le service via son nom de compilation (RTTI)
		*/
		class ISwService
		{   
        protected:
            friend class SwServicesManager_Class;
            /*! \brief Est appele lors de l'enregistrement du service*/
            virtual void InitializeService(){};   
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            virtual void Liberate()=0;   
		public:
            /*! \brief Renvoie le nom du service
            \return le nom du service */
			virtual QString GetServiceName() =0;
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
			virtual QString GetServiceRealName() =0;
		};
	}
}
#endif 
