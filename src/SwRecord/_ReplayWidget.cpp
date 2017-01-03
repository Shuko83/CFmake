/**
@file _ReplayWidget.cpp
@brief widget de rejeu
@author F.Bighelli
 */

#include "_ReplayWidget.h"
 
#define CL_MESSAGE_DISABLE "No replay in progress"

/** @brief Constructor */
_ReplayWidget::_ReplayWidget():QWidget(nullptr) {
	_rEnable=false;
    _startTime=0.0;
    _stopTime=0.0;
    _cacheTime=0.0;
    _currentTime=0.0;
    _pPause=QPixmap(":/SwRecord/replay_pause.png");
    _pause=false;
    _pauseProposal=false;
    setMouseTracking(true);
}
/** @brief activation */
void _ReplayWidget::setEnableReplayInformation(bool enable) {
    _rEnable=enable;
    if (!enable)
        _pause=false;
    update();
}
/** @brief start time */
void _ReplayWidget::setStartTime(double vtime) {
    _startTime=vtime;
}
/** @brief stop time */
void _ReplayWidget::setStopTime(double vtime){
    _stopTime=vtime;
}
/** @brief current time */
void _ReplayWidget::setCurrentTime(double vtime){
    _currentTime=vtime;
    if (_rEnable) 
        update();
}
/** @brief cache time */
void _ReplayWidget::setCacheTime(double vtime){
    _cacheTime=vtime;
    if (_rEnable) 
        update();
}
/** @brief renvoie l'etat de la pause */
bool _ReplayWidget::getPauseState() {
    return _pause;
}
/** @brief force l'etat de la pause */
void _ReplayWidget::ForcePauseState() {
    _pause=true;   
}

/** @brief paint event */
void _ReplayWidget::paintEvent ( QPaintEvent * event ) {
    QWidget::paintEvent(event);
    QPainter p(this);
    QFontMetrics fm(this->font());

    //Cas non actif
    if (!_rEnable) {
        p.setPen(QColor(Qt::gray));
        QRect r=fm.boundingRect(CL_MESSAGE_DISABLE);
        r.moveCenter(QPoint(this->width()/2,this->height()/2));
        p.drawText(r,Qt::AlignCenter,CL_MESSAGE_DISABLE);
        return;
    } 
    //Cas actif
    QRect r=this->rect();
    r.setHeight(20);
    r.moveCenter(QPoint(this->width()/2,this->height()/2));
    QLinearGradient lg(0,r.top(),0,r.bottom());
    lg.setColorAt(0,Qt::lightGray);
    lg.setColorAt(1.0,Qt::darkGray);
    p.fillRect(r,QBrush(lg));
    r=this->rect();
    r.setHeight(18);
    r.moveCenter(QPoint(this->width()/2,this->height()/2));
    r.setWidth((int)(((double)r.width())*(_cacheTime-_startTime)/(_stopTime-_startTime)));
    p.fillRect(r,QColor(255,0,0,80));
    r=this->rect();
    r.setHeight(18);
    r.moveCenter(QPoint(this->width()/2,this->height()/2));
    r.setWidth((int)(((double)r.width())*(_currentTime-_startTime)/(_stopTime-_startTime)));
    p.fillRect(r,QColor(255,0,0,255));
    QRect rjauge=r;



    QString tmp=timeToTextTime(_startTime);
    r=fm.boundingRect(tmp);
    r.moveCenter(QPoint(this->width()/2,this->height()/2+20));
    r.moveLeft(10);
    p.drawText(r,Qt::AlignLeft,tmp);

    tmp=timeToTextTime(_stopTime);
    r=fm.boundingRect(tmp);
    r.moveCenter(QPoint(this->width()/2,this->height()/2+20));
    r.moveRight(this->width()-10);
    p.drawText(r,Qt::AlignRight,tmp);

    tmp=timeToTextTime(_currentTime);
    r=fm.boundingRect(tmp);
    r.moveCenter(QPoint(this->width()/2,this->height()/2+20));
    p.drawText(r,Qt::AlignCenter,tmp);

    int pos=(int)(((double)width())*(_currentTime-_startTime)/(_stopTime-_startTime));
    //QPen pen;
    //pen.setWidth(2);
    //p.setPen(pen);
    p.drawLine(pos,rjauge.top(),pos,rjauge.bottom());
    
    QPainterPath ppath;
    ppath.moveTo(pos,rjauge.top()+5);
    ppath.lineTo(pos+5,rjauge.top()-5);
    ppath.lineTo(pos-5,rjauge.top()-5);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillPath(ppath,QBrush(Qt::black));
    p.setRenderHint(QPainter::Antialiasing,false);

    r=_pPause.rect();
    if (r.height()>=this->height()-10) {
        r.setHeight(this->height()-10);
        r.setWidth(this->height()-10);
    }
    r.moveCenter(QPoint(this->width()/2,this->height()/2));
    if(_pause) {
        p.drawPixmap(r,_pPause);
    } else if (_pauseProposal) {
        p.setOpacity(0.6);
        p.drawPixmap(r,_pPause);
    }

}
/** @brief enter event */
void _ReplayWidget::enterEvent ( QEvent * /*event*/ ) {
    _pauseProposal=true;
    update();
}
/** @brief leave event */
void _ReplayWidget::leaveEvent ( QEvent * /*event*/ ) {
    _pauseProposal=false;
    update();
}
/** @brief press event */
void _ReplayWidget::mousePressEvent ( QMouseEvent * /*event*/ ) {
    _pause=!_pause;
    update();
}
/* @brief time to text time */
QString _ReplayWidget::timeToTextTime(double vtime) {
    QDateTime dt;
    dt.setTime_t((uint)vtime);
    QString dateText=dt.toString("hh:mm:ss");
    dateText=QString("%1.%2").arg(dateText).arg(((unsigned int)(vtime*1000.0))%1000);
    return dateText;
}

