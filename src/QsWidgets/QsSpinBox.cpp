
#include <QtGui>

#include "QsSpinBox.h"

QsSpinBox::QsSpinBox(QWidget *parent)	: QSpinBox(parent)
{

}

QsSpinBox::~QsSpinBox()
{

}

bool QsSpinBox::event(QEvent *event)
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
	return QSpinBox::event(event);
}
