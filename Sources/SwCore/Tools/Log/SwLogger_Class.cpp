/*!
\file SwLogger_Class.cpp
\date 12/04/2006
\brief implementation d'un logger basique
\author  Big
\version 1.0
 */
#include <stdarg.h>
//#include <sys/io>
#include "Tools/Log/SwLogger_Class.h"
#include "Tools/Log/ISwLogRecorder.h"

using namespace StreamWork::SwCore;

static QVector<ISwLogRecorder *> recorders;

/*! \brief Constructeur*/
SwLogger_Class::SwLogger_Class() {
    _log_recorders.clear();
    for(int i=0;i<recorders.count();i++) {
        AttachLogRecorder(recorders[i]);    
    }

}
/*! \brief Destructeur*/
SwLogger_Class::~SwLogger_Class() {
    _log_recorders.clear();
}
/*! \brief Methode de log
\param[in] level niveau du log
\param[in] format format de la chaine (voir printf)
\param[in] ... arguments
*/
void SwLogger_Class::Log(TSw_Log_Level level,const char *format,...){
    char buffer[1000];

    va_list ap;
    va_start(ap,format);
    (void) vsprintf(buffer,format,ap);
    va_end(ap);
    Log(level,QString(buffer));
}
/*! \brief Methode de log
\param[in] level niveau du log
\param[in] log_msg message de log
*/
void SwLogger_Class::Log(TSw_Log_Level level,QString log_msg){
    QSet<ISwLogRecorder *>::iterator it;
    //Pour chaque recorder
    for (it=_log_recorders.begin();it!=_log_recorders.end();it++) {
        //Enregistrer le log
        (*it)->RecordLog(level,log_msg);
    }
}
/*! \brief Permet d'attacher un log recorder
\param[in] log_recorder recorder a attacher
*/
void SwLogger_Class::AttachLogRecorder(ISwLogRecorder * log_recorder){
    QSet<ISwLogRecorder *>::iterator it;

    it=_log_recorders.find(log_recorder);
    if (it!=_log_recorders.end())
        return; //Deja enregistrer
    //Attachement du recorder
    _log_recorders.insert(log_recorder);
}
/*! \brief Permet de detacher un log recorder
\param[in] log_recorder recorder a detacher
*/
void SwLogger_Class::DetachLogRecorder(ISwLogRecorder * log_recorder){
    QSet<ISwLogRecorder *>::iterator it;

    it=_log_recorders.find(log_recorder);
    //Si le recorder est enregistr�
    if (it!=_log_recorders.end()) {
        //Detachement du recorder
        _log_recorders.erase(it);
    }
}

void SwLogPreRecorder::PreAttachLogRecorder(ISwLogRecorder * log_recorder) {
    recorders.push_back(log_recorder);
}
