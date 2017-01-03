
#include <QtGui>

#include "QsCheckBox.h"


QsCheckBox::QsCheckBox(QWidget *parent)	: QCheckBox(parent)
{

}

QsCheckBox::~QsCheckBox()
{

}

bool QsCheckBox::event(QEvent *event)
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
	return QCheckBox::event(event);
}
