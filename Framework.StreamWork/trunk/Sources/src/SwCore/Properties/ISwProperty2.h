/**
@file ISwProperty2.h
@brief Property etendu
@author Big
 */

#ifndef _ISWPROPERTY2_H
#define _ISWPROPERTY2_H

#include "ISwProperty.h"

namespace StreamWork
{
    namespace SwCore
    {

        /**
        @class ISwProperty2
        @brief Property etendu
        */
        class ISwProperty2 : public virtual ISwProperty{
        public:
            /*! \brief prpriete resettable*/
            virtual bool isResettable()=0;
            /*! \brief reset de la propriété*/
            virtual void reset()=0;
        };

    }
}


#endif
