/**
@file _RecordWidget.cpp
@brief widget d'enregistremenr
@author F.Bighelli
 */

#include "_RecordWidget.h"
 

/** @brief Constructor */
_RecordWidget::_RecordWidget():QWidget(0) {
	_rPixmap=QPixmap(":/SwRecord/record_symbol.png");
	_rPixmapOff=QPixmap(":/SwRecord/record_symbol_off.png");
    _rPixmapOut=_rPixmap;
    _rEnable=false;


    QHBoxLayout * hLayout=new QHBoxLayout(this);
    hLayout->setMargin(0);
    hLayout->setSpacing(5);

    _lIcon=new QLabel(this);
    _lIcon->setPixmap(_rPixmapOff);
    hLayout->addWidget(_lIcon,0,Qt::AlignLeft);
    
    QVBoxLayout * vLayout=new QVBoxLayout();
    vLayout->setMargin(0);
    vLayout->setSpacing(5);
    hLayout->addLayout(vLayout);
    _lDateStart=new QLabel("Start record time: --",this);
    vLayout->addWidget(_lDateStart,0,Qt::AlignLeft);
    _lDateCurrent=new QLabel("Current record time: --",this);;
    vLayout->addWidget(_lDateCurrent,0,Qt::AlignLeft);
    _lDataSize=new QLabel("Recorded data size: --",this);;
    vLayout->addWidget(_lDataSize,0,Qt::AlignLeft);
    hLayout->addStretch();
    setLayout(hLayout);

    _timer=new QTimer(this);
    _timer->setInterval(100);
    connect(_timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
}
/** @brief activation */
void _RecordWidget::setRecording(bool enable) {
    if (enable) {
        _lIcon->setPixmap(_rPixmap);
        _isSymbolOn=true;
        _opacity=1.0;
        _timer->start();
    } else {
        _timer->stop();
        _lIcon->setPixmap(_rPixmapOff);
        _lDateStart->setText("Start record time: --");
        _lDateCurrent->setText("Current record time: --");
        _lDataSize->setText("Recorded data size: --");
    }
}
/** @brief temps de demarrage */
void _RecordWidget::setStartTime(double stime){
    QDateTime dt;
    dt.setTime_t((uint)stime);
    QString dateText=dt.toString("hh:mm:ss");
    dateText=QString("Start time: %1.%2").arg(dateText).arg(((unsigned int)(stime*1000.0))%1000);
    _lDateStart->setText(dateText);
}
/** @brief temps courant */
void _RecordWidget::setCurrentTime(double ctime){
    QDateTime dt;
    dt.setTime_t((uint)ctime);
    QString dateText=dt.toString("hh:mm:ss");
    dateText=QString("Current time: %1.%2").arg(dateText).arg(((unsigned int)(ctime*1000.0))%1000);
    _lDateCurrent->setText(dateText);
}
/** @brief write data size */
void _RecordWidget::setDataSize(int size){
    _lDataSize->setText(QString("Data size: %1 Ko").arg(((double)size)/1000.0,0,'f',3));
}
/** @brief write data size */
void _RecordWidget::onTimeout() {
    _rPixmapOut.fill(QColor(0,0,0,0));
    if(_isSymbolOn) {
        _opacity-=0.1;
        if (_opacity<=0.1) {
            _isSymbolOn=false;
            _opacity=0.1;
        }
    } else {
        _opacity+=0.1;
        if (_opacity>=0.9) {
            _isSymbolOn=true;
            _opacity=0.9;
        }
    }
    QPainter p(&_rPixmapOut);
    p.setOpacity(1.0-_opacity);
    p.drawPixmap(0,0,_rPixmapOff);
    p.setOpacity(_opacity);
    p.drawPixmap(0,0,_rPixmap);
    _lIcon->setPixmap(_rPixmapOut);
}
