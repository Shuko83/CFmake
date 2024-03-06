/*!
\file ISwLogRecorder.h
\date 12/04/2006
\brief interface de tout logger
\author  Big
\version 1.0
 */

#ifndef _ISwLogRecorder_H
#define _ISwLogRecorder_H
/*
  * INCLUDES LOCAUX
 */
#include "Tools/Log/ISwLogger.h"

namespace StreamWork 
{
    namespace SwCore
    { 
        /*!
        \interface ISwLogRecorder
        \brief interface de tout log recorder
        @ingroup SwCoreGrp
        @swinterface
        Pour un exemple voir les sources de SwFileLogRecorder_Class
        */
        class ISwLogRecorder  {
        public:
	        /*! \brief Methode de log */
	        virtual void RecordLog(TSw_Log_Level,QString)=0;
        };
    }
}

#endif
