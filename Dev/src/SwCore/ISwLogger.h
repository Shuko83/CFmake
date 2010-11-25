/*!
\file ISwLogger.h
\date 12/04/2006
\brief interface de tout logger
\author  Big
\version 1.0
 */

#ifndef _ISwLogger_H
#define _ISwLogger_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>

namespace StreamWork 
{
    namespace SwCore
    {
        class ISwLogRecorder;
        /*!
        \enum T_Log_Level
        \brief Etat de connection
        */
        typedef enum {
            LogLvl_Debug=0,
            LogLvl_Info=1,
            LogLvl_Warning=2,
            LogLvl_Critical=3,
            LogLvl_Emergency=4
        } TSw_Log_Level;
 
        /*!
        \interface ISwLogger
        \brief interface pour le log
        @ingroup SwCoreGrp
        @swinterface

        Cette interface definit le mecanisme de log
        Par exemple pour ecrire un log d'info:
        \code
        SW_APP->Logger().Log(LogLvl_Info,"Hello world");
        \endcode
        ou mieux utliser la macro SW_LOG
        \code
        SW_LOG.Log(LogLvl_Info,"Hello world");
        \endcode

        Les methodes AttachLogRecorder et DetachLogRecorder permet de gerer des
        enregistreurs de log.

        */
        class ISwLogger  {
        public:
	        /*! \brief Methode de log */
	        virtual void Log(TSw_Log_Level,const char *format,...)=0;
	        /*! \brief Methode de log */
	        virtual void Log(TSw_Log_Level,QString)=0;
	        /*! \brief Permet d'attacher un log recorder*/
	        virtual void AttachLogRecorder(ISwLogRecorder * log_recorder)=0;
	        /*! \brief Permet de detacher un log recorder*/
	        virtual void DetachLogRecorder(ISwLogRecorder * log_recorder)=0;
            
        };
    }
}

#endif
