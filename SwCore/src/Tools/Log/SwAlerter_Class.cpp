/*!
\file SwAlerter_Class.cpp
\date 25/03/09
\brief implementation d'un alert basique
\author  HLG
\version 1.0
 */
#include <stdarg.h>
#include "Tools/Log/SwAlerter_Class.h"
#include "Tools/Log/ISwAlertRecorder.h"
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"


using namespace StreamWork::SwCore;

/*! \brief Constructeur*/
SwAlerter_Class::SwAlerter_Class() {
    _alert_recorders.clear();
}
/*! \brief Destructeur*/
SwAlerter_Class::~SwAlerter_Class() {
    _alert_recorders.clear();
}
/*! \brief Methode d'alerte
\param[in] level : niveau d'alerte
\param[in] format : format de la chaine (voir printf)
\param[in] ... arguments
*/
void SwAlerter_Class::Alert(TSw_Alert_Level level,const char *format,...){
    char buffer[1000];

    va_list ap;
    va_start(ap,format);
    (void) vsprintf(buffer,format,ap);
    va_end(ap);
    Alert(level,QString(buffer));
}
/*! \brief Methode d'alert
\param[in] level : niveau d'alerte
\param[in] alert_msg : message d'alerte
*/
void SwAlerter_Class::Alert(TSw_Alert_Level level,QString alert_msg){
    QSet<ISwAlertRecorder *>::iterator it;
    //Enregistrement au  niveau des logs
    SW_LOG.Log((TSw_Log_Level)level,alert_msg);
    //Pour chaque recorder
    for (it=_alert_recorders.begin();it!=_alert_recorders.end();it++) {
        //Enregistrer l'alerte
        (*it)->RecordAlert(level,alert_msg);
    }
}
/*! \brief Permet d'attacher un alert recorder
\param[in] alert_recorder recorder a attacher
*/
void SwAlerter_Class::AttachAlertRecorder(ISwAlertRecorder * alert_recorder){
    QSet<ISwAlertRecorder *>::iterator it;

    it=_alert_recorders.find(alert_recorder);
    if (it!=_alert_recorders.end())
        return; //Deja enregistrer
    //Attachement du recorder
    _alert_recorders.insert(alert_recorder);
}
/*! \brief Permet de detacher un alert recorder
\param[in] alert_recorder recorder a detacher
*/
void SwAlerter_Class::DetachAlertRecorder(ISwAlertRecorder * alert_recorder){
    QSet<ISwAlertRecorder *>::iterator it;

    it=_alert_recorders.find(alert_recorder);
    //Si le recorder est enregistré
    if (it!=_alert_recorders.end()) {
        //Detachement du recorder
        _alert_recorders.erase(it);
    }
}
