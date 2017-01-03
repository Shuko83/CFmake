
#include <QtGui>

#include "QsDoubleSpinBox.h"


QsDoubleSpinBox::QsDoubleSpinBox(QWidget *parent)	: QDoubleSpinBox(parent)
{

}

QsDoubleSpinBox::~QsDoubleSpinBox()
{

}

bool QsDoubleSpinBox::event(QEvent *event)
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
	return QDoubleSpinBox::event(event);
}
