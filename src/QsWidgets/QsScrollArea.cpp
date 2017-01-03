
#include <QtGui>

#include "QsScrollArea.h"

QsScrollArea::QsScrollArea(QWidget *parent)
	: QScrollArea(parent)
{

}

QsScrollArea::~QsScrollArea()
{

}

bool QsScrollArea::event(QEvent *event)
{
	switch(event->type())				
	{
	case QEvent::Enter:
		emit mouseEnter();
		break;

	case QEvent::Leave:
		emit mouseLeave();
		break;
	}
	return QScrollArea::event(event);
}

