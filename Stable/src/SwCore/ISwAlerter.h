/*!
\file ISwAlerter.h
\date 25/03/09
\brief interface pour alerter
\author  HLG
\version 1.0
 */

#ifndef _ISwAlerter_H
#define _ISwAlerter_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>

namespace StreamWork 
{
    namespace SwCore
    {
        class ISwAlertRecorder;
        /*!
        \enum T_Alert_Level
        \brief Etat de connection
        */
        typedef enum {
            AlertLvl_Debug      = 0,
            AlertLvl_Info       = 1,
            AlertLvl_Warning    = 2,
            AlertLvl_Critical   = 3,
            AlertLvl_Emergency  = 4
        } TSw_Alert_Level;
 
        /*!
        \class ISwAlert
        \brief interface pour les alertes
        */
        class ISwAlerter  {
        public:
	        /*! \brief Methode de log */
	        virtual void Alert(TSw_Alert_Level,const char *format,...)=0;
	        /*! \brief Methode de log */
	        virtual void Alert(TSw_Alert_Level,QString)=0;
	        /*! \brief Permet d'attacher un log recorder*/
	        virtual void AttachAlertRecorder(ISwAlertRecorder * alert_recorder)=0;
	        /*! \brief Permet de detacher un log recorder*/
	        virtual void DetachAlertRecorder(ISwAlertRecorder * alert_recorder)=0;
            
        };
    }
}

#endif
