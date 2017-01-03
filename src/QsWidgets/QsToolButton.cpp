
#include <QtGui>

#include "QsToolButton.h"


QsToolButton::QsToolButton(QWidget *parent) : QToolButton(parent)
{

}
QsToolButton::~QsToolButton()
{

}

bool QsToolButton::event(QEvent *event)
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

	return QToolButton::event(event);
}
