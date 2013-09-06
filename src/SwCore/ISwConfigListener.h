/**
 @file : ISwConfigListener.h
 @brief: Implementation of the Interface ISwConfigListener
 @date : 02/07/2013 
 @author: CGD
*/

#ifndef _ISwConfigListener_H
#define _ISwConfigListener_H


#include <QtGlobal>


namespace StreamWork
{
	namespace SwCore
	{
        /**
		 @interface : ISwConfigListener 
		 @brief : Interface que doivent implémenter les composants nécessitant d'être 
		 		notifié des changements d'une conf
         @ingroup : SwCoreGrp
         @swinterface

       
        2 methodes doivent être définies:
		- notify() qui permet de savoir à quelle configuration le collector sert.
		- getListenerName() qui permet de récupérer le nom d'un listener

		*/
		class ISwConfigListener
		{     
		public:

            /** @brief : Notification en cas de changement sur une configuration
             *	@param : QString : nom de la conf concernée
             */
			virtual void notifyConfiguration(QString confName) = 0;

			/** @brief : Récupération du nom du Listener pour notifier uniquement ceux concernés
			 *	@param : QString : nom du Listener
             */
			virtual QString getListenerName() = 0;
		};
	}
}
#endif 
