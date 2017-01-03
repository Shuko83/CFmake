
#include <QtGui>

#include "QsSlider.h"


QsSlider::QsSlider(QWidget *parent)	: QSlider(parent)
{

}

QsSlider::~QsSlider()
{

}

bool QsSlider::event(QEvent *event)
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
	return QSlider::event(event);
}
