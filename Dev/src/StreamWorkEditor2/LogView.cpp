/**
@file LogView.cpp
@brief Vue log
@author Big
 */

#include "LogView.h"
 
static LogView * lView=0;
/** @brief Constructor */
LogView::LogView():QDialog() {
	ui.setupUi(this);
    connect(&_timer,SIGNAL(timeout()),this,SLOT(updateLog()));
    _timer.start(200);
    connect(ui.clearButton,SIGNAL(clicked()),ui.logView,SLOT(clear()));
}
/** @brief Destructor */
LogView::~LogView() {
  //TO DO destruction
}
//Here public method
LogView * LogView::getInstance() {
    if (lView==0) {
        lView=new LogView();
    }
    return lView;
}
//Implementation ISwLogRecorder 
void LogView::RecordLog(TSw_Log_Level level,QString msg) {
    _mutex.lock();
    QString header;
    switch(level) {
        case LogLvl_Debug:
            header="Debug:";
            break;
        case LogLvl_Info:
            header="Info:";
            break;
        case LogLvl_Warning:
            header="Warn:";
            break;
        case LogLvl_Critical:
            header="Crit:";
            break;
        case LogLvl_Emergency:
            header="Emer:";
            break;
        default:
            header="????:";
            break;
    }
    
    _msgs.push_back(header+msg);
    _mutex.unlock();
}
//Update
void LogView::updateLog() {
    if (_mutex.tryLock(50)) {
        for(int i=0;i<_msgs.count();i++) {
            ui.logView->appendPlainText(_msgs[i]);
        }
        _msgs.clear();
        _mutex.unlock();
    }
}

