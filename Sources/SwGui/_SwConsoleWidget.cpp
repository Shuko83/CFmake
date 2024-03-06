/*
 * Classe fenetre principale
 */
#include <QPainter>
#include <QScrollArea>
#include "_SwConsoleWidget.h"
#include <QTimer>

_SwConsoleWidget::_SwConsoleWidget(QWidget *parent):QWidget(parent)
{
	setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    _fHeight=fontMetrics().height()+2;
	_maxMessages=200; //Max messages 200;
	_maxWidth=0;
	QTimer *timerRefresh = new QTimer(this);
    connect(timerRefresh, SIGNAL(timeout()), this, SLOT(refreshContent()));
    timerRefresh->start(200); 
	_hasChanged=true;
}

_SwConsoleWidget::~_SwConsoleWidget(void)
{

}
QWidget * _SwConsoleWidget::buildScrollableConsole(QWidget *parent,_SwConsoleWidget ** console) {
	QScrollArea * sa=new QScrollArea(parent);
	sa->setStyleSheet("color: white;background-color: black;");
    _SwConsoleWidget *c=new _SwConsoleWidget(sa);
    (* console)=c;
	sa->setWidget(c);
    sa->setWindowTitle("Console");
	return sa;
}
void  _SwConsoleWidget::addMessage(QString message) {
    QString tmp=message;
    tmp.replace("\r","");
    QStringList list=tmp.split("\n");
    QList<QString>::iterator it=list.begin();
    while (it!=list.end()) {
        if (!(*it).isEmpty())
            _messages<<(*it);
        it++;
    }
	if (_messages.size()>_maxMessages) {
		_messages.erase(_messages.begin(),_messages.begin()+(_messages.size()-_maxMessages));
	}
	int iwidth = fontMetrics().width(message)+4;
	if (_maxWidth<iwidth) _maxWidth=iwidth;
	_hasChanged=true;
}

void _SwConsoleWidget::paintEvent ( QPaintEvent * event ) {
	if (_messages.size()==0)
		return;
	QPainter p(this);
	QRect r=event->rect();
	int start_index=r.top()/_fHeight;
	int stop_index=(r.bottom()/_fHeight)+1;
	if (start_index<0) start_index=0;
	if (start_index>_messages.size()-1) start_index=_messages.size()-1;
	if (stop_index<0) stop_index=0;
	if (stop_index>_messages.size()-1) stop_index=_messages.size()-1;
	if (start_index>stop_index) {
		int tmp=start_index;
		start_index=stop_index;
		stop_index=tmp;
	}
	for(int i=start_index;i<=stop_index;i++) 
	{
		QString messageToDisplay = _messages.at(i);
		p.setPen(QColor("#FFFFFF"));
		if(_messages.at(i).startsWith("I:"))
		{
			p.setPen(QColor("#2086ee"));
			messageToDisplay = messageToDisplay.mid(2,messageToDisplay.length()-2);
		}
		if(_messages.at(i).startsWith("C:"))
		{
			p.setPen(QColor("#ce0606"));
			messageToDisplay =messageToDisplay.mid(2,messageToDisplay.length()-2);
		}
		if(_messages.at(i).startsWith("W:"))
		{
			p.setPen(QColor("#ee8120"));
			messageToDisplay =messageToDisplay.mid(2,messageToDisplay.length()-2);
		}
		if(_messages.at(i).startsWith("E:"))
		{
			p.setPen(QColor("#8d1698"));
			messageToDisplay =messageToDisplay.mid(2,messageToDisplay.length()-2);
		}

		p.drawText(QPoint(4,_fHeight+_fHeight*i),messageToDisplay);
	}
}
void _SwConsoleWidget::refreshContent() {
	if (_hasChanged==true) {
		int iheight=_messages.size()*_fHeight;
		int iwidth = _maxWidth;
		iheight=iheight>height()?iheight:height();
		iwidth=iwidth>width()?iwidth:width();
		if (iheight!=height() || iwidth!=width()) {
			resize(iwidth,iheight);
		} else {
			update();
		}
		_hasChanged=false;
	}
}
 
