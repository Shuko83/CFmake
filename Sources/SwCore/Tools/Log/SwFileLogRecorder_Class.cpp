/*!
\file SwFileLogRecorder_Class.cpp
\date 12/04/2006
\brief log recorder basique dans un fichier
\author  Big
\version 1.0
 */

#include "Tools/Log/SwFileLogRecorder_Class.h"
#include "Tools/Exception/SwException.h"
#include "Main/SwMacros.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
SwFileLogRecorder_Class::SwFileLogRecorder_Class(QString filename):_log_file(filename) {
    if (_log_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)==false) {
        LAUNCH_SWEXCEPTION("SwCore",QString("Unable to open log file %1").arg(filename));
    }
}
/*! \brief Destructeur */
SwFileLogRecorder_Class::~SwFileLogRecorder_Class(){
    _log_file.close();
}
/*! \brief Methode de log */
void SwFileLogRecorder_Class::RecordLog(TSw_Log_Level level,QString msg){
    switch(level) {
        case LogLvl_Debug:
            break;
        case LogLvl_Info:
            _log_file.write(QString("<Informations>:").toLatin1());break;
        case LogLvl_Warning:
            _log_file.write(QString("<Warning>:").toLatin1());break;
        case LogLvl_Critical:
            _log_file.write(QString("<Critical>:").toLatin1());break;
        case LogLvl_Emergency:
            _log_file.write(QString("<Emergency>:").toLatin1());break;
        default:
            _log_file.write(QString("<Unknown>:").toLatin1());break;
    }
    _log_file.write(msg.toLatin1());
    _log_file.flush();
}
