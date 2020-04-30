/**
@file LogView.cpp
@brief Vue log
@author Big
 */

#include "LogView.h"
#include <QSettings>
 
/** @brief Constructor */
LogView::LogView():QDialog() 
{

	ui.setupUi(this);
	ui.regExpEntry->addItem(".*");
	ui.regExpEntry->addItem("Debug :.*");
    ui.regExpEntry->addItem("Info :.*");
	ui.regExpEntry->addItem("Warn :.*");
	ui.regExpEntry->addItem("Crit :.*");
	ui.regExpEntry->addItem("Emer :.*");
    ui.regExpEntry->setCurrentIndex(-1);
    connect(&_timer,SIGNAL(timeout()),this,SLOT(updateLog()));
    _timer.start(200);
    connect(ui.clearButton,SIGNAL(clicked()),this,SLOT(clearLog()));
    connect(ui.regExpEntry,SIGNAL(currentIndexChanged ( const QString & )),this,SLOT(onRegexpTextChange( const QString &)));
    connect(ui.regExpEntry,SIGNAL(editTextChanged ( const QString &  )),this,SLOT(onRegexpTextChangeOnFly( const QString &)));
	connect(ui.cb_enable,SIGNAL(stateChanged ( int )),this,SLOT(onCheckBoxChange( int)));
    ui.regExpEntry->setInsertPolicy(QComboBox::InsertAtTop);

	QSettings settings;
	_logEnable = settings.value("logEnable",true).toBool();

	ui.cb_enable->setChecked(_logEnable);
}
/** @brief Destructor */
LogView::~LogView() {
  //TO DO destruction
}
//Here public method
LogView * LogView::getInstance() {
    static LogView instance;
    return &instance;
}
//Implementation ISwLogRecorder 
void LogView::RecordLog(TSw_Log_Level level,QString msg) {

	if(!_logEnable)
		return;


    _mutex.lock();
    QString header;
    switch(level) {
        case LogLvl_Debug:
			header="<span style='color:#2aa0b9'>Debug : </span>";
            break;
        case LogLvl_Info:
            header="<span style='color:#9d9d9d'>Info : </span>";
            break;
        case LogLvl_Warning:
            header="<span style='color:#f4be1b'>Warn : </span>";
            break;
        case LogLvl_Critical:
            header="<span style='color:#e81d1d'>Crit : </span>";
            break;
        case LogLvl_Emergency:
            header="<span style='color:#2aa0b9'>Emer : </span>";
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
            _content.push_back(_msgs[i]);
            if (!_regentry.isEmpty()) {
                if (_content[i].contains(_regexp)) {
                    ui.logView->append(_msgs[i]);
                }
            } else {
                ui.logView->append(_msgs[i]);
            }
        }
        _msgs.clear();
        _mutex.unlock();
    }
}
/** @brief Sur changement du texte de combo */
void LogView::onRegexpTextChangeOnFly( const QString & text) {
  _regentry=text;
  if (_regentry.isEmpty()) {
      ui.logView->clear();
      ui.logView->append(_content.join("\n"));
  }
}
/** @brief Sur changement du texte de combo */
void LogView::onRegexpTextChange( const QString & text) {
  _regentry=text;
  _regexp.setPattern(_regentry);
  ui.logView->clear();
  if (_regentry.isEmpty()) {
    return;
  }
  for(int i=0;i<_content.count();i++) {
        if (_content[i].contains(_regexp)) {
            ui.logView->append(_content[i]);
        } 
  }
}

//clear
void  LogView::clearLog() {
    ui.logView->clear();
    _content.clear();
}

//-----------------------------------------------------------------------
void LogView::onCheckBoxChange( int val )
{
	QSettings settings;
	settings.setValue("logEnable", val);
	_logEnable = val;

	if(!_logEnable)
	{
		_timer.stop();
		_msgs.clear();
		_content.clear();
	}
	else
	{
		_timer.start(200);
	}
	
}
