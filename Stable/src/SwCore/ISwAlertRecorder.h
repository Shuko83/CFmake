/*!
\file ISwAlertRecorder.h
\date 25/03/09
\brief interface de tout enregistreur d'alerte
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
        \brief interface de tout enregistreur d'alerte
        */
        class ISwAlertRecorder  {
        public:
	        /*! \brief Callback appel&eacute;e lorsqu'une alerte est lev&eacute;e
            \param[in] level Niveau d'alerte
            \param[in] message message d'alerte
            */
	        virtual void RecordAlert(TSw_Alert_Level level,QString message)=0;
        };
    }
}

#endif
