/**
@file ISwServiceCodeTimer.h
@brief Service permettant de timer du code
@author Arnaud AUBRY
 */

#ifndef _STREAMWORK_SWCORE_ISWSERVICECODETIMER_H
#define _STREAMWORK_SWCORE_ISWSERVICECODETIMER_H

#include "ISwService.h"

//Nom du service
#define CG_SW_SERVICE_CODE_TIMER "ServiceCodeTimer"

//#pragma warning(disable:4101)

namespace StreamWork 
{
    namespace SwCore 
    {
        /**
         *  @class ISwServiceCodeTimer
         *  @brief Service permettant de timer du code
		 *  @example
		 *  		ISwServiceCodeTimer * timer = dynamic_cast<ISwServiceCodeTimer*>(SW_APP->QueryService(CG_SW_SERVICE_CODE_TIMER));
		 *  		timer->start(this);
		 *  		//HERE CODE
		 *  		qDebug() << timer->getTime(this);
         */
        class ISwServiceCodeTimer  : public ISwService 
        {
        public:
            /**
             * @brief	: permet de démarrer le timer
             * Param	: void * caller - pointeur sur la classe appelante
             */
			virtual void start (void * caller) = 0;

            /**
             * @brief	: Permet de récupérer le temps écouler depuis le start 
             * Return	: double - en ms
             * Param	: void * caller - pointeur sur la classe appelante
             */
            virtual double getTime (void * caller) = 0;
            
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------

            /**
			 * @brief	: Est appele uniquement par le service manager aupres duquel le service est enregistré
						  lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée
             */
            virtual void Liberate(){/*Rien a faire*/}   

            /**
             * @brief	: Renvoie le nom du service
             * @return	: QString - le nom du service
             */
            QString GetServiceName(){return QString(CG_SW_SERVICE_CODE_TIMER); }

            /**
             * @brief	: Renvoie le nom du service reel (le nom de l'interface)
             * @return	: QString - le nom du service reel (le nom de l'interface)
             * @note	: au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()
             */
            QString GetServiceRealName() {return QString(typeid(*this).name());}   
        };
    }
}

#endif
