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
#include <QRegExp>
#include "ui_LogView.h"
#include "Tools/Log/ISwLogRecorder.h"
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
    /** @brief Sur changement du texte de combo */
    void onRegexpTextChangeOnFly( const QString & text);
    /** @brief Sur changement du texte de combo */
    void onRegexpTextChange( const QString & text);
	/** @brief Sur changement de l'activation du log*/
	void onCheckBoxChange( int val);
    //clear
    void clearLog();
protected:
    /** @brief Constructor */
    LogView();
private:
    //Here private members
    Ui_Dialog ui;

    QTimer _timer;
    QMutex _mutex;
    QStringList _msgs;
    QRegularExpression _regexp;
    QString _regentry;
    QStringList _content;
	bool _logEnable;
};

#endif
