/*!
\file ISwAlertRecorder.h
\date 25/03/09
\brief interface pour alert
\author  HLG
\version 1.0
 */

#ifndef _ISwAlertRecorder_H
#define _ISwAlertRecorder_H
/*
  * INCLUDES LOCAUX
 */
#include "ISwAlerter.h"

namespace StreamWork 
{
    namespace SwCore
    { 
        /*!
        \class ISwAlertRecorder
        \brief interface de tout alert recorder
        */
        class ISwAlertRecorder  {
        public:
	        /*! \brief Methode d'alerte */
	        virtual void RecordAlert(TSw_Alert_Level,QString)=0;
        };
    }
}

#endif
