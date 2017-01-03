
#include <QtGui>

#include "QsFrame.h"


QsFrame::QsFrame(QWidget *parent)    : QFrame(parent)
{

}

QsFrame::~QsFrame()
{

}

bool QsFrame::event(QEvent *event)
{
	switch (event->type())
	{
	case QEvent::Enter:
		emit mouseEnter();
		break;

	case QEvent::Leave:
		emit mouseLeave();
		break;
	}

	return QFrame::event(event);
}

