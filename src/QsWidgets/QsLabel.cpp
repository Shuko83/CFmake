
#include <QtGui>

#include "QsLabel.h"

QsLabel::QsLabel(QWidget *parent) : QLabel(parent)
{

}

QsLabel::~QsLabel()
{

}

bool QsLabel::event(QEvent *event)
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

	return QLabel::event(event);
}

