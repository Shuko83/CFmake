/**
@file ISwServiceCodeTimer.h
@brief Service permettant de timer du code
@author Arnaud AUBRY
 */

#ifndef _STREAMWORK_SWCORE__SWSERVICECODETIMER_H
#define _STREAMWORK_SWCORE__SWSERVICECODETIMER_H

#include "ISwServiceCodeTimer.h"
#include <QMap>
#include "_SwPerfChrono.h"

namespace StreamWork 
{
    namespace SwCore 
    {
        /**
        @class _SwServiceCodeTimer
        @brief Parameters service implementation
        */
        class _SwServiceCodeTimer : public ISwServiceCodeTimer 
        {
        public:
            /** @brief Constructor */
            _SwServiceCodeTimer();
            /** @brief Destructor */
            virtual ~_SwServiceCodeTimer();
          
			//---------------------------------------------------------------------
			// Interface ISwServiceCodeTimer
			//---------------------------------------------------------------------
			/**
             * @brief	: permet de démarrer le timer
             * Param	: void * caller - pointeur sur la classe appelante
             */
			virtual void start (void * caller) ;

            /**
             * @brief	: Permet de récupérer le temps écouler depuis le start 
             * Return	: double - en ms
             * Param	: void * caller - pointeur sur la classe appelante
             */
            virtual double getTime (void * caller) ;

        private:
            QMap<void *, _SwPerfChrono*> _timers;        
        };
    }
}

#endif
