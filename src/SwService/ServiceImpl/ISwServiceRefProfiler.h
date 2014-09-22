/**
@file ISwServiceRefProfiler.h
@brief Service permettant d'analyser les fuites memoire liées au SwRef
@author Quentin Brun
 */

#ifndef _STREAMWORK_SWCORE_ISWSERVICEREFPROFILER_H
#define _STREAMWORK_SWCORE_ISWSERVICEREFPROFILER_H

#include "ISwService.h"

//Nom du service
#define CG_SW_SERVICE_REF_PROFILER "ServiceRefProfiler"

//#pragma warning(disable:4101)

namespace StreamWork 
{
    namespace SwCore 
    {
        /**
         *  @class ISwServiceRefProfiler
         *  @brief Service permettant d'analyser les fuites memoire liées au SwRef
		 *  @example
		 *  		ISwServiceRefProfiler * refProfiler = dynamic_cast<ISwServiceRefProfiler*>(SW_APP->QueryService(CG_SW_SERVICE_REF_PROFILER));
		 *  		refProfiler->startWatch();
		 *			//all your code
		 *			refProfiler->stopWatch();
		 *			refProfiler->printResults(); //sortie des resultats dans la console 
		 *
         */
        class ISwServiceRefProfiler  : public ISwService 
        {
        public:

			virtual void startWatch() = 0;

			virtual void stopWatch() = 0;

			virtual void cleanWatch() = 0;

			virtual void printResults() = 0;

            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            virtual void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_REF_PROFILER); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()*/
            QString GetServiceRealName() {return QString(typeid(this).name());}    
        };
    }
}

#endif
