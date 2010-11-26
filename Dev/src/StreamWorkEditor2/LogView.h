/**
@file LogView.h
@brief Vue log
@author Big
 */

#ifndef _LOGVIEW_H
#define _LOGVIEW_H

#include <QDialog>
#include <QTimer>
#include <QMutex>
#include "ui_LogView.h"
#include "ISwLogRecorder.h"
/**
@class LogView
@brief Vue log
*/
using namespace StreamWork::SwCore;

class LogView : public QDialog , public ISwLogRecorder{
    Q_OBJECT
public:
    /** @brief Destructor */
    virtual ~LogView();
    //Singleton Access
    static LogView * getInstance();
    //Implementation ISwLogRecorder 
    virtual void RecordLog(TSw_Log_Level level,QString msg);
public slots:
    //Update
    void updateLog();

protected:
    /** @brief Constructor */
    LogView();
    
private:
    //Here private members
    Ui_Dialog ui;

    QTimer _timer;
    QMutex _mutex;
    QStringList _msgs;
};

#endif
