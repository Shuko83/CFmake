/**
@file ISwClockProvider.h
@brief fournisseur de temps
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWEXECUTION_ISWCLOCKPROVIDER_H
#define _STREAMWORK_SWEXECUTION_ISWCLOCKPROVIDER_H

namespace StreamWork {

    namespace SwExecution {
    
        /**
        @interface ISwClockProvider
        @brief fournisseur de temps
        @ingroup SwExeGrp
        @swinterface
        */
        class ISwClockProvider {
        public:
		    /*! \brief renvoie du temps pour l'initialisation */
	        virtual double queryInitTime()=0;
		    /*! \brief renvoie du temps pour le demarrage */
	        virtual double queryStartTime()=0;
		    /*! \brief renvoie du temps pour l'execution */
	        virtual double queryExecuteTime(bool * isLast)=0;
		    /*! \brief renvoie du temps pour l'arret */
	        virtual double queryStopTime()=0;
		    /*! \brief pause demandé */
	        virtual bool queryPause()=0;
        };

    }
    
}
#endif
