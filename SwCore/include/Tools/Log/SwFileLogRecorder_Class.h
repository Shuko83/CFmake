/*!
\file SwFileLogRecorder_Class.h
\date 12/04/2006
\brief log recorder basique dans un fichier
\author  Big
\version 1.0
 */

#ifndef _SwFileLogRecorder_Class_H
#define _SwFileLogRecorder_Class_H
/*
  * INCLUDES GLOBAUX
 */
#include <QFile>
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "SwCoreConstantes.h"
#include "Tools/Log/ISwLogRecorder.h"

namespace StreamWork
{
    namespace SwCore
    {
        /*!
        \class SwFileLogRecorder_Class
        \brief log recorder basique dans un fichier
        @ingroup SwCoreGrp
        */
        class BUILD_SWCORE SwFileLogRecorder_Class: public ISwLogRecorder  {
        private:
            /*! \brief fichier de log */
            QFile _log_file;
        public:
            /*! \brief Constructeur */
            SwFileLogRecorder_Class(QString filename);
            /*! \brief Destructeur */
            virtual ~SwFileLogRecorder_Class();
	        /*! \brief Methode de log */
	        void RecordLog(TSw_Log_Level level,QString msg);
        };
    }
}

#endif
