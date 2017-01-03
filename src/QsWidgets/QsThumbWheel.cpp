
#include <QtGui>

#include "QsThumbWheel.h"


QsThumbWheel::QsThumbWheel(QWidget *parent)	: QtThumbWheel(parent)
{
}

QsThumbWheel::~QsThumbWheel()
{

}

bool QsThumbWheel::event(QEvent *event)
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
	return QtThumbWheel::event(event);
}
