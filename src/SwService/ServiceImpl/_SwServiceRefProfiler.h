/**
@file _SwServiceRefProfiler.h
@brief Implementation du service permettant d'analyser les objets gardé en memoire
@author Quentin Brun
 */

#ifndef _STREAMWORK_SWCORE__SWSERVICEREFPROFILER_H
#define _STREAMWORK_SWCORE__SWSERVICEREFPROFILER_H

#include "ISwServiceRefProfiler.h"
#include <SwRefPtrTools.h>

namespace StreamWork 
{
    namespace SwCore 
    {
        /**
        @class _SwServiceCodeTimer
        @brief Parameters service implementation
        */
        class _SwServiceRefProfiler : public ISwServiceRefProfiler 
        {
        public:
            /** @brief Constructor */
            _SwServiceRefProfiler();
            /** @brief Destructor */
            virtual ~_SwServiceRefProfiler();

			//---------------------------------------------------------------------
			// Interface ISwServiceRefProfiler
			//---------------------------------------------------------------------

			virtual void startWatch();

			virtual void cleanWatch();

			virtual void stopWatch();

			virtual void printResults();

        };
    }
}

#endif
