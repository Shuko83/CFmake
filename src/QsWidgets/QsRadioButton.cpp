
#include <QtGui>

#include "QsRadioButton.h"


QsRadioButton::QsRadioButton(QWidget *parent)	: QRadioButton(parent)
{

}
QsRadioButton::~QsRadioButton()
{

}

bool QsRadioButton::event(QEvent *event)
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
	return QRadioButton::event(event);
}
