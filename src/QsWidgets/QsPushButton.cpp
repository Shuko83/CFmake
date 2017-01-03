
#include <QtGui>

#include "QsPushButton.h"

QsPushButton::QsPushButton(QWidget *parent)	: QPushButton(parent)
{

}

QsPushButton::~QsPushButton()
{

}

bool QsPushButton::event(QEvent *event)
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
	return QPushButton::event(event);
}
