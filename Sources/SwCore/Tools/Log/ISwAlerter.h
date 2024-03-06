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
        \brief Niveau d'alerte
        */
        typedef enum {
            AlertLvl_Debug      = 0,
            AlertLvl_Info       = 1,
            AlertLvl_Warning    = 2,
            AlertLvl_Critical   = 3,
            AlertLvl_Emergency  = 4
        } TSw_Alert_Level;
 
        /*!
        \interface ISwAlerter
        \brief interface pour les alertes
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwAlerter  {
        public:
	        /*! \brief Methode de log au format printf
            \param[in] level Niveau d'alerte
            \param[in] format format printf
            */
	        virtual void Alert(TSw_Alert_Level level,const char *format,...)=0;
	        /*! \brief Methode de log au format Qt
            \param[in] level Niveau d'alerte
            \param[in] message message d'alerte
            */
	        virtual void Alert(TSw_Alert_Level level,QString message)=0;
	        /*! \brief Permet d'attacher un enregistreur d'alerte
            \param[in] alert_recorder enregistreur d'alerte
            */
	        virtual void AttachAlertRecorder(ISwAlertRecorder * alert_recorder)=0;
	        /*! \brief Permet de detacher un enregistreur d'alerte
            \param[in] alert_recorder enregistreur d'alerte
            */
	        virtual void DetachAlertRecorder(ISwAlertRecorder * alert_recorder)=0;
            
        };
    }
}

#endif
