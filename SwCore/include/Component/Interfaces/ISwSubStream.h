/**
@file ISwSubStream.h
@brief Si implemente, definit un sous stream
@author Big
 */

#ifndef _STREAMWORK_SWCORE_ISWSUBSTREAM_H
#define _STREAMWORK_SWCORE_ISWSUBSTREAM_H

#include "Component/Base/SwComponent_Class.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class ISwSubStream
        @brief Si implemente, definit un sous stream
        @ingroup SwCoreGrp
        @swinterface
       */
        class ISwSubStream {
        public:
            /*! \brief Permet d'acceder au sous stream*/
            virtual SwComponent_Class * getSubStream()=0;
            /*! \brief Permet d'acceder au path du fichier correspondant au sous stream*/
            virtual QString getSubStreamPath()=0;
            
        };
    
    }

}

#endif
