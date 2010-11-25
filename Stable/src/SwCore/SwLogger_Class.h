/*!
\file SwLogger_Class.h
\date 12/04/2006
\brief implementation d'un logger basique
\author  Big
\version 1.0
 */

#ifndef _SwLogger_Class_H
#define _SwLogger_Class_H
/*
  * INCLUDES GLOBAUX
 */
#include <QSet>
/*
  * INCLUDES LOCAUX
 */
#include "SwCoreConstantes.h"
#include "ISwLogger.h"

namespace StreamWork
{
    namespace SwCore
    {
        /*!
        \class SwLogger_Class
        \brief logger basique
        @ingroup SwCoreGrp
        */
        class BUILD_SWCORE SwLogger_Class : public ISwLogger {
        private:
            /*! \brief Liste des log recorders*/
            QSet<ISwLogRecorder *> _log_recorders;
        public:
            /*! \brief Constructeur*/
            SwLogger_Class();
            /*! \brief Destructeur*/
            virtual ~SwLogger_Class();
	        /*! \brief Methode de log */
	        void Log(TSw_Log_Level level,const char *format,...);
	        /*! \brief Methode de log */
	        void Log(TSw_Log_Level level,QString log_msg);
	        /*! \brief Permet d'attacher un log recorder*/
	        void AttachLogRecorder(ISwLogRecorder * log_recorder);
	        /*! \brief Permet de detacher un log recorder*/
	        void DetachLogRecorder(ISwLogRecorder * log_recorder);

        };
    }
}

#endif
